//
// Created by crax on 10/26/21.
//

#include "fields.h"

#pragma once
#ifdef KATSU_GEN
#define REFLECT(...) __attribute__((annotate("reflect")))
#else
#define REFLECT(...)
#endif

#define CORE_GENERATE_BODY(Class)                                                                           \
public:                                                                                                     \
  friend class Class##_descriptor;                                                                          \
  descriptor *get_descriptor() const override;                                                              \
private:

#define CORE_BEGIN_DESCRIPTOR(Class) \
    class Class##_descriptor : public descriptor {                                                          \
    using ClassType = Class;                                                                                \
    public:                                                                                                 \
        Class##_descriptor() : descriptor(#Class, nullptr) {                                                \

#define CORE_REFLECT_MEMBER(Member)                                                                         \
        field_adder<ClassType, decltype(ClassType::Member)>()(m_fields, #Member, &ClassType::Member);       \

#define CORE_REFLECT_METHOD(Member)                                                                         \
        m_methods.emplace(#Member, create_callable(&ClassType::Member));

#define CORE_END_DESCRIPTOR()                                                                               \
    }                                                                                                       \
                                                                                                            \
    [[nodiscard]] const std::type_info& get_typeinfo() const override {                                     \
        return typeid(ClassType);                                                                           \
    }                                                                                                       \
};                                                                                                          \

#define CORE_IMPLEMENT_FUNCTIONS(Class)                                                                     \
template<>                                                                                                  \
descriptor* get_descriptor_typed<Class>() {                                                                 \
    static Class##_descriptor desc;                                                                         \
    return &desc;                                                                                           \
}                                                                                                           \
descriptor* Class::get_descriptor() const {                                                                 \
    return get_descriptor_typed<Class>();                                                                   \
}                                                                                                           \
                                                                                                            \

