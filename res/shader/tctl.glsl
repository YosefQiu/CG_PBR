#version 410 core

layout(vertices = 3) out;


in vec2 TexCoord[];
in vec3 Normal[];

out vec2 tcTexCoord[];
out vec3 tcNormal[];


uniform float innerLevel;
uniform float outerLevel;


void main()
{
	tcTexCoord[gl_InvocationID] = TexCoord[gl_InvocationID];
	tcNormal[gl_InvocationID] = Normal[gl_InvocationID];

	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = innerLevel;

		gl_TessLevelOuter[0] = outerLevel;
		gl_TessLevelOuter[1] = outerLevel;
		gl_TessLevelOuter[2] = outerLevel;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
