#include "Engine.hpp"
#include <iostream>
#include <sstream>

Command parseCommand(const std::string& input) {
    std::stringstream ss(input);
    std::string action, entity;
    
    if (!(ss >> action)) return {};

    if (action == "LIST") {
        if (ss >> entity && entity == "PLAYERS") {
            return {"LIST_PLAYERS", 0, "", ""};
        }
    } 
    else if (action == "SPAWN") {
        std::string raw_id, username, email;
        if ((ss >> entity >> raw_id >> username >> email) && entity == "PLAYER") {
            try {
                int parsed_id = std::stoi(raw_id);
                return {"SPAWN_PLAYER", static_cast<uint32_t>(parsed_id), username, email};
            } catch (...) {
                return {"SPAWN_PLAYER", 0, username, email};
            }
        }
    }

    return {"UNKNOWN", 0, "", ""};
}

ExecResult execute(const Command& cmd, WorldState& world) {
    if (cmd.type == "SPAWN_PLAYER") {
        if (static_cast<int>(cmd.id) <= 0) {
            return {false, "ID must be a positive integer."};
        }
        if (world.isIDTaken(cmd.id)) {
            return {false, "ID " + std::to_string(cmd.id) + " is already taken."};
        }
        if (cmd.username.empty() || cmd.username.length() > 32) {
            return {false, "Username must be between 1 and 32 characters."};
        }
        if (cmd.email.empty() || cmd.email.length() > 255) {
            return {false, "Email must be between 1 and 255 characters."};
        }

        world.addPlayer({cmd.id, cmd.username, cmd.email});
        return {true, "Spawned."};
    } 
  else if (cmd.type == "LIST_PLAYERS") {
    auto players = world.getPlayers();
    for (const auto& p : players) {
        std::cout << "[" << p.id << "] " << p.username << " <" << p.email << ">\n";
    }
    std::cout << players.size() << " entities.\n"; // Will now print "1 entities."
    return {true, ""};
}

    return {false, "Syntax Error or Unknown Command."};
}