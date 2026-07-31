#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <vector>

// Simple Player Struct
struct Player {
    unsigned int id;
    std::string username;
    std::string email;
};

class WorldState {
private:
    std::vector<Player> players;

public:
    void addPlayer(const Player& player) {
        players.push_back(player);
    }

    const std::vector<Player>& getPlayers() const {
        return players;
    }

    bool isIDTaken(unsigned int id) const {
        for (const auto& p : players) {
            if (p.id == id) return true;
        }
        return false;
    }
};

struct Command {
    std::string type = "UNKNOWN"; // "SPAWN_PLAYER", "LIST_PLAYERS", or "UNKNOWN"
    unsigned int id = 0;
    std::string username;
    std::string email;
};


struct ExecResult {
    bool success;
    std::string message;
};

// Function declarations
Command parseCommand(const std::string& input);
ExecResult execute(const Command& cmd, WorldState& world);

#endif