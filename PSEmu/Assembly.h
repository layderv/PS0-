#pragma once
#include <cstdint>
#include <vector>

enum class Register : uint8_t;
class Assembly: public std::vector<uint32_t>
{
public:
	Assembly();
	~Assembly();

	Assembly& nop();

	Assembly& lb(Register rs, Register rt, uint16_t imm);
	Assembly& lbu(Register rs, Register rt, uint16_t imm);
	Assembly& lh(Register rs, Register rt, uint16_t imm);
	Assembly& lhu(Register rs, Register rt, uint16_t imm);
	Assembly& lw(Register rs, Register rt, uint16_t imm);
	Assembly& lwr(Register rs, Register rt, uint16_t imm);

	Assembly& sb(Register rs, Register rt, uint16_t imm);
	Assembly& sh(Register rs, Register rt, uint16_t imm);
	Assembly& swl(Register rs, Register rt, uint16_t imm);
	Assembly& sw(Register rs, Register rt, uint16_t imm);
	Assembly& swr(Register rs, Register rt, uint16_t imm);



private:
	uint8_t RNO(Register r);

	Assembly& load(const char* which, Register rs, Register rt, uint16_t imm);
};

enum class Register : uint8_t {
	R00,
	R01,
	R02,
	R03,
	R04,
	R05,
	R06,
	R07,
	R08,
	R09,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	R16,
	R17,
	R18,
	R19,
	R20,
	R21,
	R22,
	R23,
	R24,
	R25,
	R26,
	R27,
	R28,
	R29,
	R30,
	R31
};