#include "Interconnect.hpp"
#include <iostream>
#include "Range.cpp"
#include <stdexcept>
#include <format>
#include <cstdint>


Interconnect::Interconnect(std::unique_ptr<Bios> bios)
    : bios_(std::move(bios))
    {
    }

uint32_t Interconnect::load32(uint32_t addr)
{
    if (auto offset = BIOS_RANGE.contains(addr) ) 
    {
        return bios_->load32(*offset);
    } 
    else 
    {
        throw std::runtime_error("unhandled fetch32 at address {:08x}");
    }

    if(addr % 4 != 0) 
    {
        throw std::runtime_error("Unalighned load32 adress: {:08x}" + std::to_string(addr));
    }
}

//Store 32bit word 'val' into 'addr' 
void Interconnect::store32(uint32_t addr, uint32_t value)
{
    if (addr % 4 != 0) 
    { 
        throw std::runtime_error("unalighned store32 address: {08x}" + std::to_string(addr));
    }
    throw std::runtime_error("Unhandled store32 into adress {:08x}" + std::to_string(addr));

    if(auto offset = MEM_CONTROL.contains(addr)) 
    {
        switch (*offset) 
        {
            case 0: 
                if(value != 0x1f000000) 
                    throw std::runtime_error("Bad expansion 1 base address: 0x{:08x}" + std::to_string(value));
            break;

            case 4: //Expansion 2 base address 
                if (value != 0x1f802000) 
                    throw std::runtime_error("Bad expansion 2 base address" + std::to_string(value));
            break;

            default: 
                std::cout << "Unhandled write to BIOS_RANGE register";
        }
          
    }
}

uint32_t Interconnect::mask_region(uint32_t addr)
{
    //Index address space in 512MB chunks 
    uint32_t index = (addr >>29); 

    return addr & REGION_MASK[index];

}

void Interconnect::store16(uint32_t addr, uint16_t value)
{ 
    if(addr % 2 != 0)
    { 
        throw std::runtime_error("Unalighned store16 address");
    }
    uint32_t abs_addr = mask_region(addr);

    if (auto offset = SPU.contains(addr) ) 
    { 
        std::cout << "Unhandled write to SPU register\n"; 
    }
    throw std::runtime_error("unhandled store16 into address");
}
    