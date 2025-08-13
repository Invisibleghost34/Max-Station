#include "Range.hpp"

const Range BIOS_RANGE(0xbfc00000, 512 * 1024);
const Range MEM_CONTROL(0x1f801000, 36);
const Range RAM_SIZE(0x1f801060, 4);
const Range RAM(0xa000000, 2 * 1024 * 1024); 
const Range BIOS(0x1fc00000, 512 * 1024); 
const Range SYS_CONTROL(0x1f801000, 36);
const Range SPU(0x1f801c00, 640); 
const Range EXPANSION_2(0x1f802000, 66); 
// Cache control register 
const Range CACHE_CONTROL(0xfffe0130, 4); 