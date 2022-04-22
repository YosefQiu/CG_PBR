#version 330 core

in vec2 teTexCoord;
in vec3 teNormal;
in vec3 teWorldPos;

// material parameters
uniform vec3 basecolor;
uniform float subsurface;
uniform float metallic;
uniform float specular;
uniform float specularTint;
uniform float roughness;
uniform float anisotropic;
uniform float sheen;
uniform float sheenTint;
uniform float clearcoat;
uniform float clearcoatGloss;

// camera parameter
uniform float aperture;
uniform float shutterSpeed;
uniform float sensitivity;

//lights
uniform vec3 lightPosition[4];
uniform vec3 lightColor[4];
uniform vec3 cameraPosition;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
uniform bool b_ibl;

out vec4 FragColor;

const float PI = 3.14159265358979323846;


mat3 CalcTBN(vec3 N, vec3 P, vec2 UV)
{
    vec3 Q1 = dFdx(teWorldPos);
    vec3 Q2 = dFdy(teWorldPos);
    vec2 st1 = dFdx(teTexCoord);
    vec2 st2 = dFdy(teTexCoord);

    vec3 Ni = normalize(N);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(Ni, T));
    mat3 TBN = mat3(T, B, Ni);

    return TBN;
}

float sqr(float x) 
{ 
    return x * x;
}

float SchlickFresnel(float u)
{
    float m = clamp(1 - u, 0, 1);
    float m2 = m * m;
    return m2 * m2 * m; // pow(m,5)
}

float GTR1(float NdotH, float a)
{
    if (a >= 1) return 1 / PI;
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return (a2 - 1) / (PI * log(a2) * t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return a2 / (PI * t * t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
    return 1 / (PI * ax * ay * sqr(sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH));
}

float smithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1 / (NdotV + sqrt(a + b - a * b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
    return 1 / (NdotV + sqrt(sqr(VdotX * ax) + sqr(VdotY * ay) + sqr(NdotV)));
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}


vec3 BRDF(vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y)
{
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    if (NdotL < 0 || NdotV < 0) return vec3(0);

    vec3 H = normalize(L + V);
    float NdotH = dot(N, H);
    float LdotH = dot(L, H);

    vec3 Cdlin = mon2lin(basecolor);
    float Cdlum = .3 * Cdlin[0] + .6 * Cdlin[1] + .1 * Cdlin[2]; // luminance approx.

    vec3 Ctint = Cdlum > 0 ? Cdlin / Cdlum : vec3(1); // normalize lum. to isolate hue+sat
    vec3 Cspec0 = mix(specular * .08 * mix(vec3(1), Ctint, specularTint), Cdlin, metallic);
    vec3 Csheen = mix(vec3(1), Ctint, sheenTint);

    // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
    // and mix in diffuse retro-reflection based on roughness
    float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
    float Fd90 = 0.5 + 2 * LdotH * LdotH * roughness;
    float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

    // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
    // 1.25 scale is used to (roughly) preserve albedo
    // Fss90 used to "flatten" retroreflection based on roughness
    float Fss90 = LdotH * LdotH * roughness;
    float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
    float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5);

    // specular
    float aspect = sqrt(1 - anisotropic * .9);
    float ax = max(.001, sqr(roughness) / aspect);
    float ay = max(.001, sqr(roughness) * aspect);
    float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
    float FH = SchlickFresnel(LdotH);
    vec3 Fs = mix(Cspec0, vec3(1), FH);
    float Gs;
    Gs = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay);
    Gs *= smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

    // sheen
    vec3 Fsheen = FH * sheen * Csheen;

    // clearcoat (ior = 1.5 -> F0 = 0.04)
    float Dr = GTR1(NdotH, mix(.1, .001, clearcoatGloss));
    float Fr = mix(.04, 1.0, FH);
    float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25);

    return ((1 / PI) * mix(Fd, ss, subsurface) * Cdlin + Fsheen)
        * (1 - metallic)
        + Gs * Fs * Ds + .25 * clearcoat * Gr * Fr * Dr;
}

//camera
float exposureSettings(float aperture, float shutterSpeed, float sensitivity)
{
    return log2((aperture * aperture) / shutterSpeed * 100.0f / sensitivity);
}

float exposure(float ev100)
{
    return 1.0f / (pow(2.0f, ev100) * 1.2f);
}

vec3 F_Schlick(float cosTheta, vec3 f0)
{
    return f0 + (1.0f - f0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}


void main()
{
    float ev100 = exposureSettings(aperture, shutterSpeed, sensitivity);
    float exposure = exposure(ev100);

    mat3 TBN = CalcTBN(teNormal, teWorldPos, teTexCoord);

    vec3 N = normalize(teNormal);
    vec3 T = transpose(TBN)[0];
    vec3 B = transpose(TBN)[1];
    vec3 V = normalize(cameraPosition - teWorldPos);
    vec3 R = reflect(-V, N);
    vec3 Lo = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; i++)
    {
        vec3 L = normalize(lightPosition[i] - teWorldPos);
        vec3 H = normalize(V + L);
        
        float distance = length(lightPosition[i] - teWorldPos);
        float attenuation = 1.0f / (distance * distance);
        vec3 radiance = lightColor[i] * attenuation;

        Lo += BRDF(L, V, N, T, B) * radiance;
    }

    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, basecolor, metallic);
    vec3 kS = F_Schlick(max(dot(N, V), 0.0f), F0);
    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * basecolor;
    vec3 ambient = vec3(0.0f);
    if (b_ibl)
    {
        const float MAX_REFLECTION_LOD = 4.0f;
        vec3 prefilteredColor = texture(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);


        ambient = (kD * diffuse + specular);
    }
    else
    {
        ambient = vec3(0.03) * basecolor;
    }


    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f / 2.2f));

    //color *= exposure;

    FragColor = vec4(color, 1.0f);

}