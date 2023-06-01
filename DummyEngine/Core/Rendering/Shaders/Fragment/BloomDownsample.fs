#version 410 core

uniform sampler2D u_Texture;

in vec2 v_TexCoords;
out vec4 f_FragColor;

void main()
{
    vec2 srcTexelSize = 1.0 / vec2(textureSize(u_Texture, 0));
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    vec3 a = texture(u_Texture, vec2(v_TexCoords.x - 2*x, v_TexCoords.y + 2*y)).rgb;
    vec3 b = texture(u_Texture, vec2(v_TexCoords.x,       v_TexCoords.y + 2*y)).rgb;
    vec3 c = texture(u_Texture, vec2(v_TexCoords.x + 2*x, v_TexCoords.y + 2*y)).rgb;

    vec3 d = texture(u_Texture, vec2(v_TexCoords.x - 2*x, v_TexCoords.y)).rgb;
    vec3 e = texture(u_Texture, vec2(v_TexCoords.x,       v_TexCoords.y)).rgb;
    vec3 f = texture(u_Texture, vec2(v_TexCoords.x + 2*x, v_TexCoords.y)).rgb;

    vec3 g = texture(u_Texture, vec2(v_TexCoords.x - 2*x, v_TexCoords.y - 2*y)).rgb;
    vec3 h = texture(u_Texture, vec2(v_TexCoords.x,       v_TexCoords.y - 2*y)).rgb;
    vec3 i = texture(u_Texture, vec2(v_TexCoords.x + 2*x, v_TexCoords.y - 2*y)).rgb;

    vec3 j = texture(u_Texture, vec2(v_TexCoords.x - x, v_TexCoords.y + y)).rgb;
    vec3 k = texture(u_Texture, vec2(v_TexCoords.x + x, v_TexCoords.y + y)).rgb;
    vec3 l = texture(u_Texture, vec2(v_TexCoords.x - x, v_TexCoords.y - y)).rgb;
    vec3 m = texture(u_Texture, vec2(v_TexCoords.x + x, v_TexCoords.y - y)).rgb;

    vec3 color = vec3(0);
    color += e*0.125;
    color += (a+c+g+i)*0.03125;
    color += (b+d+f+h)*0.0625;
    color += (j+k+l+m)*0.125;
    f_FragColor = vec4(color, 1.0);
}