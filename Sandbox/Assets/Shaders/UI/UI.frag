#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color = vec4(1.0);

out vec4 f_FragColor;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    f_FragColor = texColor * u_Color;
} 