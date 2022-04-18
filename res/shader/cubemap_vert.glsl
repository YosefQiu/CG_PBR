#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 P;
uniform mat4 V;

void main()
{
	WorldPos = aPos;
	gl_Position = P * V * vec4(WorldPos, 1.0f);
}