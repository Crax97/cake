//
// Created by crax on 11/1/21.
//

#include "game_framework/actor_instance.h"
#include "game_framework/component.h"
#include "object/serializer.h"


class override_field : public field {
private:
    bool b_was_set = false;
    field* m_wrapped;
public:
    explicit override_field(field* wrapped)
        : field(wrapped->get_name()), m_wrapped(wrapped) { }
    void set_from_string_internal(void *base, std::string_view value) override {
        m_wrapped->set_from_string(base, value);
        b_was_set = true;
    }

    const void *get_impl(void *base, const std::type_info &info) const override {
        return m_wrapped->get_impl(base, info);
    }

    void set_impl(void *base, const void *value, const std::type_info &info) override {
        m_wrapped->set_impl(base, value, info);
        b_was_set = true;
    }

    [[nodiscard]] const descriptor *get_field_descriptor() const override {
        return m_wrapped->get_field_descriptor();
    }

    std::string to_string(const void *base) const noexcept override {
        return m_wrapped->to_string(base);
    }

    void visit(void *base, field_visitor &visitor) override {
        return m_wrapped->visit(base, visitor);
    }
};

class override_descriptor_wrapper : public descriptor {
private:

public:
    override_descriptor_wrapper(descriptor *original)
            : descriptor("instance_actor", nullptr) {
        for(auto& field : original->get_fields()) {
            m_fields.emplace_back(std::make_unique<override_field>(field.get()));
        }
    }

    [[nodiscard]] const std::type_info &get_typeinfo() const override {
        return typeid(spectacle::actor_instance);
    }

};

void spectacle::actor_instance::serialize(serializer &ser) const {
    ser.begin_section("instance");
    MIKU_ASSERT(m_prototype)
    ser.add_parameter("prototype", std::string(m_prototype->get_name() ));
    for (const auto& overridden_actor_property : m_descriptor->get_fields()) {
    }
    ser.end_section("instance");
}

void spectacle::actor_instance::on_component_added(const std::shared_ptr<component> &new_component) {
    actor::on_component_added(new_component);


}

spectacle::actor_instance::actor_instance()
    : actor() {

    m_descriptor = std::make_unique<override_descriptor_wrapper>(get_descriptor_typed<actor>());
}

descriptor *spectacle::actor_instance::get_descriptor() const {
    return m_descriptor.get();
}

std::shared_ptr<spectacle::actor> spectacle::actor_instance::construct_instance(const std::shared_ptr<spectacle::actor> &base) {
    auto new_instance = std::make_shared<actor_instance>();
    new_instance->m_transform = base->get_transform();
    new_instance->set_name(base->get_name());
    new_instance->m_prototype = base;
    base->for_each_component([new_instance](auto& component) {
        auto new_component_instance = component->clone(*new_instance);
        new_instance->add_new_component(new_component_instance);
    });
    return new_instance;
}
