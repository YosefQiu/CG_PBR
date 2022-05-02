#version 330 core

precision mediump float;

in vec2 teTexCoord;
in vec3 teWorldPos;
in vec3 teNormal;

out vec4 FragColor;

//light
uniform vec3 lightPosition[4];
uniform vec3 lightColor[4];
uniform vec3 cameraPosition;

//camera
uniform float aperture;
uniform float shutterSpeed;
uniform float sensitivity;

//noise
uniform vec2 u_resolution;
uniform float u_time;
uniform bool b_tex;

//brdf
uniform bool b_lam;
uniform float metallic;
uniform float roughness;
uniform float ao;

const float Pi = 3.14159265359;
#define MEDIUMP_FLT_MAX 65504.0
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)


float random(in vec2 _st) 
{
    return fract(sin(dot(_st.xy,
        vec2(12.9898, 78.233))) *
        43758.5453123);
}

//Morgan McGuire
float noise(in vec2 _st) 
{
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
        (c - a) * u.y * (1.0 - u.x) +
        (d - b) * u.x * u.y;
}

#define NUM_OCTAVES 5

float fbm(in vec2 _st) 
{
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5),
        -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) 
    {
        v += a * noise(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}




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
//    return saturateMediump(d);
    
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


// Some useful functions
vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x * 34.0) + 1.0) * x); }

//
// Description : GLSL 2D simplex noise function
//      Author : Ian McEwan, Ashima Arts
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License :
//  Copyright (C) 2011 Ashima Arts. All rights reserved.
//  Distributed under the MIT License. See LICENSE file.
//  https://github.com/ashima/webgl-noise
//
float snoise(vec2 v) {

    // Precompute values for skewed triangular grid
    const vec4 C = vec4(0.211324865405187,
        // (3.0-sqrt(3.0))/6.0
        0.366025403784439,
        // 0.5*(sqrt(3.0)-1.0)
        -0.577350269189626,
        // -1.0 + 2.0 * C.x
        0.024390243902439);
    // 1.0 / 41.0

// First corner (x0)
    vec2 i = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);

    // Other two corners (x1, x2)
    vec2 i1 = vec2(0.0);
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec2 x1 = x0.xy + C.xx - i1;
    vec2 x2 = x0.xy + C.zz;

    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    vec3 p = permute(
        permute(i.y + vec3(0.0, i1.y, 1.0))
        + i.x + vec3(0.0, i1.x, 1.0));

    vec3 m = max(0.5 - vec3(
        dot(x0, x0),
        dot(x1, x1),
        dot(x2, x2)
    ), 0.0);

    m = m * m;
    m = m * m;

    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);

    // Compute final noise value at P
    vec3 g = vec3(0.0);
    g.x = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * vec2(x1.x, x2.x) + h.yz * vec2(x1.y, x2.y);
    return 130.0 * dot(m, g);
}

#define OCTAVES 4

// Ridged multifractal
// See "Texturing & Modeling, A Procedural Approach", Chapter 12
float ridge(float h, float offset) {
    h = abs(h);     // create creases
    h = offset - h; // invert so creases are at top
    h = h * h;      // sharpen creases
    return h;
}

float ridgedMF(vec2 p) {
    float lacunarity = 2.0;
    float gain = 0.5;
    float offset = 0.9;

    float sum = 0.0;
    float freq = 1.0, amp = 0.5;
    float prev = 1.0;
    for (int i = 0; i < OCTAVES; i++) {
        float n = ridge(snoise(p * freq), offset);
        sum += n * amp;
        sum += n * amp * prev;  // scale by previous octave
        prev = n;
        freq *= lacunarity;
        amp *= gain;
    }
    return sum;
}



void main()
{
   
    vec3 color = vec3(0.0);
    vec3 albedo = vec3(0.0f);
    if (b_tex)
    {
        vec2 st = gl_FragCoord.xy / u_resolution.xy;
        st.x *= u_resolution.x / u_resolution.y;
        
        color += ridgedMF(st * 3.0);
        albedo = color * u_time;
    }
    else
    {
        vec2 st = gl_FragCoord.xy / u_resolution.xy * 3.;
        // st += st * abs(sin(u_time*0.1)*3.0);
        vec3 color = vec3(0.0);

        vec2 q = vec2(0.);
        q.x = fbm(st + 0.00 * u_time);
        q.y = fbm(st + vec2(1.0));

        vec2 r = vec2(0.);
        r.x = fbm(st + 1.0 * q + vec2(1.7, 9.2) + 0.15 * u_time);
        r.y = fbm(st + 1.0 * q + vec2(8.3, 2.8) + 0.126 * u_time);

        float f = fbm(st + r);

        color = mix(vec3(0.101961, 0.619608, 0.666667),
            vec3(0.666667, 0.666667, 0.498039),
            clamp((f * f) * 4.0, 0.0, 1.0));

        color = mix(color,
            vec3(0, 0, 0.164706),
            clamp(length(q), 0.0, 1.0));

        color = mix(color,
            vec3(0.666667, 1, 1),
            clamp(length(r.x), 0.0, 1.0));
        albedo = vec3((f * f * f + .6 * f * f + .5 * f) * color);
    }


    
    float ev100 = exposureSettings(aperture, shutterSpeed, sensitivity);
    float exposure = exposure(ev100);
    
    //vec3 albedo =vec3((f * f * f + .6 * f * f + .5 * f) * color);
    //vec3 albedo = color * u_time;
    vec3 normal = normalize(teNormal);
    
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
        vec3 diffuseColor = (1.0f - metallic) * color;
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
    
    vec3 result = ambient + Lo;

    result = result / (result + vec3(1.0f));
    result = pow(result, vec3(1.0f / 2.2f));

    result *= exposure;
    
    FragColor = vec4(result, 1.0f);
}
