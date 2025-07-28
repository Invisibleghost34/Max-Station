#include "Range.hpp"

const Range BIOS_RANGE(0xbfc00000, 512 * 1024);
const Range MEM_CONTROL(0x1f801000, 36);
const Range RAM_SIZE(0x1f801060, 4);
const Range CACHE_CONTROL(0xfffe0130, 4); 