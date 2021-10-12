#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace spectacle {
class component;
class actor;
} // namespace spectacle

namespace gameframework {
using component_constructor_fn =
    std::function<std::shared_ptr<spectacle::component>(spectacle::actor &)>;

class component_repository {
private:
  std::unordered_map<std::string, gameframework::component_constructor_fn>
      m_components;
  component_repository(){};

public:
  static component_repository &the() noexcept;

  void register_new_component(const std::string &component_name,
                              component_constructor_fn &&creator) noexcept;
  std::optional<std::shared_ptr<spectacle::component>>
  construct_component(const std::string &component_name,
                      spectacle::actor &owner) noexcept;
  std::vector<std::string> get_component_names() noexcept;
};

template <class T> class registrators {};

} // namespace gameframework

#define DEFINE_COMPONENT_REGISTRATOR(CLASS)                                    \
  namespace gameframework {                                                    \
  template <> class registrators<CLASS> {                                      \
  public:                                                                      \
    static bool do_registration() noexcept {                                   \
      component_repository::the().register_new_component(                      \
          #CLASS, [](spectacle::actor &actor) {                                \
            return std::make_unique<CLASS>(actor);                             \
          });                                                                  \
      return true;                                                             \
    }                                                                          \
  };                                                                           \
  }                                                                            \
  namespace registration_##CLASS {                                             \
    bool b_is_registered =                                                     \
        ::gameframework::registrators<CLASS>::do_registration();               \
  }
