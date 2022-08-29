
#version 460 core
layout (location = 0) in vec2 i_Pos;
layout (location = 1) in vec2 i_TexCoords;

out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(i_Pos.x, i_Pos.y, 0.0, 1.0); 
    v_TexCoords = i_TexCoords;
} 