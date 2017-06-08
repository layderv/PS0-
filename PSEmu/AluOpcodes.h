#pragma once
#include <cstdint>
#include "Instruction.h"

struct AluOpcodes {
	static comp_result alu_dispatch(instruction i, uint32_t rs, uint32_t rt) {
		switch (i.code) {
		case 0x00:	return alu_dispatch_special(i, rs, rt);
			//1 BcondZ
			//2 J
			//2 JAL
			//4 BEQ
			//5 BNE
			//6 BLEZ
			//7 BGTZ
		case 0x08:	return Addi(i,rs,rt);
		case 0x09:	return Addiu(i, rs, rt);
		case 0x0a:	return Slti(i, rs, rt);
		case 0x0b:	return Sltiu(i, rs, rt);
		case 0x0c:	return Andi(i, rs, rt);
		case 0x0d:	return Ori(i, rs, rt);
		case 0x0e:	return Xori(i, rs, rt);
			//f LUI
			//10-13 COP
			//20-26 load
			//28-2b,2e store
			//30-33 load cop
			//38-3b store cop
		}
		return reserved_instruction_exception;
	}
	static comp_result alu_dispatch_special(instruction i, uint32_t rs, uint32_t rt) {
		switch (i.xcode) {
		case 0x00:	return Sll(i, rs, rt);
		case 0x02:	return Srl(i, rs, rt);
		case 0x03:	return Sra(i, rs, rt);
		case 0x04:	return Sllv(i, rs, rt);
		case 0x06:	return Srlv(i, rs, rt);
		case 0x07:	return Srav(i, rs, rt);
			//8 jr
			//9 jalr
			//c syscall
			//d break
			//10-13 mt/mf
			//18-1b mult/div
		case 0x20:	return Add(i, rs, rt);
		case 0x21:	return Addu(i, rs, rt);
		case 0x22:	return Sub(i, rs, rt);
		case 0x23:	return Subu(i, rs, rt);
		case 0x24:	return And(i, rs, rt);
		case 0x25:	return Or(i, rs, rt);
		case 0x26:	return Xor(i, rs, rt);
		case 0x27:	return Nor(i, rs, rt);
		case 0x2a:	return Slt(i, rs, rt);
		case 0x2b:	return Sltu(i, rs, rt);
		}
		return reserved_instruction_exception;
	}
	//immediates
	static constexpr comp_result Addi(instruction i, uint32_t rs, uint32_t rt) { return check(b(rs) + i.immi16); }
	static constexpr comp_result Addiu(instruction i, uint32_t rs, uint32_t rt) { return rs + i.immi16; }
	static constexpr comp_result Slti(instruction i, uint32_t rs, uint32_t rt) { return s(rs) < i.immi16 ? 1 : 0; }
	static constexpr comp_result Sltiu(instruction i, uint32_t rs, uint32_t rt) { return rs < i.imm16 ? 1 : 0; }
	static constexpr comp_result Andi(instruction i, uint32_t rs, uint32_t rt) { return rs & i.imm16; }
	static constexpr comp_result Ori(instruction i, uint32_t rs, uint32_t rt) { return rs | i.imm16; }
	static constexpr comp_result Xori(instruction i, uint32_t rs, uint32_t rt) { return rs ^ i.imm16; }
	static constexpr comp_result Lui(instruction i, uint32_t rs, uint32_t rt) { return i.imm16 << 16; }

	//special
	static constexpr comp_result Add(instruction i, uint32_t rs, uint32_t rt) { return check(b(rs) + b(rt)); }
	static constexpr comp_result Addu(instruction i, uint32_t rs, uint32_t rt) { return rs + rt; }
	static constexpr comp_result Sub(instruction i, uint32_t rs, uint32_t rt) { return check(b(rs) - b(rt)); }
	static constexpr comp_result Subu(instruction i, uint32_t rs, uint32_t rt) { return rs - rt; }
	static constexpr comp_result And(instruction i, uint32_t rs, uint32_t rt) { return rs & rt; }
	static constexpr comp_result Or(instruction i, uint32_t rs, uint32_t rt) { return rs | rt; }
	static constexpr comp_result Xor(instruction i, uint32_t rs, uint32_t rt) { return rs ^ rt; }
	static constexpr comp_result Nor(instruction i, uint32_t rs, uint32_t rt) { return ~(rs | rt); }
	static constexpr comp_result Slt(instruction i, uint32_t rs, uint32_t rt) { return s(rs) < s(rt) ? 1 : 0; }
	static constexpr comp_result Sltu(instruction i, uint32_t rs, uint32_t rt) { return rs < rt ? 1 : 0; }

	static constexpr comp_result Sllv(instruction i, uint32_t rs, uint32_t rt) { return rt << rs; }
	static constexpr comp_result Srlv(instruction i, uint32_t rs, uint32_t rt) { return rt >> rs; }
	static constexpr comp_result Srav(instruction i, uint32_t rs, uint32_t rt) { return s(rt) >> rs; }

	static constexpr comp_result Sll(instruction i, uint32_t rs, uint32_t rt) { return rt << i.p4; }
	static constexpr comp_result Srl(instruction i, uint32_t rs, uint32_t rt) { return rt >> i.p4; }
	static constexpr comp_result Sra(instruction i, uint32_t rs, uint32_t rt) { return s(rt) >> i.p4; }


private:
	static constexpr comp_result overflow_exception{ 0, 0x0c << 2 };
	static constexpr comp_result reserved_instruction_exception{ 0, 0x0a << 2 };
	static constexpr comp_result check(uint64_t r) { return  ((r >> 32) + 1 < 2) ? r : overflow_exception; }
	static constexpr int64_t b(uint32_t r) { return s(r); }
	static constexpr int32_t s(uint32_t r) { return static_cast<int32_t>(r); }
	static constexpr int16_t s(uint16_t r) { return static_cast<int16_t>(r); }
	static constexpr uint16_t l(uint32_t r) { return r & 0xffff; }
	static constexpr uint16_t h(uint32_t r) { return r >>16; }
	static constexpr int16_t sl(uint32_t r) { return s(l(r)); }
	static constexpr int16_t sh(uint32_t r) { return s(h(r)); }
};