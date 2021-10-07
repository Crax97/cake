#pragma once

#include "api_factory.h"
#include "graphics_api.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "OBJ_Loader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace utilities {
#pragma pack(push, 1)
struct application_data {
  glm::mat4 mvp;
};
#pragma pack(pop)

inline std::shared_ptr<renderer::mesh>
create_quad(const std::unique_ptr<renderer::graphics_api> &api) {
  static std::shared_ptr<renderer::mesh> g_quad_mesh = api->create_mesh(
      {
          {-1.0f, 1.0f, 0.0f},
          {1.0f, 1.0f, 0.0f},
          {-1.0f, -1.0f, 0.0f},
          {1.0f, -1.0f, 0.0f},
      },
      {0, 1, 2, 1, 2, 3}, {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}},
      {{.0f, 1.0f}, {1.0f, 1.0f}, {.0f, .0f}, {1.0f, .0f}});
  return g_quad_mesh;
}

inline std::vector<std::shared_ptr<renderer::mesh>>
mesh_load_from_path(const std::unique_ptr<renderer::graphics_api> &api,
                    const std::string &file_path) {
  using namespace renderer;
  std::vector<std::shared_ptr<renderer::mesh>> meshes;
  objl::Loader loader;
  if (loader.LoadFile(file_path)) {
    for (const auto &mesh : loader.LoadedMeshes) {

      renderer::vertices verts;
      renderer::tex_coords coords;
      renderer::normals norms;
      for (const auto &vert : mesh.Vertices) {

        const auto &vert_pos = vert.Position;
        const auto &tex_coord = vert.TextureCoordinate;
        const auto &norm = vert.Normal;

        verts.emplace_back(vert_pos.X, vert_pos.Y, vert_pos.Z);
        norms.emplace_back(norm.X, norm.Y, norm.Z);
        coords.emplace_back(tex_coord.X, tex_coord.Y);
      }
      meshes.emplace_back(api->create_mesh(verts, mesh.Indices, norms, coords));
    }
  }
  return meshes;
}

inline std::string read_file(const std::string &file_path) {
  std::ifstream file;
  file.open(file_path);
  if (!file.is_open()) {
    std::cerr << "Could not read " << file_path << "\n";
    std::exit(-1);
  }
  std::string line;
  std::string file_content;
  while (std::getline(file, line)) {
    file_content.append(line + "\n");
  }
  return file_content;
}

inline std::shared_ptr<renderer::texture>
texture_load_from_path(const std::unique_ptr<renderer::graphics_api> &api,
                       const std::string &file_path) {
  using namespace renderer;
  int w, h, channels;
  stbi_uc *data = stbi_load(file_path.c_str(), &w, &h, &channels, 0);
  if (!data) {
    std::cerr << "Could not load " << file_path << "\n";
    std::exit(-1);
  }

  texture_format fmt = texture_format::unknown;
  switch (channels) {
  case 1:
    fmt = texture_format::red;
    break;
  case 3:
    fmt = texture_format::rgb;
    break;
  case 4:
    fmt = texture_format::rgba;
    break;
  }

  auto texture = api->create_texture(data, w, h, fmt);
  stbi_image_free(data);

  return texture;
}

} // namespace utilities