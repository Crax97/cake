#version 460 core

in vec2 fs_texcoord;

uniform sampler2D diffuse;

out vec4 color;

void main() {
    color = texture(diffuse, fs_texcoord);
    if(color.a < 0.2f) {
        discard;
    }
}