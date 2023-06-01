#version 410 core

in vec2 v_TexCoords;
out vec4 f_FragColor;

uniform sampler2D u_Texture;
uniform float u_Exposure;
uniform float u_Gamma;


void main()
{
    vec3 color = texture(u_Texture, v_TexCoords).rgb;
    vec3 mapped_color = vec3(1.0) - exp(-color * u_Exposure);
    mapped_color = pow(mapped_color, vec3(1.0 / u_Gamma));
    f_FragColor = vec4(mapped_color, 1.0);
}