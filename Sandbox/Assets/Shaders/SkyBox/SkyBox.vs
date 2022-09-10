#version 460 core
layout(location = 0) in vec3 i_Pos;

out vec3 v_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    v_TexCoords = i_Pos;
    vec4 pos = (u_ViewProjection * u_Transform * vec4(i_Pos, 1.0)).xyww;
    pos.z *= 0.99999;
    gl_Position = pos;
}