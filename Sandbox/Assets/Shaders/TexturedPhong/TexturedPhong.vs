#version 410 core

struct VP{
    mat4 view;
    mat4 projection;
};

layout(location = 0) in vec3  i_Pos;
layout(location = 1) in vec3  i_Normal;
layout(location = 2) in vec3  i_Tangent;
layout(location = 3) in vec2  i_TexCoords;
layout(location = 4) in ivec4 i_BoneIDs1;
layout(location = 5) in ivec4 i_BoneIDs2;
layout(location = 6) in vec4  i_BoneWeights1;
layout(location = 7) in vec4  i_BoneWeights2;
layout(location = 8) in mat4  i_Transform;

out VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPos;
    vec2 TexCoords;
}
vs_out;

uniform VP u_Camera;

void main()
{
    mat4 view = -inverse( u_Camera.view) ;
    gl_Position      = u_Camera.projection * u_Camera.view * i_Transform * vec4(i_Pos, 1.0);
    vs_out.FragPos   = vec3(i_Transform * vec4(i_Pos, 1.0));
    vs_out.Normal    = mat3(transpose(inverse(i_Transform))) * i_Normal;
    vs_out.TexCoords = i_TexCoords;
    vs_out.CameraPos = -vec3(view[3][0], view[3][1], view[3][2]);
}