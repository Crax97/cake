#version 460 core

in vec3 vertex;
in vec2 texcoord;
in vec3 normal;

layout(location = 10) in vec3 offset;

out vec2 fs_texcoord;
uniform mat4 mvp;
uniform float time;

void main() {
    float displacement = 0.5f;
    float displace_y = sin(time / 1000.0f) * 0.5f;
    float factor = 100.0f;
    vec3 displace_vec = vec3(
        sin(time + (gl_InstanceID * gl_VertexID / factor )) * displacement,
        displace_y,
        0.0f
    );

    gl_Position = mvp * vec4(vertex + offset + displace_vec, 1.0);
    fs_texcoord = texcoord;
    fs_texcoord.y = 1.0f - fs_texcoord.y;
}