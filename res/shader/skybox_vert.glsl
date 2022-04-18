#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 P;
uniform mat4 V;

void main()
{
	WorldPos = aPos;
	mat4 rotView = mat4(mat3(V));
	vec4 clipPos = P * rotView * vec4(WorldPos, 1.0f);
	gl_Position = clipPos.xyww;
}