#version 410 core

out vec4 f_FragColor;
in vec3 v_TexCoords;

uniform samplerCube u_SkyBox;
uniform float u_SkyBoxLOD;

void main()
{
    f_FragColor = textureLod(u_SkyBox, v_TexCoords, u_SkyBoxLOD);
}