#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace renderer {
class shader {
public:
  virtual void use() = 0;
  virtual void set_uniform_matrix4x4(const std::string &uniform_name,
                                     float *mat4x4) = 0;
  virtual void set_uniform_float(const std::string &uniform_name,
                                 float value) = 0;
  virtual void set_uniform_vec3(const std::string &uniform_name,
                                float *vec) = 0;
  virtual void bind_textures(
      std::vector<std::pair<std::string, std::shared_ptr<class texture>>>
          textures) = 0;
};
} // namespace renderer