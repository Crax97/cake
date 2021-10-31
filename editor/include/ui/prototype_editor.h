//
// Created by crax on 10/31/21.
//

#pragma once

#include "ui/actor_editor.h"

#include <memory>

namespace spectacle {
    class actor;
}
namespace editor {
    class prototype_editor {
    private:
        bool m_is_open = false;
        actor_editor m_actor_editor;
        std::weak_ptr<spectacle::actor> m_selected_actor;
    public:
        void open() {
            m_is_open = true;
        }

        void update();
    };
}
