//
// Created by crax on 10/30/21.
//

#include "object/resource.h"
#include "object/descriptor.h"

class resource_descriptor : public descriptor {
public:
    resource_descriptor()
        : descriptor("resource", nullptr) { }
    [[nodiscard]] const std::type_info& get_typeinfo() const override {
        return typeid(resource);
    }
};

template<>
descriptor* get_descriptor_typed<resource>() {
    static resource_descriptor desc;
    return &desc;
}

descriptor* resource::get_descriptor() const {
    return get_descriptor_typed<resource>();
}

