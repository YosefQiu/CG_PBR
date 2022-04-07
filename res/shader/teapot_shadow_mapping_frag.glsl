#version 330 core

precision mediump float;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


struct Texture
{
	sampler2D diffuse;
	sampler2D specualr;
};

struct Material
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Ns;
};

struct LightDirection
{
	vec3 color;
	vec3 dirToLight;
};



out vec4 FragColor;

uniform vec3			AmbientClr;
uniform vec3			CameraPos;
uniform Material		mat;
uniform Texture			tex;
uniform LightDirection	lightD;
uniform bool b_lightD;


vec3 CalcLightD(LightDirection light, vec3 Normal, vec3 dirToCamera, Material mat, Texture tex)
{
	//diffuse
	float diffIntensity = max(dot(light.dirToLight, Normal), 0.0f);
	vec3 diff = texture(tex.diffuse, TexCoord).rgb * diffIntensity * light.color;

	//specualr
	vec3 halfDir = normalize(light.dirToLight + dirToCamera);
	float specularIntensity = pow(max(dot(Normal, halfDir), 0.0f), mat.Ns);
	vec3 spec = texture(tex.specualr, TexCoord).rgb *  specularIntensity * light.color;

	//ambient
	vec3 ambi =  texture(tex.diffuse, TexCoord).rgb * AmbientClr;

	//reslut
	vec3 result = (ambi + diff + spec);
	return result;
}



void main()
{
	vec3 finalResult = vec3(0.0f, 0.0f, 0.0f);
	vec3 dirToCamera = normalize(CameraPos - FragPos);
	finalResult += CalcLightD(lightD, Normal, dirToCamera, mat, tex);
	
	FragColor = vec4(finalResult, 1.0f);
}