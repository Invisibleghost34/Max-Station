#pragma once 

#include "BIOS.hpp"
#include <cstdint>
#include <memory>

class Interconnect
{
private:
    std::unique_ptr<Bios> bios_;
public:
    Interconnect(std::unique_ptr<Bios> bios);

    uint32_t load32(uint32_t addr);
    void store32(uint32_t addr, uint32_t value); 
   
};



