#include "Assembly.h"
#include <map>


Assembly & Assembly::bltz(Register rs, uint16_t imm)
{
	this->push_back((0x1 << 26) + (RNO(rs) << 21) + imm);
	return *this;
}

Assembly & Assembly::bgez(Register rs, uint16_t imm)
{
	this->push_back((0x1 << 26) + (RNO(rs) << 21) + (0x1 << 16) + imm);
	return *this;
}

Assembly & Assembly::bltzal(Register rs, uint16_t imm)
{
	this->push_back((0x1 << 26) + (RNO(rs) << 21) + (0x10 << 16) + imm);
	return *this;
}

Assembly & Assembly::bgezal(Register rs, uint16_t imm)
{
	this->push_back((0x1 << 26) + (RNO(rs) << 21) + (0x11 << 16) + imm);
	return *this;
}

Assembly& Assembly::nop()
{
	this->push_back(0);
	return *this;
}



Assembly & Assembly::lb(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x20 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lbu(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x24 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lh(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x21 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lhu(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x25 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lw(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x23 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lwr(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x26 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::sb(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x28 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::sh(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x29 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::swl(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x2a << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::sw(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x2b << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::swr(Register rt, Register rs, uint16_t imm)
{
	this->push_back((0x2e << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::j(uint32_t imm26)
{
	push_back((0x02 << 26) + ((imm26 & 0xfffffff) >> 2));
	return *this;
}

Assembly & Assembly::jal(uint32_t imm26)
{
	push_back((0x03 << 26) + ((imm26 & 0xfffffff) >> 2));
	return *this;
}

Assembly & Assembly::beq(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x04 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::bne(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x05 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::blez(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x06 << 26) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::addi(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x08 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::addiu(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x09 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::slti(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x0a << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::sltiu(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x0b << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::andi(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x0c << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::ori(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x0d << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::xori(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x0e << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lui(Register rt, uint16_t imm)
{
	this->push_back((0x0f << 26) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::bgtz(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x07 << 26) + (RNO(rt) << 16) + imm);
	return *this;
}

uint8_t Assembly::RNO(Register r)
{
	return static_cast<uint8_t>(r);
}


