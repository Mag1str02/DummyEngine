#version 460 core
layout(location = 0) in vec3 i_Pos;
out vec3 v_TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

void main()
{
    mat4 view = mat4(mat3(u_View));
    v_TexCoords = i_Pos;
    gl_Position =  u_Projection * view * u_Transform * vec4(i_Pos, 1.0);
}