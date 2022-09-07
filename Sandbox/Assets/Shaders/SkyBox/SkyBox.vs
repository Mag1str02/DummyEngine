#version 460 core
layout(location = 0) in vec3 i_Pos;

out vec3 v_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    v_TexCoords = i_Pos;
    gl_Position = (u_ViewProjection * u_Transform * vec4(i_Pos, 1.0));
}