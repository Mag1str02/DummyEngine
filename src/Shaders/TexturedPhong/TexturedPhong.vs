#version 450 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_TexCoords;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(i_Pos, 1.0);
    v_FragPos = vec3(u_Transform * vec4(i_Pos, 1.0));
    v_Normal = mat3(transpose(inverse(u_Transform))) * i_Normal;
    v_TexCoords = i_TexCoords;
}