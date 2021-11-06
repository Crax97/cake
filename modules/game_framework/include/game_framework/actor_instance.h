//
// Created by crax on 11/1/21.
//

#pragma once
#include "actor.h"
#include <unordered_set>
#include <unordered_map>
class descriptor;
class override_descriptor_wrapper;
namespace spectacle {
    class actor_instance : public actor {
    private:
        std::unique_ptr<override_descriptor_wrapper> m_descriptor;
    public:
        actor_instance();

        void serialize(serializer &serializer) const override;
        void on_component_added(const std::shared_ptr<component> &new_component) override;
        static std::shared_ptr<actor> construct_instance(const std::shared_ptr<actor>& base);

        descriptor *get_descriptor() const override;

    };
}