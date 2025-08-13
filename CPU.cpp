#include <iostream>
#include <algorithm>
#include <vector>
#include "CPU.hpp"
#include "Instruction.hpp"
#include "Interconnect.hpp"
#include <cstdint>
#include <string>
#include <limits>

void CPU::run_next_instruction()
{
    uint32_t curr_pc = pc; 
    Instruction instruction = next_instruction;
    uint32_t raw_next_instruction = inter->load32(curr_pc); //32-bit bindary data read from memory
    next_instruction = Instruction(raw_next_instruction);
    pc = curr_pc + 4; 

    //Execute the pending load(if any, otherwise it will load $zero which is a NOP)
    //Set_reg works only on out_regs so this operation won't be visible by the next instructioin
    this->m_load;  
    

    decode_and_execute(instruction);

    //Copy the output registers as input for the next instruction
    regs = out_regs; 
}

CPU::CPU(std::unique_ptr<Interconnect> interconnect) 
    : pc(0xbfc00000), 
    inter(std::move(interconnect)), 
    regs(regs), 
    next_instruction(0x0), //NOP
    sr(0), out_regs(regs), m_load{RegisterIndex(0), 0}
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
//TODO add the implemented instruction opcodes to the decode and execute method 
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

void CPU::op_sltu(Instruction &instruction)
{
    RegisterIndex d = RegisterIndex(instruction.return_registers_two());
    RegisterIndex s = RegisterIndex(instruction.return_bits());
    RegisterIndex rt = RegisterIndex(instruction.return_registers());

    uint32_t v = reg(s) < reg(rt); 

    set_reg(d, v);
}

void CPU::op_sh(Instruction &instruction)
{
    if(sr & 0x100000 != 0)
    {
        //Cache is isolated, ignore write 
        std::cout << "Ignoring store while cache is isolated\n";
        return; 
    }

    uint32_t i = instruction.imm_se();
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());

    uint32_t addr = reg(s) + i; 
    uint16_t v = reg(rt); 

    store16(addr, v);
}

void CPU::store16(uint32_t addr, uint16_t val)
{
    inter->store16(addr, val);
}
void CPU::op_mtc0(Instruction &instruction)
{
    RegisterIndex cpu_r = RegisterIndex(instruction.return_registers());
    RegisterIndex cop_r = RegisterIndex(instruction.return_registers_two());
    

    uint32_t v = reg(cpu_r);

    switch(cop_r) 
    {
        case 3 | 5 | 6 | 7 | 9 | 11: // Breakpoints registers
            throw std::runtime_error("Unhandled wrtie to cop0r{}");
             break; 
        case 12: 
            sr = v; 
            break; 
        case 13: // Cause register 
            if(v != 0) 
            { 
                throw std::logic_error("unhandled write to CAUSE register");
            }
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
    //TODO fix this 
    if(sr & 0x10000 != 0)
    {
        //Cache is isolated, we ignore the write 
        std::cout << "Ignoring store while cache is isolated"; 
        return;
    }

    store32(addr, v);
}

void CPU::op_addu(Instruction &instruction)
{
    RegisterIndex d = RegisterIndex(instruction.return_registers_two());
    RegisterIndex s = RegisterIndex(instruction.return_bits());
    RegisterIndex rt = RegisterIndex(instruction.return_registers());

    uint32_t v = reg(s) + reg(rt); 

    set_reg(d, v); 
}

void CPU::op_lw(Instruction &instruction)
{
    if(sr & 0x10000 != 0)
    {
        //Cache is isolated, ignore write
        std::cout << "Ignoring load while cache is isolated \n"; 
        return;
    }

    uint32_t i = instruction.imm_se();
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());

    uint32_t addr = reg(s) + i; 
    uint32_t v = load32(addr);

    set_reg(rt, v);
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
    //TODO cast the operands ie imm and s to int32_t, then implement a check_add for the overflow error
    uint32_t imm = instruction.imm_se();
    int32_t i = static_cast<int32_t>(imm);
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());
    int32_t ss = static_cast<RegisterIndex>(s);

    if(!check_add(i, ss))
    {
        throw std::runtime_error("ADDI overflow error");
    }else 
    {
        uint32_t v = reg(s) + imm; 
        set_reg(rt, v);
    }
    
}

bool CPU::check_add(int32_t a, int32_t b)
{
    if(b > 0 && a > INT32_MAX - b) // a+b would overflow 
    {
        return false; 
    }
    if(b < 0 && a < INT32_MIN - b) //a+b would underflow
    {
        return false;
    }

    return true;
         
}

void CPU::op_j(Instruction &instruction)
{
    uint32_t imm = instruction.imm_jump(); 

    pc = (pc & 0xf0000000) | (imm << 2);
}

void CPU::op_jal(Instruction &instruction)
{
   uint32_t ra = this->pc; 

   set_reg(RegisterIndex(31), ra); 

   op_j(instruction); 

}

void CPU::op_andi(Instruction &instruction)
{
    uint32_t imm = instruction.return_immediate();
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());

    uint32_t v = reg(s) & imm; 

    set_reg(rt, v);
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

void CPU::op_sb(Instruction &instruction)
{
    if (sr & 0x10000 != 0)
    {
        std::cout << "Ignoring store while cache is isolated\n";
        return; 
    }

    uint32_t i = instruction.imm_se();
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());

    uint32_t addr = reg(s) + i; 
    uint8_t v = reg(rt); 

    store8(addr, v);
}

void CPU::op_jr(Instruction &instruction)
{
    RegisterIndex s = RegisterIndex(instruction.return_bits());

    this->pc = reg(s); 
}

void CPU::branch(uint32_t offset)
{
    offset = offset << 2; 
    uint32_t curr_pc = pc; 
    curr_pc = curr_pc + offset; 

    //Compensate for the plus 4 in the run_next_instruction method 
    curr_pc = curr_pc - 4; 

    pc = curr_pc; 
}

//Branch if Not Equal 
void CPU::op_bne(Instruction &instruction)
{
    uint32_t i = instruction.imm_se(); 
    RegisterIndex s = RegisterIndex(instruction.return_bits());
    RegisterIndex rt = RegisterIndex(instruction.return_registers());

    if(reg(s) != reg(rt))
    {
        branch(i);
    }
}

void CPU::store32(uint32_t addr, uint32_t val)
{
    inter->store32(addr, val);
}

void CPU::store8(uint32_t addr, uint8_t val)
{
   inter->store8(addr, val);
}

uint8_t CPU::load8(uint32_t addr)
{
    
}

void CPU::op_lb(Instruction &instruction)
{
    uint32_t i = instruction.imm_se(); 
    RegisterIndex rt = RegisterIndex(instruction.return_registers());
    RegisterIndex s = RegisterIndex(instruction.return_bits());
    uint32_t addr = reg(s) + i; 

    uint8_t v = load8(addr); 

    
}


