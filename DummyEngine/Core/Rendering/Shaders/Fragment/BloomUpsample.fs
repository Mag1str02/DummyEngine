#version 410 core

uniform sampler2D u_Texture;
uniform float u_Radius;

in vec2 v_TexCoords;
out vec4 f_FragColor;

void main()
{
    float x = u_Radius;
    float y = u_Radius;

    vec3 a = texture(u_Texture, vec2(v_TexCoords.x - x, v_TexCoords.y + y)).rgb;
    vec3 b = texture(u_Texture, vec2(v_TexCoords.x,     v_TexCoords.y + y)).rgb;
    vec3 c = texture(u_Texture, vec2(v_TexCoords.x + x, v_TexCoords.y + y)).rgb;

    vec3 d = texture(u_Texture, vec2(v_TexCoords.x - x, v_TexCoords.y)).rgb;
    vec3 e = texture(u_Texture, vec2(v_TexCoords.x,     v_TexCoords.y)).rgb;
    vec3 f = texture(u_Texture, vec2(v_TexCoords.x + x, v_TexCoords.y)).rgb;

    vec3 g = texture(u_Texture, vec2(v_TexCoords.x - x, v_TexCoords.y - y)).rgb;
    vec3 h = texture(u_Texture, vec2(v_TexCoords.x,     v_TexCoords.y - y)).rgb;
    vec3 i = texture(u_Texture, vec2(v_TexCoords.x + x, v_TexCoords.y - y)).rgb;

    vec3 color = e*4.0;
    color += (b+d+f+h)*2.0;
    color += (a+c+g+i);
    color *= 1.0 / 16.0;
    f_FragColor = vec4(color, 1.0);
}