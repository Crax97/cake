#version 460 core

in vec2 fs_texcoord;

uniform sampler2D diffuse;
uniform float time;

out vec4 color;

float zosin(float x) { return mod(sin(x), 1.0); }

void main() {
  vec4 tex_color = texture(diffuse, fs_texcoord);
  float t = time / 3.0;
  tex_color.r = zosin(t * tex_color.r);
  tex_color.g = zosin(t * tex_color.g);
  tex_color.b = zosin(t * tex_color.b);
  color = tex_color;
}