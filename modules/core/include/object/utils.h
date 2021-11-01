#pragma once

#include "logging/logger.h"

namespace std {
template <typename T> T from_string(const std::string &s) {
    MIKU_ASSERT(false && "This shouldn't be reached");
    return T();
}

} // namespace std
