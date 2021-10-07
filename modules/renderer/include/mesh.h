#pragma once

#include <array>
#include <optional>
#include <vector>

namespace renderer {
#pragma pack(push, 1)
struct vertex {
  float x;
  float y;
  float z;

  vertex(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
  vertex() : vertex(0.0f, 0.0f, 0.0f){};
};
#pragma pack(pop)
using normal = vertex;

#pragma pack(push, 1)
struct tex_coord {
  float u;
  float v;
  tex_coord(float _u, float _v) noexcept : u(_u), v(_v) {}
  tex_coord() : tex_coord(0.0f, 0.0f) {}
};
#pragma pack(pop)

using vertices = std::vector<vertex>;
using normals = std::vector<normal>;
using tex_coords = std::vector<tex_coord>;
using indices = std::vector<unsigned int>;

struct face {
  std::array<vertex, 3> vertices{};
  std::optional<std::array<tex_coord, 3>> tex_coords;
  std::optional<std::array<normal, 3>> normals;
};

class mesh {
public:
  virtual void add_face(const face &face) noexcept = 0;
  virtual void draw() const noexcept = 0;
  virtual void bind() const noexcept = 0;
  virtual size_t element_count() const noexcept = 0;

  virtual ~mesh() noexcept = default;
};
}; // namespace renderer