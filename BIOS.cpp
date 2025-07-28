#include "BIOS.hpp"
#include <fstream>
#include <stdexcept>
#include <cstdint>

Bios::Bios(const std::string &filepath) 
{
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if(!file) throw std::runtime_error("Failed to open the bios file");

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if(size != BIOS_SIZE) 
        throw std::runtime_error("BIOS Size is wrong"); 

    data.resize(BIOS_SIZE);
    if(!file.read(reinterpret_cast<char*>(data.data()), BIOS_SIZE))
        throw std::runtime_error("Cannot read BIOS data");

}

uint Bios::load32(uint32_t address) const 
{
    uint32_t offset = address & 0x000FFFFF; 
    return (data[offset + 3] << 24) | (data[offset + 2] << 16) | (data[offset + 1] << 8) | (data[offset]);
    // Reads 4 consecutive bytes from memory and returns a 32-bit unsigned integer
}