#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include <cstring>
#include <string>

#pragma pack(push, 1)
struct PlayerRecord {
    uint32_t id;          // 4 bytes
    char username[32];    // 32 bytes
    char email[255];      // 255 bytes
};
#pragma pack(pop)

// Compile-time sanity check
static_assert(sizeof(PlayerRecord) == 291, "PlayerRecord must be exactly 291 bytes");

void serialize_player(uint32_t id, const std::string& username, const std::string& email, void* destination);
void deserialize_player(const void* source, uint32_t& id, std::string& username, std::string& email);

#endif