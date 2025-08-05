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
    std::array<uint32_t, 32> out_regs; //second set of registers used to emulate the load delay slot, contain the output of the current instruction
    struct load { 
        RegisterIndex id; 
        uint32_t value; 

    };
    void reset();
    void run_next_instruction();
    Instruction next_instruction; 
    uint32_t load32(uint32_t addr); 
    void op_lui(Instruction &instruction);
    void op_ori(Instruction &instruction);
    void op_sw(Instruction &instruction);
    void op_lw(Instruction &instruction);
    void op_sll(Instruction &instruction);
    void op_j(Instruction &instruction);
    void op_or(Instruction &instruction);
    void op_addiu(Instruction &instruction);
    void op_cop0(Instruction &instruction);
    void op_mtc0(Instruction &instruction);
    void branch(uint32_t offset); 
    void op_bne(Instruction &instruction);
    void store32(uint32_t addr, uint32_t val);
    bool check_add(int32_t a, int32_t b);
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

        //target the output register set 
        out_regs[static_cast<uint32_t>(index)] = val;
        out_regs[0] = 0; 
    };

private: 
    uint32_t pc; 
    std::array<uint32_t, 32> regs;
    std::unique_ptr<Interconnect> inter;
    
    uint32_t load32(uint32_t address) const; 
    
}; 