#version 410 core

in vec2 v_TexCoords;
out vec4 f_FragColor;

uniform sampler2D u_Texture;

void main()
{
    f_FragColor = vec4(vec3(texture(u_Texture, v_TexCoords)), 1.0);
}