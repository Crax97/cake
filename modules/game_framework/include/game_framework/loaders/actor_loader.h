//
// Created by crax on 10/31/21.
//

#pragma once

#include <unordered_map>
#include <string_view>
#include <filesystem>
#include <optional>
#include <istream>
#include <memory>
#include <list>
namespace spectacle {
    class actor;
}
namespace gameframework {
    class actor_loader {
    private:
        std::unordered_map<std::string, std::shared_ptr<spectacle::actor>> m_preloaded_actors;
        actor_loader() = default;
    public:
        static actor_loader& the();
        void reset();
        void load_actors(const std::filesystem::path& root);
        std::optional<std::shared_ptr<spectacle::actor>> get_actor(std::string_view actor_name);
        void dump_actors(const std::filesystem::path& folder);
        std::list<std::shared_ptr<spectacle::actor>> get_actors();

        void add_prototype(const std::shared_ptr<spectacle::actor>& sharedPtr);
    };
}