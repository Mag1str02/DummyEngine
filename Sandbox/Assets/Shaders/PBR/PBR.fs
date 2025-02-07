#version 410 core

const float kMaxPrefilterLOD = 9;
const float kPI = 3.1415926;
const vec3  kDefaultReflectivity = vec3(0.04);

#define MAX_LIGHT_SOURCES 128

struct MaterialPrecalc{
    float metallic;
    float roughness;
    float ao;
    vec3 albedo;
    vec3 base_reflectivity;
    vec3 n_view;
    vec3 n_normal;
};

struct Material
{   
    vec3      m_Ambient;
    vec3      m_Albedo;
    vec3      m_ORM;
    vec3      m_Emission;
    float     m_Shininess;
    float     m_EmissionStrength;
    sampler2D m_AlbedoMap;
    sampler2D m_NormalMap;
    sampler2D m_ORMMap;
    sampler2D m_EmissionMap;
};

struct LightSource
{
    vec3 m_Ambient;
    vec3 m_Diffuse;
    vec3 m_Specular;
    vec3 m_Direction;
    vec3 m_Position;
    vec3 m_CLQ;
    vec3 m_ConesAndType;
};

layout(std140) uniform ub_Lights { LightSource lights[100]; };

in VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPos;
    vec2 TexCoords;
    mat3 TBNMatrix;
    vec4 ShadowMapPos;
}
vs_in;
out vec4 f_FragColor;

uniform int         u_LightAmount;
uniform Material    u_Material;
uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D   u_BRDF;
uniform int         u_UseShadowMap;
uniform sampler2D   u_ShadowMap;
uniform float       u_ShadowFarPlane;
uniform mat4        u_EnvRotation;
uniform int         u_UsePointShadowMap;
uniform samplerCube u_PointShadowMap[10];

float ShadowImpact(vec4 pos);
vec3  FresnelSchlick(float angle_cos, vec3 base_reflectivity);
vec3  FresnelSchlickWithRoughness(float angle_cos, vec3 base_reflectivity, float roughness);
float DistributionGGX(vec3 n_normal, vec3 n_halfway, float roughness);
float GeometrySchlickGGX(float angle_cos, float roughness);
float GeometrySmith(vec3 n_normal, vec3 n_view, vec3 n_light, float roughness);
vec3  CalcLightImpact(LightSource source, MaterialPrecalc mp);
int shadowMapIdx = 0;

void main()
{
    vec3 result = vec3(0.0);
    MaterialPrecalc mp;

    mp.albedo = vec3(texture(u_Material.m_AlbedoMap, vs_in.TexCoords)) * u_Material.m_Albedo;
    mp.ao =  (1 - u_Material.m_ORM.x) + vec3(texture(u_Material.m_ORMMap, vs_in.TexCoords)).x * u_Material.m_ORM.x;
    mp.roughness = vec3(texture(u_Material.m_ORMMap, vs_in.TexCoords)).y * u_Material.m_ORM.y;
    mp.metallic =  vec3(texture(u_Material.m_ORMMap, vs_in.TexCoords)).z * u_Material.m_ORM.z;
    mp.base_reflectivity = mix(kDefaultReflectivity,  mp.albedo, mp.metallic); 
    mp.n_view  = normalize(vs_in.CameraPos - vs_in.FragPos);
    mp.n_normal = texture(u_Material.m_NormalMap, vs_in.TexCoords).rgb;
    mp.n_normal = mp.n_normal * 2.0 - 1.0;   
    mp.n_normal = normalize(vs_in.TBNMatrix * mp.n_normal);

    float shadow = 0;
    if (u_UseShadowMap > 0) {
        shadow = ShadowImpact(vs_in.ShadowMapPos) * 0.4;
    }
    shadowMapIdx = 0;
    for (int i = 0; i < u_LightAmount; ++i){
        vec3 impact = CalcLightImpact(lights[i], mp);
        if (lights[i].m_ConesAndType.z == 1) {
            impact = impact * (1.0 - shadow);
        }
        result += impact;
    }
    vec3 env_n = mat3(u_EnvRotation) * mp.n_normal;
    vec3 ray = mat3(u_EnvRotation) * normalize(reflect(-mp.n_view, mp.n_normal));
    vec3 kS = FresnelSchlickWithRoughness(max(dot(mp.n_normal, mp.n_view), 0.0), mp.base_reflectivity, mp.roughness);
    vec3 kD = (1.0 - kS) * (1.0 - mp.metallic);
    vec3 irradiance = texture(u_IrradianceMap, env_n).rgb;
    vec3 diffuse    = irradiance * mp.albedo;

    vec3 prefiltered_color = textureLod(u_PrefilterMap, ray,  mp.roughness * kMaxPrefilterLOD).rgb;  
    vec2 brdf  = texture(u_BRDF, vec2(max(dot(mp.n_normal, mp.n_view), 0.0), mp.roughness)).rg; 
    vec3 specular = prefiltered_color * (kS * brdf.x + brdf.y);

    vec3 ambient = (specular + kD * diffuse) * mp.ao;
    result += u_Material.m_Ambient * ambient; 
    result += texture(u_Material.m_EmissionMap, vs_in.TexCoords).rgb * u_Material.m_EmissionStrength * u_Material.m_Emission;

    // vec3 a = vec3()
    // result = result / (result + vec3(0.2));
    // result = pow(result, vec3(3));  
    // result *= vec3(1.728); 
    // result = result / (result + vec3(1));
    // result = pow(result, vec3(0.9));  
    // result *= vec3(1.5);

    f_FragColor = vec4(result, 1.0);
}

