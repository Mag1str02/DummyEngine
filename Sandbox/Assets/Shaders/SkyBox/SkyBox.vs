#version 460 core
layout(location = 0) in vec3 i_Pos;

out vec3 v_TexCoords;

struct VP{
    mat4 view;
    mat4 projection;
};
layout(std140) uniform ub_VP { VP vp[32]; };
uniform int u_VP;

uniform mat4 u_Transform;

void main()
{
    v_TexCoords = i_Pos;
    vec4 pos = (vp[u_VP].projection * vp[u_VP].view * u_Transform * vec4(i_Pos, 1.0)).xyww;
    pos.z *= 0.99999;
    gl_Position = pos;
}