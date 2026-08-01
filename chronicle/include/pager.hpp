#ifndef PAGER_HPP
#define PAGER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <cstdint>

constexpr uint32_t PAGE_SIZE = 4096;
constexpr uint32_t ENTITY_SIZE = 291;
constexpr uint32_t ENTITIES_PER_PAGE = PAGE_SIZE / ENTITY_SIZE; // 14
constexpr uint32_t MAX_PAGES = 100;
constexpr uint32_t MAX_ENTITIES = ENTITIES_PER_PAGE * MAX_PAGES; // 1400

class Pager {
private:
    std::string filename;
    std::fstream file_stream;
    uint32_t file_length;
    uint8_t* pages[MAX_PAGES];

public:
    explicit Pager(const std::string& filename);
    ~Pager();

    uint8_t* get_page(uint32_t page_num);
    void flush(uint32_t page_num);
    void close();

    uint32_t get_file_length() const { return file_length; }
    const std::string& get_filename() const { return filename; }
};

#endif // PAGER_HPP