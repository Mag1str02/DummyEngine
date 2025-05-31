#version 410 core

#define MAX_LIGHT_SOURCES 128

struct Material
{
    vec3      m_Ambient;
    vec3      m_Diffuse;
    vec3      m_Specular;
    vec3      m_Emission;
    float     m_EmissionStrength;
    float     m_Shininess;
    sampler2D m_DiffuseMap;
    sampler2D m_SpecularMap;
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
    int m_UseShadowMap;
};

in VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPos;
    vec2 TexCoords;
}
vs_in;

out vec4 f_FragColor;

uniform int      u_LightAmount;
uniform int         u_UseShadowMap;
uniform sampler2D   u_ShadowMap;
uniform float       u_ShadowFarPlane;
uniform Material u_Material;

layout(std140) uniform ub_Lights { LightSource lights[100]; };

vec3 DirectionalLightImpact(LightSource direction_light, vec3 v_Normal, vec3 view_direction)
{
    vec3 light_ray = normalize(-direction_light.m_Direction);
    vec3 halfway_ray          = normalize(light_ray + view_direction);

    float bounce_angle_cos = max(dot(v_Normal, light_ray), 0.0);
    float spec             = pow(max(dot(v_Normal, halfway_ray), 0.0), u_Material.m_Shininess);

    vec3 diffuse  = bounce_angle_cos * direction_light.m_Diffuse * vec3(texture(u_Material.m_DiffuseMap, vs_in.TexCoords)) * u_Material.m_Diffuse;
    vec3 specular = spec * direction_light.m_Specular * vec3(texture(u_Material.m_SpecularMap, vs_in.TexCoords)) * u_Material.m_Specular;

    return  diffuse + specular;
}
vec3 PointLightImpact(LightSource point_light, vec3 v_Normal, vec3 view_direction, vec3 v_FragPos)
{
    vec3 light_ray            = normalize(point_light.m_Position - v_FragPos);
    vec3 halfway_ray          = normalize(light_ray + view_direction);

    float dist             = length(light_ray);
    float bounce_angle_cos = max(dot(v_Normal, light_ray), 0.0);
    float spec             = pow(max(dot(v_Normal, halfway_ray), 0.0), u_Material.m_Shininess);

    float attenuation = 1.0 / (point_light.m_CLQ.x + point_light.m_CLQ.y * dist + point_light.m_CLQ.z * (dist * dist));

    vec3 diffuse  = bounce_angle_cos * point_light.m_Diffuse * vec3(texture(u_Material.m_DiffuseMap, vs_in.TexCoords))* u_Material.m_Diffuse;
    vec3 specular = spec * point_light.m_Specular * vec3(texture(u_Material.m_SpecularMap, vs_in.TexCoords)) * u_Material.m_Specular;

    return ( diffuse + specular) * attenuation;
}
vec3 SpotLightImpact(LightSource spot_light, vec3 v_Normal, vec3 view_direction, vec3 v_FragPos)
{
    vec3 point_light_result = PointLightImpact(spot_light, v_Normal, view_direction, v_FragPos);

    vec3 light_ray            = spot_light.m_Position - v_FragPos;
    vec3 normalized_light_ray = normalize(light_ray);

    float angle_coef      = 0.0;
    float light_angle_cos = dot(-normalized_light_ray, normalize(spot_light.m_Direction));
    if (spot_light.m_ConesAndType.x < light_angle_cos)
    {
        angle_coef = (light_angle_cos - spot_light.m_ConesAndType.x) / (spot_light.m_ConesAndType.y - spot_light.m_ConesAndType.x);
    }
    if (spot_light.m_ConesAndType.y < light_angle_cos)
    {
        angle_coef = 1.0;
    }

    return point_light_result * angle_coef;
}

void main()
{
    if (vec4(texture(u_Material.m_DiffuseMap, vs_in.TexCoords)).a < 0.000000001) discard;

    vec3 normalized_normal = normalize(vs_in.Normal);
    vec3 view_direction    = normalize(vs_in.CameraPos - vs_in.FragPos);

    vec3 result = vec3(0.0);

    for (int i = 0; i < u_LightAmount; ++i)
    {
        if (lights[i].m_ConesAndType.z == 1) result += DirectionalLightImpact(lights[i], normalized_normal, view_direction);
        if (lights[i].m_ConesAndType.z == 2) result += PointLightImpact(lights[i], normalized_normal, view_direction, vs_in.FragPos);
        if (lights[i].m_ConesAndType.z == 3) result += SpotLightImpact(lights[i], normalized_normal, view_direction, vs_in.FragPos);
    }
    result += u_Material.m_Ambient * texture(u_Material.m_DiffuseMap, vs_in.TexCoords).rgb;
    result += u_Material.m_Emission * texture(u_Material.m_EmissionMap, vs_in.TexCoords).rgb * u_Material.m_EmissionStrength;

    f_FragColor = vec4(result, 1.0);
}