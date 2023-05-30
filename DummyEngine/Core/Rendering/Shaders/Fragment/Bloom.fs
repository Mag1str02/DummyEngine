#version 410 core
const float kGamma = 3;

in vec2 v_TexCoords;
layout (location = 0) out vec4 f_FragColor;
layout (location = 1) out vec4 f_Filter;
layout (location = 2) out vec4 f_Raw;

uniform sampler2D u_Texture;
uniform sampler2D u_BrighnessTexture;
uniform float u_Exposure;


void main()
{
    vec3 hdrColor = texture(u_Texture, v_TexCoords).rgb;
    vec3 bloomColor = texture(u_BrighnessTexture, v_TexCoords).rgb;
    bloomColor *= 0.3;
    f_Filter = vec4(bloomColor, 1.0);
    f_Raw = vec4(hdrColor, 1.0);
    hdrColor += bloomColor;
    // hdrColor = pow(hdrColor, vec3(1.0 / kGamma));
    f_FragColor = vec4(hdrColor, 1.0);
}