#version 410 core

in vec2 v_TexCoords;
out vec4 f_FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_BrighnessTexture;

void main()
{
    vec3 color = texture(u_Texture, v_TexCoords).rgb;
    vec3 bloom_color = texture(u_BrighnessTexture, v_TexCoords).rgb;
    // bloom_color *= 0.3;
    color += bloom_color;
    f_FragColor = vec4(color, 1.0);
}