#include "Assembly.h"
#include <map>


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

uint8_t Assembly::RNO(Register r)
{
	return static_cast<uint8_t>(r);
}


