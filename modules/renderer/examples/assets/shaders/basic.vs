#version 460 core

in vec3 vertex;
in vec2 texcoord;
in vec3 normal;

out vec2 fs_texcoord;
layout(std140) uniform GlobalData {
    mat4 mvp;
};

void main() {
    gl_Position = mvp * vec4(vertex, 1.0);
    fs_texcoord = texcoord;
    fs_texcoord.y = 1.0f - fs_texcoord.y;
}