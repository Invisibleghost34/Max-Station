#pragma once 
#include <vector>
#include <cstdint>

class Ram
{ 
public: 
Ram(); // Ram constructor
std::vector<uint8_t> data; 
uint32_t load32(uint32_t offset); 
void store32(uint32_t offset, uint32_t val); 
private: 
}; 