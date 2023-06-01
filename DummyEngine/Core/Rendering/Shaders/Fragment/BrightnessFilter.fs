#version 410 core

in vec2 v_TexCoords;
out vec4 f_FragColor;

uniform sampler2D u_Texture;
uniform float u_Treshold;
uniform float u_SoftTreshold;

void main()
{
    vec3 color = texture(u_Texture, v_TexCoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > u_SoftTreshold) {
        float mul = min((brightness - u_SoftTreshold) / (u_Treshold - u_SoftTreshold + 0.000000001),1);
        f_FragColor = vec4(color * mul, 1.0);
    }
    else {
        f_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}