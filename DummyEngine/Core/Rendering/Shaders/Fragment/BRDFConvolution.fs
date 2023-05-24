#version 410 core

const uint kSampleCount = 1024u;
const float kPI = 3.14159265359;

out vec4 FragColor;
in vec2 v_TexCoords;

float RadicalInverseVDC(uint n);
vec2  Hammersley(uint i, uint n);
vec3  ImportanceSampleGGX(vec2 x, vec3 n, float roughness);
float GeometrySchlickGGX(float angle_cos, float roughness);
float GeometrySmith(vec3 n, vec3 vec, vec3 light, float roughness);
vec2  IntegrateBRDF(float angle_cos, float roughness);

void main() 
{
    FragColor = vec4(IntegrateBRDF(v_TexCoords.x, v_TexCoords.y), 0.0, 1.0);
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
float GeometrySchlickGGX(float angle_cos, float roughness)
{
    float a = roughness;
    float k = (a * a) / 2.0;

    return angle_cos / (angle_cos * (1.0 - k) + k);
}
float GeometrySmith(vec3 n_normal, vec3 n_view, vec3 n_light, float roughness)
{	
    float nv_angle = max(dot(n_normal, n_view), 0.0);
    float nl_angle = max(dot(n_normal, n_light), 0.0);
    return GeometrySchlickGGX(nv_angle, roughness) * GeometrySchlickGGX(nl_angle, roughness);
}
vec2 IntegrateBRDF(float angle_cos, float roughness)
{
    vec3 vec;
    vec.x = sqrt(1.0 - angle_cos * angle_cos);
    vec.y = 0.0;
    vec.z = angle_cos;

    float scale = 0.0;
    float bias = 0.0; 
    vec3 n = vec3(0.0, 0.0, 1.0);
    
    for(uint i = 0u; i < kSampleCount; ++i)
    {
        vec2 x = Hammersley(i, kSampleCount);
        vec3 coords = ImportanceSampleGGX(x, n, roughness);
        vec3 light = normalize(2.0 * dot(vec, coords) * coords - vec);

        float nl_angle = max(light.z, 0.0);
        float nh_angle = max(coords.z, 0.0);
        float vh_angle = max(dot(vec, coords), 0.0);

        if(nl_angle > 0.0)
        {
            float g = GeometrySmith(n, vec, light, roughness);
            float g_vis = (g * vh_angle) / (nh_angle * angle_cos);
            float Fc = pow(1.0 - vh_angle, 5.0);

            scale += (1.0 - Fc) * g_vis;
            bias += Fc * g_vis;
        }
    }
    scale /= float(kSampleCount);
    bias /= float(kSampleCount);

    return vec2(scale, bias);
}