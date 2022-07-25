#version 450 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;

out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

void main() {
    //  gl_Position = vec4(iPos, 1.0);
    gl_Position = u_ViewProjection * u_Transform * vec4(i_Pos, 1.0);
    v_FragPos = vec3(u_Transform * vec4(i_Pos, 1.0));
    v_Normal = mat3(transpose(inverse(u_Transform))) * i_Normal;
}