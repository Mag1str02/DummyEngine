#version 410 core

out vec4 f_FragColor;
in vec3 v_TexCoords;

uniform samplerCube u_SkyBox;

void main()
{
    f_FragColor = texture(u_SkyBox, v_TexCoords);
}