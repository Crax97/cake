#version 460 core

in vec3 vertex;
in vec2 texcoord;
in vec3 normal;

out vec2 fs_texcoord;

void main() {
  gl_Position = vec4(vertex, 1.0);
  fs_texcoord = texcoord;
}