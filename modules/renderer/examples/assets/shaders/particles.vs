#version 460 core

in vec3 vertex;
in vec2 texcoord;
in vec3 normal;

layout(location = 10) in vec3 initial_velocity;
layout(location = 14) in vec3 color;
layout(location = 15) in float opacity;
in float lifetime;
in float initial_rotation;
in float rotation_rate;

out vec2 fs_texcoord;
out vec3 fs_particle_color;
uniform mat4 mvp;
uniform float time;
uniform vec3 gravity;

void main() {
  float t = lifetime;
  vec3 position = initial_velocity * t + 0.5 * gravity * t * t;
  float phi = initial_rotation + lifetime * rotation_rate;
  // Build mvp matrix in place
  mat4 rotation = mat4(cos(phi), -sin(phi), 0, 0, sin(phi), cos(phi), 0, 0, 0,
                       0, 1, 0, position.x, position.y, position.z, 1);
  gl_Position = mvp * rotation * vec4(vertex, 1.0);
  fs_texcoord = texcoord;
  fs_texcoord.y = 1.0f - fs_texcoord.y;
  fs_particle_color = color;
}