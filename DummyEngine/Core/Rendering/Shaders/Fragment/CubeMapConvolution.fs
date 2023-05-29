#version 410 core

const float k_SampleDelta = 0.050;
const float kPI = 3.1415926;

out vec4 f_FragColor;
in vec3 v_TexCoords;

uniform samplerCube u_CubeMap;

void main()
{		
    vec3 normal = normalize(v_TexCoords);
    vec3 irradiance = vec3(1.0, 0.0, 1.0);
  
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up         = normalize(cross(normal, right));

    float samples_amount = 0.0; 
    for(float phi = 0.0; phi < 2.0 * kPI; phi += k_SampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * kPI; theta += k_SampleDelta)
        {
            vec3 tangent_sample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 vec_sample = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * normal; 

            irradiance += texture(u_CubeMap, vec_sample).rgb * cos(theta) * sin(theta);
            ++samples_amount;
        }
    }
    irradiance = kPI * irradiance * (1.0 / float(samples_amount));
    f_FragColor = vec4(irradiance, 1.0);
}