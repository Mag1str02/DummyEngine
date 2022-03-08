#version 330 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexCords;

out vec3 normal;
out vec3 frag_pos;
out vec2 texture_cords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 rotation;

void main() {
    gl_Position = projection * view * model * vec4(iPos, 1.0);
    frag_pos = vec3(model * vec4(iPos, 1.0));
    normal = vec3(rotation * vec4(iNormal, 1.0));
    texture_cords = iTexCords;
}