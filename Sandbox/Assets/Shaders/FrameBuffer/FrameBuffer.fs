#version 410 core

out vec4 f_FragColor;
  
in vec2 v_TexCoords;

uniform sampler2D u_ScreenTexture;

void main()
{ 
    f_FragColor = texture(u_ScreenTexture, v_TexCoords);
}