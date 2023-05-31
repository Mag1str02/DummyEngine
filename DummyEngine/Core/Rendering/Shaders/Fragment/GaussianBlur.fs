#version 410 core
const int   kBlurWidth = 5;
const float kWeights[kBlurWidth] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
// const int   kBlurWidth = 12;
// const float kWeights[kBlurWidth] = float[](0.14, 0.13, 0.12, 0.11, 0.1, 0.09, 0.08, 0.07, 0.06, 0.05, 0.03, 0.02);
// const int   kBlurWidth = 20;
// const float kWeights[kBlurWidth] = float[] (0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05);

in vec2 v_TexCoords;
out vec4 f_FragColor;


uniform sampler2D u_Texture;
uniform int u_Horizontal;

void main()
{
    vec2 tex_offset = 1.0 / textureSize(u_Texture, 0);
    vec3 result = texture(u_Texture, v_TexCoords).rgb * kWeights[0];
    if(u_Horizontal > 0)
    {
        for(int i = 1; i < kBlurWidth; ++i)
        {
            result += texture(u_Texture, v_TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * kWeights[i];
            result += texture(u_Texture, v_TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * kWeights[i];
        }
    }
    else
    {
        for(int i = 1; i < kBlurWidth; ++i)
        {
            result += texture(u_Texture, v_TexCoords + vec2(0.0, tex_offset.y * i)).rgb * kWeights[i];
            result += texture(u_Texture, v_TexCoords - vec2(0.0, tex_offset.y * i)).rgb * kWeights[i];
        }
    }
    f_FragColor = vec4(result, 1.0);
}