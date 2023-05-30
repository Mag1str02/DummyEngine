#version 410 core

in vec2 v_TexCoords;

layout (location = 0) out vec4 f_FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D u_Texture;
uniform float u_BrightnessTreshold;

void main()
{
    f_FragColor = vec4(vec3(texture(u_Texture, v_TexCoords)), 1.0);
    float brightness = dot(f_FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > u_BrightnessTreshold)
        BrightColor = vec4(f_FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}