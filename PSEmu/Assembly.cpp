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

Assembly & Assembly::addiu(Register rs, Register rt, int16_t imm)
{
	this->push_back((0x09 << 26) | (RNO(rs) << 21) | (RNO(rt) << 16) | imm);
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

Assembly & Assembly::mult(Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + 0x18);
	return *this;
}

Assembly & Assembly::multu(Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + 0x19);
	return *this;
}

Assembly & Assembly::div(Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + 0x1a);
	return *this;
}

Assembly & Assembly::divu(Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + 0x1b);
	return *this;
}

Assembly & Assembly::add(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x20);
	return *this;
}

Assembly & Assembly::addu(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x21);
	return *this;
}

Assembly & Assembly::sub(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x22);
	return *this;
}

Assembly & Assembly::subu(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x23);
	return *this;
}

Assembly & Assembly:: And(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x24);
	return *this;
}

Assembly & Assembly:: Or(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x25);
	return *this;
}

Assembly & Assembly:: Xor(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x26);
	return *this;
}

Assembly & Assembly::nor(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x27);
	return *this;
}

Assembly & Assembly::slt(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x2a);
	return *this;
}

Assembly & Assembly::sltu(Register rd, Register rs, Register rt)
{
	this->push_back((RNO(rs) << 21) + (RNO(rt) << 16) + (RNO(rd) << 11) + 0x2b);
	return *this;
}

Assembly & Assembly::sll(Register rd, Register rt, uint8_t imm)
{
	this->push_back((RNO(rt) << 16) + (RNO(rd) << 11) + (imm & 0xe0) + 0);
	return *this;
}

Assembly & Assembly::srl(Register rd, Register rt, uint8_t imm)
{
	this->push_back((RNO(rt) << 16) + (RNO(rd) << 11) + (imm & 0xe0) + 0x02);
	return *this;
}

Assembly & Assembly::sra(Register rd, Register rt, uint8_t imm)
{
	this->push_back((RNO(rt) << 16) + (RNO(rd) << 11) + (imm & 0xe0) + 0x03);
	return *this;
}

Assembly & Assembly::sllv(Register rd, Register rt, uint8_t imm)
{
	this->push_back((RNO(rt) << 16) + (RNO(rd) << 11) + (imm & 0xe0) + 0x04);
	return *this;
}

Assembly & Assembly::srlv(Register rd, Register rt, uint8_t imm)
{
	this->push_back((RNO(rt) << 16) + (RNO(rd) << 11) + (imm & 0xe0) + 0x06);
	return *this;
}

Assembly & Assembly::srav(Register rd, Register rt, uint8_t imm)
{
	this->push_back((RNO(rt) << 16) + (RNO(rd) << 11) + (imm & 0xe0) + 0x07);
	return *this;
}

Assembly & Assembly::jr(Register rs)
{
	this->push_back((RNO(rs) << 21) + 0x08);
	return *this;
}

Assembly & Assembly::jalr(Register rs, Register rd)
{
	this->push_back((RNO(rs) << 21) + (RNO(rd) << 11) + 0x09);
	return *this;
}

Assembly & Assembly::sys(uint32_t comm)
{
	this->push_back((comm & 0xfffff) + 0x0c);
	return *this;
}

Assembly & Assembly::brk(uint32_t comm)
{
	this->push_back((comm & 0xfffff) + 0x0d);
	return *this;
}

Assembly & Assembly::mfhi(Register rd)
{
	this->push_back((RNO(rd) << 11) + 0x10);
	return *this;
}

Assembly & Assembly::mthi(Register rs)
{
	this->push_back((RNO(rs) << 21) + 0x11);
	return *this;
}

Assembly & Assembly::mflo(Register rd)
{
	this->push_back((RNO(rd) << 11) + 0x12);
	return *this;
}

Assembly & Assembly::mtlo(Register rs)
{
	this->push_back((RNO(rs) << 21) + 0x13);
	return *this;
}

