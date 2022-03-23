#version 330 core
layout (location = 0) in vec3 aPos;


out vec3 TexCoord;

uniform mat4 P;
uniform mat4 V;

void main()
{
	TexCoord = aPos;
	vec4 pos = P * V * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
}