#include "Engine.hpp"
#include <iostream>
#include <sstream>

Command parseCommand(const std::string& input) {
    std::stringstream ss(input);
    std::string action, entity;
    
    if (!(ss >> action)) return {};

    // Parse LIST PLAYERS
    if (action == "LIST") {
        if (ss >> entity && entity == "PLAYERS") {
            return {"LIST_PLAYERS", 0, "", ""};
        }
    } 
    // Parse SPAWN PLAYER <id> <username> <email>
    else if (action == "SPAWN") {
        std::string raw_id, username, email;
        if ((ss >> entity >> raw_id >> username >> email) && entity == "PLAYER") {
            try {
                // Read any valid integer (positive or negative)
                int parsed_id = std::stoi(raw_id);
                return {"SPAWN_PLAYER", static_cast<unsigned int>(parsed_id), username, email};
            } catch (...) {
                // If it wasn't a valid integer at all
                return {"SPAWN_PLAYER", 0, username, email};
            }
        }
    }

    return {"UNKNOWN", 0, "", ""};
}

ExecResult execute(const Command& cmd, WorldState& world) {
    if (cmd.type == "SPAWN_PLAYER") {
        // Handle positive check explicitly (converting negative int cast to unsigned overflow back, or simply checking raw condition)
        // Since cmd.id is unsigned, negative numbers wrapped around to very large positive numbers, or equaled 0.
        // We check if static_cast<int>(cmd.id) <= 0
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

        // Store into temporary WorldState vector
        world.addPlayer({cmd.id, cmd.username, cmd.email});
        return {true, "Player " + cmd.username + " spawned successfully."};
    } 
    
    else if (cmd.type == "LIST_PLAYERS") {
        const auto& players = world.getPlayers();
        if (players.empty()) {
            std::cout << "No players active in world.\n";
        } else {
            std::cout << "--- Active Players ---\n";
            for (const auto& p : players) {
               // Updated format: [id] username email
                    std::cout << "[" << p.id << "] " 
                         << p.username << " " 
                        << p.email << "\n";
            }
        }
        return {true, "Listed " + std::to_string(players.size()) + " player(s)."};
    }

    return {false, "Syntax Error or Unknown Command."};
}