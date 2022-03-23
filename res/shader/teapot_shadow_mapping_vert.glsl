#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0f);
	FragPos = (M * vec4(aPos.xyz, 1.0f)).xyz;
	Normal = mat3(transpose(inverse(M))) * aNormal;
	TexCoord = aTexCoord;
}