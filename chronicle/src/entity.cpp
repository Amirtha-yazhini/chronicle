#include "entity.hpp"

void serialize_player(uint32_t id, const std::string& username, const std::string& email, void* destination) {
    std::memset(destination, 0, sizeof(PlayerRecord));
    
    PlayerRecord* record = reinterpret_cast<PlayerRecord*>(destination);
    record->id = id;
    
    std::strncpy(record->username, username.c_str(), sizeof(record->username) - 1);
    std::strncpy(record->email, email.c_str(), sizeof(record->email) - 1);
}

void deserialize_player(const void* source, uint32_t& id, std::string& username, std::string& email) {
    const PlayerRecord* record = reinterpret_cast<const PlayerRecord*>(source);
    id = record->id;
    
    // Ensure null-termination safety
    char uname_buf[33] = {0};
    char email_buf[256] = {0};
    
    std::memcpy(uname_buf, record->username, 32);
    std::memcpy(email_buf, record->email, 255);
    
    username = std::string(uname_buf);
    email = std::string(email_buf);
}