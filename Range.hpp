#pragma once 
#include <cstdint>
#include <optional>

class Range {
    public: 
        Range(uint32_t start, uint32_t length ) 
            : start_(start) , length_(length) {}
        std::optional<uint32_t> contains(uint32_t addr) const 
        {
            if(addr >= start_ && addr < start_ + length_) 
            {
                return addr - start_; 

            } 
            else 
            {
                return std::nullopt;
            }
        }
        
    private: 
        uint32_t start_; 
        uint32_t length_;        
};