//
// Created by crax on 10/29/21.
//

#include "object/resource.h"
#include "texture.h"

#include <filesystem>

#pragma once
namespace game_framework {
    class texture_resource : public resource {
    private:
        std::filesystem::path m_path;
        std::shared_ptr<renderer::texture> m_texture;
    public:
        [[nodiscard]] std::string to_string() const override {
            return m_path.string();
        }
        void from_string(std::string_view str) override;

        [[nodiscard]] std::shared_ptr<renderer::texture>& get_texture() {
            return m_texture;
        }
        texture_resource& operator=(const std::shared_ptr<renderer::texture>& new_texture) {
            m_texture = new_texture;
            return *this;
        }
        explicit operator bool() const {
            return m_texture != nullptr;
        }
    };
}

DEFINE_DO_VISIT(game_framework::texture_resource, visit_resource_property);