#version 410 core
layout(triangles, equal_spacing, cw) in;

in vec2 tcTexCoord[];
in vec3 tcNormal[];

uniform float fact;

uniform sampler2D heightMap;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;


out vec2 teTexCoord;
out vec3 teWorldPos;
out vec3 teNormal;






void main()
{

    vec4 p0 = gl_TessCoord.x * gl_in[0].gl_Position;
    vec4 p1 = gl_TessCoord.y * gl_in[1].gl_Position;
    vec4 p2 = gl_TessCoord.z * gl_in[2].gl_Position;
    vec4 p = p0 + p1 + p2;

    vec3 n0 = gl_TessCoord.x * tcNormal[0];
    vec3 n1 = gl_TessCoord.y * tcNormal[1];
    vec3 n2 = gl_TessCoord.z * tcNormal[2];
    vec3 normal = normalize(n0 + n1 + n2);

    vec2 t0 = gl_TessCoord.x * tcTexCoord[0];
    vec2 t1 = gl_TessCoord.y * tcTexCoord[1];
    vec2 t2 = gl_TessCoord.z * tcTexCoord[2];
    vec2 texCoord = t0 + t1 + t2;

    float height = texture(heightMap, texCoord).r;

    p += vec4((1.0f) * normal * (height * fact), 1.0f);
    
    gl_Position = P * V * M * p;   
    teTexCoord = texCoord;
    teNormal = mat3(M) * normal;
    teWorldPos = vec3(M * p);


}
