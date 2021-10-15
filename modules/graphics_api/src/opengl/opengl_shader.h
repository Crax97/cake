#include "shader.h"

#include <glad/glad.h>
#include <optional>
#include <unordered_map>

namespace renderer {
class opengl_shader final : public shader {
private:
  GLuint m_program;
  std::unordered_map<std::string, GLint> uniforms;
  GLint get_uniform_location(const std::string &basicString) noexcept;

public:
  opengl_shader(GLuint vs, GLuint fs) noexcept;
  void use() override;
  void set_uniform_matrix4x4(const std::string &uniform_name,
                             float *mat4x4) override;
  virtual void set_uniform_float(const std::string &uniform_name,
                                 float value) override;
  void set_uniform_vec3(const std::string &uniform_name, float *vec) override;
  void bind_textures(
      std::vector<std::pair<std::string, std::shared_ptr<texture>>> textures)
      override;
  ~opengl_shader() noexcept;

  enum KNOWN_LOCATIONS {
    VertexPosition = 0,
    TexcoordPosition = 1,
    NormalPosition = 2,
  };
};
} // namespace renderer