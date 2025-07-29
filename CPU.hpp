#pragma once
#include <array>
#include "Instruction.hpp"
#include "Interconnect.hpp"
#include <memory>
#include <cstdint>

class CPU { 
public: 
    CPU(std::unique_ptr<Interconnect> interconnect);
    struct RegisterIndex 
    {
        uint32_t value; 

        explicit RegisterIndex(uint32_t v) : value(v)
        {
            if (v >= 32)
            {
                throw std::out_of_range("Register index out of range");
            }
        }

        operator uint32_t() const {return value;}
    };

    //Cop0 register 12: Status Register 
    uint32_t sr; 
    void reset();
    void run_next_instruction();
    Instruction next_instruction; 
    uint32_t load32(uint32_t addr); 
    void op_lui(Instruction &instruction);
    void op_ori(Instruction &instruction);
    void op_sw(Instruction &instruction);
    void op_sll(Instruction &instruction);
    void op_j(Instruction &instruction);
    void op_or(Instruction &instruction);
    void op_addiu(Instruction &instruction);
    void op_cop0(Instruction &instruction);
    void store32(uint32_t addr, uint32_t val);
    void decode_and_execute(Instruction &instruction);

    uint32_t reg(RegisterIndex index)  
    {
        if(index >= regs.size())
        {
            throw std::out_of_range("Register index out of range");
        }
        return regs[static_cast<uint32_t>(index)];
    }; 
    void set_reg(RegisterIndex index, uint32_t val) 
    {
        regs[static_cast<uint32_t>(index)] = val; 
        regs[0] = 0; 
    };

private: 
    uint32_t pc; 
    std::array<uint32_t, 32> regs;
    std::unique_ptr<Interconnect> inter;
    
    uint32_t load32(uint32_t address) const; 
    
}; 