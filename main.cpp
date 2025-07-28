#include <iostream>
#include "CPU.hpp"
#include "Interconnect.hpp"
#include "BIOS.hpp"
#include "Range.hpp"
#include <memory>
#include <cstdint>


int main()
{
   auto bios = std::make_unique<Bios>("roms/SCPH1001.BIN");
   auto interconnect = std::make_unique<Interconnect>(std::move(bios));

   CPU cpu (std::move(interconnect));

   //Creates a fake LUI instruction 
   //uint32_t lui_instr = (0x2B << 26) | (4 << 21) | (5 << 16) | 0x0010;

   //cpu.set_reg(4, 0x10000000);
   //cpu.set_reg(5,0xCAFEBABE); //some value

   //Instruction instr(lui_instr);
   //cpu.decode_and_execute(instr);

   while(true){
    cpu.run_next_instruction();
   }

   return 0;
}