float ShadowImpact(vec4 pos) {
    vec3 projCoords = pos.xyz / pos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float depth = texture(u_ShadowMap, projCoords.xy).r;
    if (projCoords.z > 1.0) {
        return 0.0;
    }
    if (projCoords.z > depth + 0.001) {
        return 1.0;
    } else {
        return 0.0;
    }
}

vec3 CalcLightImpact(LightSource source, MaterialPrecalc mp){
    float attenuation;
    vec3 n_light;
    if(source.m_ConesAndType.z == 1){
        attenuation = 1;
        n_light = normalize(source.m_Direction);
    }else{ 
        float dist = length(source.m_Position - vs_in.FragPos);
        attenuation =  1.0 / (source.m_CLQ.x + source.m_CLQ.y * dist + source.m_CLQ.z * (dist * dist));
        n_light = normalize(source.m_Position - vs_in.FragPos);
    }
    if(u_UsePointShadowMap > 0 && source.m_ConesAndType.z == 2) {
        vec3 fragToLight = vs_in.FragPos - source.m_Position;
        float depth = texture(u_PointShadowMap[shadowMapIdx], fragToLight).r * u_ShadowFarPlane;
        float realDepth = length(fragToLight);
        shadowMapIdx = shadowMapIdx + 1;
        if (realDepth > depth + 0.001) {
            return vec3(0.0);
        }
    }
    vec3 n_halfway = normalize(n_light + mp.n_view);
    vec3 radiance = source.m_Diffuse * attenuation;
    float D = DistributionGGX(mp.n_normal, n_halfway, mp.roughness);
    float G = GeometrySmith(mp.n_normal, mp.n_view, n_light, mp.roughness);
    vec3 k_s =  FresnelSchlick(dot(n_halfway, mp.n_view), mp.base_reflectivity);
    vec3 k_d =  (vec3(1.0) - k_s) * (1.0 - mp.metallic);
    float nl_angle = max(dot(mp.n_normal, n_light), 0.0);
    float nv_angle = max(dot(mp.n_normal, mp.n_view), 0.0);
    
    vec3 specular = k_s * G * D/ (4.0 * nl_angle * nv_angle + 0.0001);
    vec3 diffuse = k_d * mp.albedo / kPI;
    vec3 L = (diffuse + specular) * radiance * nl_angle;

    return L;
}
vec3 FresnelSchlick(float angle_cos, vec3 base_reflectivity)
{
    return base_reflectivity + (1.0 - base_reflectivity) * pow(clamp(1.0 - angle_cos, 0.0, 1.0), 5.0);
}
vec3 FresnelSchlickWithRoughness(float angle_cos, vec3 base_reflectivity, float roughness)
{
    return base_reflectivity + (max(vec3(1.0 - roughness), base_reflectivity) - base_reflectivity) * pow(clamp(1.0 - angle_cos, 0.0, 1.0), 5.0);
}  
float DistributionGGX(vec3 n_normal, vec3 n_halfway, float roughness)
{
    float a      = roughness*roughness;
    float cos_angle  = max(dot(n_normal, n_halfway), 0.0);
    a = a*a;
    cos_angle = cos_angle * cos_angle;
	
    float t = cos_angle * (a - 1.0) + 1.0;
    return a / (t * t * kPI);
}
float GeometrySchlickGGX(float angle_cos, float roughness)
{
    float a = (roughness + 1.0);
    float k = a * a / 8.0;

    return angle_cos / (angle_cos * (1.0 - k) + k);
}
float GeometrySmith(vec3 n_normal, vec3 n_view, vec3 n_light, float roughness)
{	
    float nv_angle = max(dot(n_normal, n_view), 0.0);
    float nl_angle = max(dot(n_normal, n_light), 0.0);
    return GeometrySchlickGGX(nv_angle, roughness) * GeometrySchlickGGX(nl_angle, roughness);
}