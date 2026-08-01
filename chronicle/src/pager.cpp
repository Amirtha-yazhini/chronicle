#include "pager.hpp"
#include <iostream>
#include <cstring>

Pager::Pager(const std::string& filename) : filename(filename), file_length(0) {
    for (uint32_t i = 0; i < MAX_PAGES; ++i) {
        pages[i] = nullptr;
    }

    // Open file in read/write binary mode; create if missing
    file_stream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file_stream.is_open()) {
        file_stream.clear();
        // Create fresh file
        file_stream.open(filename, std::ios::out | std::ios::binary);
        file_stream.close();
        file_stream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }

    file_stream.seekg(0, std::ios::end);
    file_length = static_cast<uint32_t>(file_stream.tellg());
}

Pager::~Pager() {
    close();
}

uint8_t* Pager::get_page(uint32_t page_num) {
    if (page_num >= MAX_PAGES) return nullptr;

    // Lazy load: allocate and fetch from file only on first access
    if (pages[page_num] == nullptr) {
        pages[page_num] = new uint8_t[PAGE_SIZE];
        std::memset(pages[page_num], 0, PAGE_SIZE);

        uint32_t num_pages_in_file = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE != 0) {
            num_pages_in_file += 1;
        }

        if (page_num < num_pages_in_file) {
            file_stream.seekg(page_num * PAGE_SIZE, std::ios::beg);
            file_stream.read(reinterpret_cast<char*>(pages[page_num]), PAGE_SIZE);
        }
    }

    return pages[page_num];
}

void Pager::flush(uint32_t page_num) {
    if (pages[page_num] == nullptr) return;

    file_stream.seekp(page_num * PAGE_SIZE, std::ios::beg);
    file_stream.write(reinterpret_cast<char*>(pages[page_num]), PAGE_SIZE);
    file_stream.flush();
}

void Pager::close() {
    for (uint32_t i = 0; i < MAX_PAGES; ++i) {
        if (pages[i] != nullptr) {
            flush(i);
            delete[] pages[i];
            pages[i] = nullptr;
        }
    }

    if (file_stream.is_open()) {
        file_stream.close();
    }
}