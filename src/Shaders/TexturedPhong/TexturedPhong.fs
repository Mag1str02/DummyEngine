#version 450 core

#define MAX_POINT_LIGHT_AMOUNT 64
#define MAX_DIRECTIONAL_LIGHT_AMOUNT 16
#define MAX_SPOT_LIGHT_AMOUNT 64

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
};
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct DirectionalLight {
    Light light;

    vec3 direction;
};
struct PointLight {
    Light light;

    vec3 position;
    vec3 clq;
};
struct SpotLight {
    PointLight point_light;

    vec3 direction;
    float inner_cone_cos;
    float outer_cone_cos;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 texture_cords;

out vec4 frag_color;

uniform vec3 view_pos;
uniform ivec3 light_amount;
uniform Material u_Material;
uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT_AMOUNT];
uniform SpotLight spot_lights[MAX_SPOT_LIGHT_AMOUNT];
uniform PointLight point_lights[MAX_POINT_LIGHT_AMOUNT];

vec3 DirectionalLightImpact(DirectionalLight direction_light, vec3 normal, vec3 view_direction) {
    vec3 normalized_light_ray = normalize(-direction_light.direction);

    vec3 ambient = direction_light.light.ambient * vec3(texture(u_Material.diffuse_map, texture_cords));

    float bounce_angle_cos = max(dot(normal, normalized_light_ray), 0.0);
    vec3 diffuse = bounce_angle_cos * direction_light.light.diffuse * vec3(texture(u_Material.diffuse_map, texture_cords));

    vec3 reflected_ray = reflect(-normalized_light_ray, normal);
    float spec = pow(max(dot(view_direction, reflected_ray), 0.0), 32);
    vec3 specular = spec * direction_light.light.specular * vec3(texture(u_Material.specular_map, texture_cords));

    return ambient + diffuse + specular;
}
vec3 PointLightImpact(PointLight point_light, vec3 normal, vec3 view_direction, vec3 frag_pos) {
    if (point_light.clq.x <= 0.0) {
        return vec3(0.0);
    }

    vec3 light_ray = point_light.position - frag_pos;
    vec3 normalized_light_ray = normalize(light_ray);
    vec3 reflected_ray = reflect(-normalized_light_ray, normal);

    float dist = length(light_ray);
    float bounce_angle_cos = max(dot(normal, normalized_light_ray), 0.0);
    float spec = pow(max(dot(view_direction, reflected_ray), 0.0), 32);

    float attenuation = 1.0 / (point_light.clq.x + point_light.clq.y * dist + point_light.clq.z * (dist * dist));

    vec3 ambient = point_light.light.ambient * vec3(texture(u_Material.diffuse_map, texture_cords));
    vec3 diffuse = bounce_angle_cos * point_light.light.diffuse * vec3(texture(u_Material.diffuse_map, texture_cords));
    vec3 specular = spec * point_light.light.specular * vec3(texture(u_Material.specular_map, texture_cords));

    return (ambient + diffuse + specular) * attenuation;
}
vec3 SpotLightImpact(SpotLight spot_light, vec3 normal, vec3 view_direction, vec3 frag_pos) {
    vec3 point_light_result = PointLightImpact(spot_light.point_light, normal, view_direction, frag_pos);

    vec3 light_ray = spot_light.point_light.position - frag_pos;
    vec3 normalized_light_ray = normalize(light_ray);

    float angle_coef = 0.0;
    float light_angle_cos = dot(-normalized_light_ray, normalize(spot_light.direction));
    if (spot_light.outer_cone_cos < light_angle_cos) {
        angle_coef = (light_angle_cos - spot_light.outer_cone_cos) / (spot_light.inner_cone_cos - spot_light.outer_cone_cos);
    }
    if (spot_light.inner_cone_cos < light_angle_cos) {
        angle_coef = 1.0;
    }

    return point_light_result * angle_coef;
}

void main() {
    vec3 normalized_normal = normalize(normal);
    vec3 view_direction = normalize(view_pos - frag_pos);

    vec3 result = vec3(0.0);

    for (int i = 0; i < light_amount.x; ++i) {
        result += DirectionalLightImpact(directional_lights[i], normalized_normal, view_direction);
    }
    for (int i = 0; i < light_amount.y; ++i) {
        result += PointLightImpact(point_lights[i], normalized_normal, view_direction, frag_pos);
    }
    for (int i = 0; i < light_amount.z; ++i) {
        result += SpotLightImpact(spot_lights[i], normalized_normal, view_direction, frag_pos);
    }

    frag_color = vec4(result, 1.0);
}