#include "Assembly.h"
#include <map>

Assembly::Assembly()
{
}


Assembly::~Assembly()
{
}

Assembly& Assembly::nop()
{
	this->push_back(0);
	return *this;
}



Assembly & Assembly::lb(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x20 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lbu(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x24 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lh(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x21 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lhu(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x25 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lw(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x23 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::lwr(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x26 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::sb(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x28 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::sh(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x29 << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::swl(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x2a << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::sw(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x2b << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

Assembly & Assembly::swr(Register rs, Register rt, uint16_t imm)
{
	this->push_back((0x2e << 26) + (RNO(rs) << 21) + (RNO(rt) << 16) + imm);
	return *this;
}

uint8_t Assembly::RNO(Register r)
{
	return static_cast<uint8_t>(r);
}


