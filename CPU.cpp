#include <iostream>
#include <algorithm>
#include <vector>
#include "CPU.hpp"
#include "Instruction.hpp"
#include "Interconnect.hpp"
#include <cstdint>
#include <string>


void CPU::run_next_instruction()
{
    uint32_t curr_pc = pc; 
    Instruction instruction = next_instruction;
    uint32_t raw_next_instruction = inter->load32(curr_pc); //32-bit bindary data read from memory
    next_instruction = Instruction(raw_next_instruction);
    pc = curr_pc + 4; 

    decode_and_execute(instruction);
}

CPU::CPU(std::unique_ptr<Interconnect> interconnect) 
    : pc(0xbfc00000), 
    inter(std::move(interconnect)), 
    regs(regs), 
    next_instruction(0x0), //NOP
    sr(0)
    {
        regs.fill(0xdeadbeee);
        regs[0] = 0;
    }

void CPU::reset()
{
    pc = 0xbfc00000;
    // std::fill(std::begin(regs.begin(), regs.end(), 0));
}

uint32_t CPU::load32(uint32_t addr) const 
{ 
    return inter->load32(addr);
}

void CPU::decode_and_execute(Instruction &instruction)
{
    switch (instruction.return_bits())
    {
        case 0b000000: 
            switch (instruction.return_bits_two())
            {
                case 0b000000: 
                    op_sll(instruction); 
                    break; 
                case 0b100101: 
                    op_or(instruction);
                    break;
                default: 
                    throw std::runtime_error("Unhandled instruction {:08x}" + std::to_string(instruction.op));
            }
        case 0b001111: //Load upper immediate instruction in MIPS
            op_lui(instruction);
            break;
        case 0b001001:
            op_addiu(instruction);
            break;
        case 0b001101: // ORI bitwirse OR immediate instruction 
            op_ori(instruction);
        case 0b101011: // store operation 
            op_sw(instruction);
            break;
        case 0b010000: 
            op_cop0(instruction);
            break;
        default: 
            throw std::runtime_error("Unhandled instruction {:08x}" + 
                std::to_string(instruction.op));

    }
    
}

void CPU::op_cop0(Instruction &instruction) 
{
    switch(instruction.cop_opcode())
    {
        default: 
            throw std::runtime_error("Unhandled cop0 instruction");
    }
}

void CPU::op_mtc0(Instruction &instruction)
{
    RegisterIndex cpu_r = RegisterIndex(instruction.return_registers());
    RegisterIndex cop_r = RegisterIndex(instruction.return_registers_two());
    

    uint32_t v = reg(cpu_r);

    switch(cop_r) 
    {
        case 12: 
            sr = v; 
            break; 
        default: 
            throw std::runtime_error("Unhandled cop0 register");
    }

}

void CPU::op_lui(Instruction &instruction)
{
    RegisterIndex rt = RegisterIndex(instruction.return_registers()); 
    uint32_t imm = instruction.return_immediate();

    uint32_t v = imm << 16; 

    set_reg(rt, v);

    throw std::runtime_error("What now?");
}

void CPU::op_ori(Instruction &instruction)
{
    uint32_t imm = instruction.return_immediate();
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());

    uint32_t v = reg(s) | imm; // | is bitwise or 

    set_reg(rt, v);
}

void CPU::op_sw(Instruction &instruction)
{
    int32_t imm = instruction.imm_se(); 
    RegisterIndex rt = RegisterIndex(instruction.return_registers()); 
    RegisterIndex s = RegisterIndex(instruction.return_bits()); 

    uint32_t addr = reg(s) + imm; 
    uint32_t v = reg(rt);

    store32(addr, v);
}

//shift left logical SLL 
void CPU::op_sll(Instruction &instruction)
{
    uint32_t i = instruction.shift();
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex d = RegisterIndex(instruction.return_registers_two());

    uint32_t v = reg(rt) << i; 

    set_reg(d, v); 
}

void CPU::op_addiu(Instruction &instruction)
{
    uint32_t imm = instruction.imm_se();
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());

    uint32_t v = reg(s) + imm; 
    set_reg(rt, v);
}

void CPU::op_j(Instruction &instruction)
{
    uint32_t imm = instruction.imm_jump(); 

    pc = (pc & 0xf0000000) | (imm << 2);
}

//Bitwise Or 
void CPU::op_or(Instruction &instruction)
{
    RegisterIndex d = RegisterIndex(instruction.return_registers_two());
    RegisterIndex s = RegisterIndex(instruction.return_bits());
    RegisterIndex rt = RegisterIndex(instruction.return_registers());

    uint v = reg(s) | reg(rt);
    set_reg(d, v);
}

void CPU::store32(uint32_t addr, uint32_t val)
{
    inter->store32(addr, val);
}


