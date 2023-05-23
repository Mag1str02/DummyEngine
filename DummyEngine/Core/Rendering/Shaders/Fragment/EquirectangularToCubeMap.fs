#version 460 core

in vec3 v_TexCoords;
out vec4 f_FragColor;

uniform sampler2D u_EquirectangularMap;

const vec2 inv_atan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= inv_atan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(v_TexCoords));
    vec3 color = texture(u_EquirectangularMap, uv).rgb;
    
    f_FragColor = vec4(color, 1.0);
}