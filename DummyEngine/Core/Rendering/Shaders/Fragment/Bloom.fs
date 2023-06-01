#version 410 core

in vec2 v_TexCoords;
out vec4 f_FragColor;

uniform float     u_Strength;
uniform sampler2D u_Texture;
uniform sampler2D u_BrighnessTexture;

void main()
{
    vec3 color = texture(u_Texture, v_TexCoords).rgb;
    vec3 bloom = texture(u_BrighnessTexture, v_TexCoords).rgb;
    vec3 with_bloom = bloom + color;
    f_FragColor = vec4(mix(color, with_bloom, u_Strength), 1.0);
}