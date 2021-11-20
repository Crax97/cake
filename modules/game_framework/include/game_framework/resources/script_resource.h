//
// Created by crax on 11/20/21.
//

#pragma once

#include "object/resource.h"
#include "luanatic/script_fwd.h"

#include <filesystem>

#pragma once
namespace game_framework {
    class script_resource : public resource {
    private:
        std::filesystem::path m_path;
        script_ptr m_script;
    public:
        [[nodiscard]] std::string to_string() const override {
            return m_path.string();
        }
        void from_string(std::string_view str) override;

        [[nodiscard]] script_ptr& get_script() {
            return m_script;
        }
        script_resource& operator=(script_ptr& new_script) {
            m_script = std::move(new_script);
            return *this;
        }
        std::string get_script_name() const noexcept;
        explicit operator bool() const {
            return m_script != nullptr;
        }
    };
}

DEFINE_DO_VISIT(game_framework::script_resource, visit_resource_property);