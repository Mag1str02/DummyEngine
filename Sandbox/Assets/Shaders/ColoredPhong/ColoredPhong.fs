#version 450 core

#define MAX_LIGHT_SOURCES 64

struct Material
{
    vec3 diffuse_color;
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

in vec3 v_FragPos;
in vec3 v_Normal;

out vec4 f_FragColor;

uniform vec3 u_CameraPos;
uniform int u_LightAmount;
uniform Material u_Material;
uniform LightSource u_LightSources[MAX_LIGHT_SOURCES];

vec3 DirectionalLightImpact(LightSource direction_light, vec3 v_Normal, vec3 view_direction)
{
    vec3 normalized_light_ray = normalize(-direction_light.m_Direction);

    vec3 ambient = direction_light.m_Ambient * u_Material.diffuse_color;

    float bounce_angle_cos = max(dot(v_Normal, normalized_light_ray), 0.0);
    vec3 diffuse = bounce_angle_cos * direction_light.m_Diffuse * u_Material.diffuse_color;

    vec3 reflected_ray = reflect(-normalized_light_ray, v_Normal);
    float spec = pow(max(dot(view_direction, reflected_ray), 0.0), 32);
    vec3 specular = spec * direction_light.m_Specular * u_Material.diffuse_color;

    return ambient + diffuse + specular;
}
vec3 PointLightImpact(LightSource point_light, vec3 v_Normal, vec3 view_direction, vec3 v_FragPos)
{
    vec3 light_ray = point_light.m_Position - v_FragPos;
    vec3 normalized_light_ray = normalize(light_ray);
    vec3 reflected_ray = reflect(-normalized_light_ray, v_Normal);

    float dist = length(light_ray);
    float bounce_angle_cos = max(dot(v_Normal, normalized_light_ray), 0.0);
    float spec = pow(max(dot(view_direction, reflected_ray), 0.0), 32);

    float attenuation = 1.0 / (point_light.m_CLQ.x + point_light.m_CLQ.y * dist + point_light.m_CLQ.z * (dist * dist));

    vec3 ambient = point_light.m_Ambient * u_Material.diffuse_color;
    vec3 diffuse = bounce_angle_cos * point_light.m_Diffuse * u_Material.diffuse_color;
    vec3 specular = spec * point_light.m_Specular * u_Material.diffuse_color;

    return (ambient + diffuse + specular) * attenuation;
}
vec3 SpotLightImpact(LightSource spot_light, vec3 v_Normal, vec3 view_direction, vec3 v_FragPos)
{
    vec3 point_light_result = PointLightImpact(spot_light, v_Normal, view_direction, v_FragPos);

    vec3 light_ray = spot_light.m_Position - v_FragPos;
    vec3 normalized_light_ray = normalize(light_ray);

    float angle_coef = 0.0;
    float light_angle_cos = dot(-normalized_light_ray, normalize(spot_light.m_Direction));
    if (spot_light.m_ConesAndType.x < light_angle_cos)
    {
        angle_coef = (light_angle_cos - spot_light.m_ConesAndType.x) /
                     (spot_light.m_ConesAndType.y - spot_light.m_ConesAndType.x);
    }
    if (spot_light.m_ConesAndType.y < light_angle_cos)
    {
        angle_coef = 1.0;
    }

    return point_light_result * angle_coef;
}

void main()
{
    vec3 normalized_normal = normalize(v_Normal);
    vec3 view_direction = normalize(u_CameraPos - v_FragPos);

    vec3 result = vec3(0.0);

    for (int i = 0; i < u_LightAmount; ++i)
    {
        if (u_LightSources[i].m_ConesAndType.z == 1)
            result += DirectionalLightImpact(u_LightSources[i], normalized_normal, view_direction);
        if (u_LightSources[i].m_ConesAndType.z == 2)
            result += PointLightImpact(u_LightSources[i], normalized_normal, view_direction, v_FragPos);
        if (u_LightSources[i].m_ConesAndType.z == 3)
            result += SpotLightImpact(u_LightSources[i], normalized_normal, view_direction, v_FragPos);
    }

    f_FragColor = vec4(result, 1.0);
}