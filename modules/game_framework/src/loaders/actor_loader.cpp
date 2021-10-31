//
// Created by crax on 10/31/21.
//

#include "game_framework/loaders/actor_loader.h"
#include "game_framework/loaders/stage_serializer.h"
#include "game_framework/loaders/actor_deserializer.h"
#include "game_framework/actor.h"

#include <fstream>

gameframework::actor_loader &gameframework::actor_loader::the() {
    static actor_loader loader;
    return loader;
}

void gameframework::actor_loader::reset() {
    m_preloaded_actors.clear();
}

std::optional<std::shared_ptr<spectacle::actor>> gameframework::actor_loader::get_actor(std::string_view actor_name) {
    auto present = m_preloaded_actors.find(std::string(actor_name));
    if(present != m_preloaded_actors.end()) {
        return present->second;
    }
    std::fstream actor_file;
    // TODO Resolve content:// url
    actor_file.open(std::filesystem::path("actors") / actor_name / ".actor");
    actor_deserializer dese;
    dese.deserialize(actor_file);
    if(dese.good()) {
        m_preloaded_actors[std::string(actor_name)] = dese.get_actor();
        return dese.get_actor();
    }

    return {};
}

void gameframework::actor_loader::dump_actors(const std::filesystem::path& folder) {
    for(const auto& [name, actor] : m_preloaded_actors) {
        std::fstream output;
        output.open(folder / "actors" / name / ".actor");
        stage_serializer ser(output);
        actor->serialize(ser);
    }
}

std::list<std::shared_ptr<spectacle::actor>> gameframework::actor_loader::get_actors() {
    std::list<std::shared_ptr<spectacle::actor>> actors;
    for(auto& [_, actor] : m_preloaded_actors) {
        actors.push_back(actor);
    }
    return actors;
}

void gameframework::actor_loader::load_actors(const std::filesystem::path &root) {
    for(auto& dir : std::filesystem::directory_iterator(root / "actors")) {
        if(dir.is_directory()) continue;
        std::fstream input;
        input.open(dir.path(), std::ios::in);
        actor_deserializer dese;
        dese.deserialize(input);

        m_preloaded_actors[dir.path().filename()] = dese.get_actor();
    }
}

void gameframework::actor_loader::add_prototype(const std::shared_ptr<spectacle::actor>& new_actor) {
    auto proto = new_actor->clone();
    m_preloaded_actors[std::string(proto->get_name())] = proto;
}
