#version 460 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_TexCoords;
layout(location = 3) in mat4 i_Transform;

out VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPos;
    vec2 TexCoords;
}
vs_out;

struct VP{
    mat4 view;
    mat4 projection;
};
layout(std140) uniform ub_VP { VP vp[32]; };
uniform int u_VP;

uniform mat4 u_Transform;
uniform int  u_Instanced;

void main()
{
    mat4 Transform;
    if (u_Instanced == 1)
    {
        Transform = i_Transform;
    }
    else
    {
        Transform = u_Transform;
    }
    mat4 view = -inverse( vp[u_VP].view) ;
    gl_Position      = vp[u_VP].projection * vp[u_VP].view * Transform * vec4(i_Pos, 1.0);
    vs_out.FragPos   = vec3(Transform * vec4(i_Pos, 1.0));
    vs_out.Normal    = mat3(transpose(inverse(Transform))) * i_Normal;
    vs_out.TexCoords = i_TexCoords;
    vs_out.CameraPos = -vec3(view[3][0],view[3][1],view[3][2]);
}