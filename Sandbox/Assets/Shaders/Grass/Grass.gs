#version 410 core
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

const float GRASS_WIDTH  = 0.5;
const float GRASS_HEIGHT = 5;

in vec3 v_Up[];
in vec3 v_Right[];

out vec3 g_Color;

struct VP
{
    mat4 view;
    mat4 projection;
};
layout(std140) uniform ub_VP { VP vp[32]; };
uniform int   u_VP;
uniform float u_Time;

float random(in vec2 st) { return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123); }

float noise(in vec2 st)
{
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    // float sin_value = sin(u_Time + gl_in[0].gl_Position.x);
    float x_offset          = noise(gl_in[0].gl_Position.xz / 10 + vec2(u_Time / 2)) * 2 - 1;
    float z_offset          = noise(-gl_in[0].gl_Position.xz / 10 + vec2(u_Time / 2, u_Time / 2)) * 2 - 1;
    float height_multiplier = noise(-gl_in[0].gl_Position.xz / 10) + 0.5;
    vec3  offset            = vec3(x_offset, 0, z_offset) * 2;
    // vec3 offset = vec3(0);

    g_Color     = vec3(0.2, 0.54, 0.2);
    gl_Position = vp[u_VP].projection * vp[u_VP].view * (gl_in[0].gl_Position + vec4(v_Right[0] * GRASS_WIDTH / 2, 0.0));
    EmitVertex();

    g_Color     = vec3(0.2, 0.54, 0.2);
    gl_Position = vp[u_VP].projection * vp[u_VP].view * (gl_in[0].gl_Position - vec4(v_Right[0] * GRASS_WIDTH / 2, 0.0));
    EmitVertex();

    g_Color = vec3(0.2, 0.7, 0.2);
    gl_Position =
        vp[u_VP].projection * vp[u_VP].view * (gl_in[0].gl_Position + vec4(v_Right[0] * GRASS_WIDTH / 3, 0.0) + vec4(v_Up[0] * GRASS_HEIGHT / 2 * height_multiplier, 0.0) + vec4(offset, 0.0) * 0.3);
    EmitVertex();

    g_Color = vec3(0.2, 0.7, 0.2);
    gl_Position =
        vp[u_VP].projection * vp[u_VP].view * (gl_in[0].gl_Position - vec4(v_Right[0] * GRASS_WIDTH / 3, 0.0) + vec4(v_Up[0] * GRASS_HEIGHT / 2 * height_multiplier, 0.0) + vec4(offset, 0.0) * 0.3);
    EmitVertex();

    g_Color     = vec3(0.2, 1.0, 0.2);
    gl_Position = vp[u_VP].projection * vp[u_VP].view * (gl_in[0].gl_Position + vec4(v_Up[0] * GRASS_HEIGHT * height_multiplier, 0.0) + vec4(offset, 0.0));
    EmitVertex();

    EndPrimitive();
}