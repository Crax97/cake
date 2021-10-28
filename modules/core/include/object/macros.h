//
// Created by crax on 10/26/21.
//

#pragma once
#ifdef KATSU_GEN
#define REFLECT(...) __attribute__((annotate("reflect")))
#else
#define REFLECT(...)
#endif

#define GENERATE_REFLECT_BODY(Class)                                           \
public:                                                                        \
  friend class Class##_descriptor;                                             \
  descriptor *get_descriptor() const override;                                       \
private:

