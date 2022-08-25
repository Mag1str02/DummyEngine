#version 460 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_TexCoords;
layout(location = 3) in mat4 i_Transform;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform int u_Instanced;

void main()
{
    mat4 Transform;
    if(u_Instanced == 1){
        Transform = i_Transform;
    }else{
        Transform = u_Transform;
    }
    gl_Position = u_ViewProjection * Transform * vec4(i_Pos, 1.0);
    v_FragPos = vec3(Transform * vec4(i_Pos, 1.0));
    v_Normal = mat3(transpose(inverse(Transform))) * i_Normal;
    v_TexCoords = i_TexCoords;
}