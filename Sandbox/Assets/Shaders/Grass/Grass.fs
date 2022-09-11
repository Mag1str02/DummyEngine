#version 460 core

out vec4 f_FragColor;
in vec3 g_Color;

void main() { f_FragColor = vec4(g_Color, 1.0); }