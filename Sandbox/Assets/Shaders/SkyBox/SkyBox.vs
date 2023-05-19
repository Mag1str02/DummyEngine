#version 460 core

struct VP{
    mat4 view;
    mat4 projection;
};

layout(location = 0) in vec3 i_Pos;

out vec3 v_TexCoords;

uniform VP u_Camera;
uniform mat4 u_Transform;

void main()
{
    mat4 view = u_Camera.view;
    view[3][0]  = 0;
    view[3][1]  = 0;
    view[3][2]  = 0;
    v_TexCoords = i_Pos;
    gl_Position = u_Camera.projection * view * u_Transform * vec4(i_Pos, 1.0);
}