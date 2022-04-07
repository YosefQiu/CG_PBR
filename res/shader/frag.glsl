#version 330 core

in vec2 teTexCoord;
in vec3 teWorldPos;
in vec3 teNormal;

uniform vec3 lightPosition[4];
uniform vec3 lightColor[4];
uniform vec3 cameraPosition;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform bool b_lam;

uniform float aperture;
uniform float shutterSpeed;
uniform float sensitivity;

out vec4 FragColor;

const float Pi = 3.14159265359;
#define MEDIUMP_FLT_MAX 65504.0
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)

// Trowbridge-Reitez GGX
// NDF(n, h, a) =  a ^ 2 / denom
// denom = pi((n * h) ^ 2 * (a ^ 2 - 1) + 1) ^ 2

float D_GGX(vec3 N, vec3 H, float NtoH, float roughness)
{

//    vec3 NxH = cross(N, H);
//    float a = NtoH * roughness;
//    float k = roughness / (dot(NxH, NxH) + a * a);
//    float d = k * k * (1.0f / Pi);
//
//	return saturateMediump(d);
    
    float a = roughness * roughness;
    float a_square = a * a;
    float normal_to_halfway = max(dot(N, H), 0.0f);
    float normal_to_halfway_square = normal_to_halfway * normal_to_halfway;
    float denom = (normal_to_halfway_square * (a_square - 1.0f) + 1.0f);
    denom = Pi * denom * denom;

    return a_square / denom;
}

// Smith's Schlick-GGX
// GoemF(n, v, k) = (n * v) / [(n * v)(1 - k) + k]
// GeomSmith = G_sub(n,v,k)G_sub(n, l, k)
float GeometrySchlickGGX(float normal_to_halfway, float k)
{
	float denom = normal_to_halfway * (1.0f - k) + k;

	return normal_to_halfway / denom;
}

float GeoSmith(vec3 N, vec3 V, vec3 L, float k)
{
	float normal_to_halfway = max(dot(N, V), 0.0f);
	float normal_to_light = max(dot(N, L), 0.0f);
	float ggx1 = GeometrySchlickGGX(normal_to_halfway, k);
	float ggx2 = GeometrySchlickGGX(normal_to_light, k);

	return ggx1 * ggx2;

}

// Fresnel-Schlick
// Frensel(h, v, F0) = F0 + (1 - F0)(1 - (h * v)) ^ 5
vec3 Frensel(float cosTheta, vec3 F0)
{
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

vec3 getNormalFromNormalMap()
{
	vec3 tangentNormal = texture(normalMap, teTexCoord).xyz * 2.0f - 1.0f;

	vec3 Q1 = dFdx(teWorldPos);
	vec3 Q2 = dFdy(teWorldPos);
	vec2 st1 = dFdx(teTexCoord);
	vec2 st2 = dFdy(teTexCoord);

	vec3 N = normalize(teNormal);
	vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

//Lambert
float Fd_Lambert()
{
    return 1.0f / Pi;
}

float F_Schlick(float u, float F0, float F90)
{
    return F0 + (F90 - F0) * pow(1.0f - u, 5.0f);
}

float Fd_Burley(float NtoV, float NtoL, float LtoH, float roughness)
{
    float F90 = 0.5f + 2.0f * roughness * LtoH * LtoH;
    float lightScatter = F_Schlick(NtoL, 1.0f, F90);
    float viewScatter = F_Schlick(NtoV, 1.0f, F90);
    return lightScatter * viewScatter * (1.0f / Pi);
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

void main()
{

    float ev100 = exposureSettings(aperture, shutterSpeed, sensitivity);
    float exposure = exposure(ev100);

	vec3 albedo = pow(texture(albedoMap, teTexCoord).rgb, vec3(2.2f));
	vec3 normal = getNormalFromNormalMap();
	float metallic = texture(metallicMap, teTexCoord).r;
	float roughness = texture(roughnessMap, teTexCoord).r;
	float ao = texture(aoMap, teTexCoord).r;

	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPosition - teWorldPos);

	vec3 F0 = vec3(0.04f);
	F0 = mix(F0, albedo, metallic);

	//reflectance equation
	vec3 Lo = vec3(0.0f, 0.0f, 0.0f);
	for(int i = 0; i < 4; i++)
	{ 
		// Calc per-light radiance
		vec3 L = normalize(lightPosition[i] - teWorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPosition[i] - teWorldPos);
		float attenuation = 1.0f / (distance * distance);
		vec3 radiance = lightColor[i] * attenuation;

        float NtoV = abs(dot(N, V)) + 1e-5;
        float NtoL = clamp(dot(N, L), 0.0f, 1.0f);
        float NtoH = clamp(dot(N, H), 0.0f, 1.0f);
        float LtoH = clamp(dot(L, H), 0.0f, 1.0f);

		// cook-torrance BRDF
		float D = D_GGX(N, H, NtoH, roughness);
		float G = GeoSmith(N, V, L, roughness);
		vec3 F = Frensel(max(dot(H, V), 0.0f), F0);

		vec3 Ks = F;
        vec3 diffuseColor = (1.0f - metallic) * texture(albedoMap, teTexCoord).rgb;
        vec3 Kd = vec3(0.0f);
        if(!b_lam)
        {
		    Kd = diffuseColor * Fd_Burley(NtoV, NtoL, LtoH, roughness);
        }
        else
        {
            Kd = diffuseColor * Fd_Lambert();
        }

		vec3 nominator = D * G * F;
		float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
		vec3 specular = nominator / denominator;

		// add to outgoing radiance Lo
		float normal_to_halfway = max(dot(N, L), 0.0f);
		Lo += (Kd * albedo / Pi + specular) * radiance * normal_to_halfway;
	}

	vec3 ambient = vec3(0.03f) * albedo * ao;
	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0f));
	color = pow(color, vec3(1.0f / 2.2f));

    color *= exposure;

	FragColor = vec4(color, 1.0f);
}
