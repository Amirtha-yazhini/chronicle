#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <vector>
#include <cmath>
#include "pager.hpp"
#include "entity.hpp"

struct Player {
    uint32_t id;
    std::string username;
    std::string email;
};

class WorldState {
private:
    Pager pager;
    uint32_t num_entities;

public:
    explicit WorldState(const std::string& filename = "ironhold.world")
        : pager(filename) {
        num_entities = pager.get_file_length() / ENTITY_SIZE;
    }

    void* entity_slot(uint32_t index) {
        uint32_t page_num = index / ENTITIES_PER_PAGE;
        uint8_t* page = pager.get_page(page_num);
        uint32_t offset = (index % ENTITIES_PER_PAGE) * ENTITY_SIZE;
        return page + offset;
    }

    void addPlayer(const Player& player) {
        if (num_entities >= MAX_ENTITIES) return;
        void* slot = entity_slot(num_entities);
        serialize_player(player.id, player.username, player.email, slot);
        num_entities++;
    }

    std::vector<Player> getPlayers() {
    std::vector<Player> players;
    for (uint32_t i = 0; i < num_entities; ++i) {
        void* slot = entity_slot(i);
        Player p;
        deserialize_player(slot, p.id, p.username, p.email);
        
        // --- ADD THIS CHECK ---
        // Only count records that have a valid positive ID
        if (p.id > 0) {
            players.push_back(p);
        }
    }
    return players;
}
    bool isIDTaken(uint32_t id) const {
        for (uint32_t i = 0; i < num_entities; ++i) {
            const void* slot = const_cast<WorldState*>(this)->entity_slot(i);
            uint32_t pid;
            std::string uname, email;
            deserialize_player(slot, pid, uname, email);
            if (pid == id) return true;
        }
        return false;
    }

uint32_t getNumEntities() {
    return getPlayers().size();
}    
    uint32_t getPageCount() {
    uint32_t active_count = getNumEntities(); // Returns 2
    if (active_count == 0) return 0;
    return static_cast<uint32_t>(std::ceil(static_cast<double>(active_count) / ENTITIES_PER_PAGE));
}

    const std::string& getFilename() const { return pager.get_filename(); }

    void close() { pager.close(); }
};

struct Command {
    std::string type = "UNKNOWN";
    uint32_t id = 0;
    std::string username;
    std::string email;
};

struct ExecResult {
    bool success;
    std::string message;
};

Command parseCommand(const std::string& input);
ExecResult execute(const Command& cmd, WorldState& world);

#endif // ENGINE_HPP