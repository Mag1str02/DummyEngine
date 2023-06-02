#version 410 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Up;
layout(location = 2) in vec3 i_Right;

out vec3 v_Up;
out vec3 v_Right;

void main()
{
    gl_Position = vec4(i_Pos, 1.0);
    v_Up        = i_Up;
    v_Right     = i_Right;
}