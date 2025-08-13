#pragma once 

#include "BIOS.hpp"
#include <cstdint>
#include <memory>
#include <map> 

class Interconnect
{
private:
    std::unique_ptr<Bios> bios_;
    const uint32_t REGION_MASK[8] = 
    {
        // KUSEG: 2048MB
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        // KSEG0: 512MB
        0x7fffffff,
        // KSEG1: 512MB
        0x1fffffff,
        // KSEG2: 1024MB
        0xffffffff, 0xffffffff,
    };

public:
    Interconnect(std::unique_ptr<Bios> bios);

    uint32_t load32(uint32_t addr);
    uint32_t mask_region(uint32_t addr);
    void store32(uint32_t addr, uint32_t value); 
    void store16(uint32_t addr, uint16_t value); 
   
};



