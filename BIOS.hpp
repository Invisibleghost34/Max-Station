#pragma once 
#include <cstdint>
#include <string>
#include <vector>

class Bios {
public: 
    explicit Bios(const std::string &filepath);

    uint32_t load32(uint32_t adress) const;
    

private: 
    static constexpr size_t BIOS_SIZE = 512 * 1024;
    //BIOS memory 
    std::vector<uint8_t> data; 
};