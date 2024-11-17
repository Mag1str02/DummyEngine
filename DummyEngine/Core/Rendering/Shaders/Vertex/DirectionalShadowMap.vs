#version 410 core

const int kMaxBones = 128;

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

uniform VP u_Camera;
uniform int u_Animated;
uniform mat4 u_BoneMatrices[kMaxBones];
uniform mat4 u_Transform;

void main()
{
    vec4 final_pos;
    vec3 anim_normal;
    if(u_Animated > 0){
        final_pos = vec4(0.0);
        for(int i = 0 ; i < 4; ++i)
        {
            if(i_BoneIDs1[i] == -1)
                continue;
            if(i_BoneIDs1[i] >= kMaxBones)
            {
                final_pos = vec4(0.0);
                break;
            }
            vec4 local_pos = u_BoneMatrices[i_BoneIDs1[i]] * vec4(i_Pos, 1.0);
            final_pos += local_pos * i_BoneWeights1[i];
            anim_normal = mat3(u_BoneMatrices[i_BoneIDs1[i]]) * i_Normal;
        }
        for(int i = 0 ; i < 4; ++i)
        {
            if(i_BoneIDs2[i] == -1)
                continue;
            if(i_BoneIDs2[i] >= kMaxBones)
            {
                final_pos = vec4(0.0);
                break;
            }
            vec4 local_pos = u_BoneMatrices[i_BoneIDs2[i]] * vec4(i_Pos, 1.0);
            final_pos += local_pos * i_BoneWeights2[i];
            anim_normal = mat3(u_BoneMatrices[i_BoneIDs2[i]]) * i_Normal;
        }
    }else{
        final_pos = vec4(i_Pos, 1.0);
        anim_normal = i_Normal;
    }

    gl_Position      = u_Camera.projection * u_Camera.view * u_Transform * final_pos;
}