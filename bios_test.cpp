// #include <iostream>
// #include <vector>
// #include <cassert>
// #include "BIOS.hpp"
// #include "BIOS.cpp"

// int main()
// {
//     // Fake BIOS 
//     std::vector<uint8_t> bios_data(512 * 1024, 0);
//     bios_data[0] = 0xAA;
//     bios_data[1] = 0xBB;
//     bios_data[2] = 0xCC;
//     bios_data[3] = 0xDD; 

//     //create Bios from constructor 
//     Bios bios(bios_data);

//     //testing the read32() method 
//     uint32_t result = bios.read32(0);
//     assert(result == 0xDDCCBBAA);

//     std::cout << "Bios read32 method works";
//     return 0;

// }