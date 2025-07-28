#pragma once 
#include <cstdint>


struct Instruction 
{
    uint32_t op; 
    explicit Instruction(uint32_t value) : op(value){}
    //returns bits [31:26] of the instruction 
    uint32_t return_bits() 
    {
        return op >> 26; 
    }
    //return register index in bits [20:16]
    uint32_t return_registers() 
    {
        return (op >> 16) & 0x1f; 
    }
    //return register index in bits [15:11]
    uint32_t return_registers_two()
    {
        return (op >> 11) & 0x1f;
    }
    // Return bits [5:0] of the instruction 
    uint32_t return_bits_two()
    {
        return op & 0x3f;
    }
    //shit Immediate values are stored in bits [10:6]
    uint32_t shift()
    {
        return (op >> 6) & 0x1f;
    }
    
    uint32_t return_immediate() 
    {
        return op & 0xffff; 
    }

    //Return immediate value in bits [16:0] as a sign-extended 32 bit value 
    uint32_t imm_se() 
    {
        return static_cast<uint32_t>(static_cast<int16_t>(op & 0xffff));
    }

    uint32_t imm_jump()
    {
        return op & 0x3ffffff; 
    }
};