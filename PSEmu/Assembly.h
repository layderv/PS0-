#pragma once
#include <cstdint>
#include <vector>
#include "Registers.h"

#include "MemoryDevice.h"

enum class Register : uint8_t;
class Assembly: public std::vector<uint32_t>, public MemoryDevice
{
public:
	Assembly() = default;
	~Assembly() = default;

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
	
	Assembly& j(uint32_t imm26);
	Assembly& jal(uint32_t imm26);

	virtual void Write(uint32_t address, uint32_t size, uint8_t const * data) {
		auto b = reinterpret_cast<uint8_t*>(&(*begin()));
		std::copy(data, data + size,b + address);
	}
	virtual void Read(uint32_t address, uint32_t size, uint8_t* outData) {
		auto b = reinterpret_cast<uint8_t*>(&(*begin()));
		std::copy(b + address, b + address + size, outData);
	}
	virtual uint8_t* Map(uint32_t address, uint32_t size) {
		auto b = reinterpret_cast<uint8_t*>(&(*begin()));
		return b+address;
	}


private:
	uint8_t RNO(Register r);

	Assembly& load(const char* which, Register rs, Register rt, uint16_t imm);
};
