#pragma once

#include <algorithm>
#include <concepts>
#include <type_traits>

namespace spectacle {

namespace detail {

template <typename Iter>
concept Iterable = requires(Iter it) {
  {it.begin(), it.end()};
};

} // namespace detail
} // namespace spectacle