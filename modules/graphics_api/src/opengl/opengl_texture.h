#include "texture.h"

#include <glad/glad.h>

namespace renderer {
class opengl_texture final : public texture {
private:
  GLuint m_texture;

public:
  explicit opengl_texture(GLuint tex_id, int width, int height) noexcept;
  void bind_to_unit(int unit) override;
  GLuint get_texture() const noexcept { return m_texture; }

  void *get_texture_object() override {
    return reinterpret_cast<void *>(get_texture());
  }
  ~opengl_texture() noexcept override;
};
} // namespace renderer