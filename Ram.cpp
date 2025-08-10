#include <vector>
#include "Ram.hpp"
#include <memory> 
#include <cstdint>
#include "Range.cpp"

// Ram::Ram():
//     { 

//     }; 

uint32_t Ram::load32(uint32_t offset)
{
    return (data[offset + 3] << 24) | (data[offset + 2] << 16) | (data[offset + 1] << 8) | (data[offset]);
}

void Ram::store32(uint32_t offset, uint32_t val)
{ 
    uint8_t b0 = val; 
    uint8_t b1 = (val >> 8); 
    uint8_t b2 = (val >> 16); 
    uint8_t b3 = (val >> 24); 

    data[offset + 0] = b0; 
    data[offset + 1] = b1; 
    data[offset + 2] = b2; 
    data[offset + 3] = b3; 
}

    