#version 330 core

in vec2 teTexCoord;
in vec3 teWorldPos;
in vec3 teNormal;

uniform vec3 lightPosition[4];
uniform vec3 lightColor[4];
uniform vec3 cameraPosition;

uniform vec3 albedo;
uniform vec3 sheenColor;
uniform float normal;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform bool b_lam;

uniform float aperture;
uniform float shutterSpeed;
uniform float sensitivity;

out vec4 FragColor;

const float Pi = 3.14159265359;
#define MEDIUMP_FLT_MAX 65504.0
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)

float D_Ashikhmin(float roughness, float NoH)
{
    float a2 = roughness * roughness;
    float cos2h = NoH * NoH;
    float sin2h = max(1.0f - cos2h, 0.0078125f);
    float sin4h = sin2h * sin2h;
    float cot2 = -cos2h / (a2 * sin2h);
    return 1.0f / (Pi * (4.0f * a2 + 1.0f) * sin4h) * (4.0f * exp(cot2) + sin4h);
}

float distributionCloth(float roughness, float NoH)
{
    return D_Ashikhmin(roughness, NoH);
}

float V_Neubelt(float NoV, float NoL)
{
    return saturateMediump(1.0f / (4.0f * (NoL + NoV - NoL * NoV)));
}

float visibilityCloth(float NoV, float NoL)
{
    return V_Neubelt(NoV, NoL);
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

    vec3 normal = teNormal;
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

        float D = distributionCloth(roughness, NtoH);
        float V = visibilityCloth(NtoV, NtoL);
        vec3 F = sheenColor;
        vec3 Fr = (D * V) * F;
        
        vec3 diffuseColor = (1.0f - metallic) * albedo;
        vec3 Kd = vec3(0.0f);
        if(!b_lam)
        {
		    Kd = diffuseColor * Fd_Burley(NtoV, NtoL, LtoH, roughness);
        }
        else
        {
            Kd = diffuseColor * Fd_Lambert();
        }

		// add to outgoing radiance Lo
		float normal_to_halfway = max(dot(N, L), 0.0f);
        Lo += Kd + Fr;
	}

	vec3 ambient = vec3(0.03f) * albedo * ao;
	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0f));
	color = pow(color, vec3(1.0f / 2.2f));

    color *= exposure;

	FragColor = vec4(color, 1.0f);
}
