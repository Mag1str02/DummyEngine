#version 410 core

out vec2 v_TexCoords;

layout(location = 0) in vec2 i_Pos;
layout(location = 1) in vec2 i_Pos_L;

void main()
{
    v_TexCoords = i_Pos_L;
    gl_Position = vec4(i_Pos, 0.0, 1.0);
}