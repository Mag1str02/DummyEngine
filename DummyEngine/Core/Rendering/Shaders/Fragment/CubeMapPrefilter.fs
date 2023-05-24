#version 410 core

const uint k_SampleDelta = 1024u;
const float kPI = 3.14159265359;

out vec4 f_FragColor;
in vec3 v_TexCoords;

uniform samplerCube u_CubeMap;
uniform float u_Roughness;

float RadicalInverseVDC(uint n);
vec2  Hammersley(uint i, uint n);
vec3  ImportanceSampleGGX(vec2 x, vec3 n, float roughness);
  
void main()
{		
    vec3 N = normalize(v_TexCoords);    
    vec3 R = N;
    vec3 V = R;

    float totalWeight = 0.0;   
    vec3 prefilteredColor = vec3(0.0);     
    for(uint i = 0u; i < k_SampleDelta; ++i)
    {
        vec2 Xi = Hammersley(i, k_SampleDelta);
        vec3 H  = ImportanceSampleGGX(Xi, N, u_Roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            prefilteredColor += texture(u_CubeMap, L).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / totalWeight;

    f_FragColor = vec4(prefilteredColor, 1.0);
} 
//Magic XD
float RadicalInverseVDC(uint n) 
{
     n = (n << 16u) | (n >> 16u);
     n = ((n & 0x55555555u) << 1u) | ((n & 0xAAAAAAAAu) >> 1u);
     n = ((n & 0x33333333u) << 2u) | ((n & 0xCCCCCCCCu) >> 2u);
     n = ((n & 0x0F0F0F0Fu) << 4u) | ((n & 0xF0F0F0F0u) >> 4u);
     n = ((n & 0x00FF00FFu) << 8u) | ((n & 0xFF00FF00u) >> 8u);
     return float(n) * 2.3283064365386963e-10;
}
vec2 Hammersley(uint i, uint n)
{
	return vec2(float(i) / float(n), RadicalInverseVDC(i));
}
vec3 ImportanceSampleGGX(vec2 x, vec3 n, float roughness)
{
	float a = roughness * roughness;
	float phi = 2.0 * kPI * x.x;
	float cos_t = sqrt((1.0 - x.y) / (1.0 + (a * a - 1.0) * x.y));
	float sin_t = sqrt(1.0 - cos_t * cos_t);
	vec3 coords;
	coords.x = cos(phi) * sin_t;
	coords.y = sin(phi) * sin_t;
	coords.z = cos_t;

	vec3 up          = (abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0));
	vec3 tangent   = normalize(cross(up, n));
	vec3 bitangent = cross(n, tangent);
	
	vec3 vec = tangent * coords.x + bitangent * coords.y + n * coords.z;
	return normalize(vec);
}
