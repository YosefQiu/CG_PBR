#version 330 core

in vec3 WorldPos;

uniform samplerCube environmentMap;

out vec4 FragColor;

void main()
{
	vec3 envColor = texture(environmentMap, WorldPos).rgb;

	envColor = envColor / (envColor + vec3(1.0f));
	envColor = pow(envColor, vec3(1.0f / 2.2f));

	FragColor = vec4(envColor, 1.0f);
}