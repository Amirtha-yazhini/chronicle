#ifndef input_buffer_h
#define input_buffer_h

#include <string>


#include <vector>

class InputBuffer {
private:
   
    std::string trim(const std::string& str);

public:
    void processAndLog(const std::vector<std::string>& history, const std::string& filename);
};
#endif