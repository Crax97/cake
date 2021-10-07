#version 460 core

in vec2 fs_texcoord;
in vec3 fs_particle_color;

uniform sampler2D diffuse;

out vec4 color;

void main() {
    color = texture(diffuse, fs_texcoord) * vec4(fs_particle_color, 1.0);
    if(color.a < 0.2f) {
        discard;
    }
}