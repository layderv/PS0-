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

	/*
		  00h=SPECIAL 08h=ADDI  10h=COP0 18h=N/A   20h=LB   28h=SB   30h=LWC0 38h=SWC0
		  01h=BcondZ  09h=ADDIU 11h=COP1 19h=N/A   21h=LH   29h=SH   31h=LWC1 39h=SWC1
		  02h=J       0Ah=SLTI  12h=COP2 1Ah=N/A   22h=LWL  2Ah=SWL  32h=LWC2 3Ah=SWC2
		  03h=JAL     0Bh=SLTIU 13h=COP3 1Bh=N/A   23h=LW   2Bh=SW   33h=LWC3 3Bh=SWC3
		  04h=BEQ     0Ch=ANDI  14h=N/A  1Ch=N/A   24h=LBU  2Ch=N/A  34h=N/A  3Ch=N/A
		  05h=BNE     0Dh=ORI   15h=N/A  1Dh=N/A   25h=LHU  2Dh=N/A  35h=N/A  3Dh=N/A
		  06h=BLEZ    0Eh=XORI  16h=N/A  1Eh=N/A   26h=LWR  2Eh=SWR  36h=N/A  3Eh=N/A
		  07h=BGTZ    0Fh=LUI   17h=N/A  1Fh=N/A   27h=N/A  2Fh=N/A  37h=N/A  3Fh=N/A

	*/

	Assembly& bltz(Register rs, uint16_t dest);
	Assembly& bgez(Register rs, uint16_t dest);
	Assembly& bltzal(Register rs, uint16_t dest);
	Assembly& bgezal(Register rs, uint16_t dest);

	Assembly& j(uint32_t imm26);
	Assembly& jal(uint32_t imm26);

	Assembly& beq(Register rs, Register rt, uint16_t dest);
	Assembly& bne(Register rs, Register rt, uint16_t dest);

	Assembly& blez(Register rs, Register rt, uint16_t dest);
	Assembly& bgtz(Register rs, Register rt, uint16_t dest);

	Assembly& addi(Register rt, Register rs, uint16_t imm);
	Assembly& addiu(Register rt, Register rs, uint16_t imm);

	Assembly& slti(Register rt, Register rs, uint16_t imm);
	Assembly& sltiu(Register rt, Register rs, uint16_t imm);

	Assembly& andi(Register rt, Register rs, uint16_t imm);
	Assembly& ori(Register rt, Register rs, uint16_t imm);

	Assembly& xori(Register rt, Register rs, uint16_t imm);
	Assembly& lui(Register rt, uint16_t imm);

	Assembly& cop0(Register rs, Register rt, uint16_t imm);
	Assembly& cop1(Register rs, Register rt, uint16_t imm);
	Assembly& cop2(Register rs, Register rt, uint16_t imm);
	Assembly& cop3(Register rs, Register rt, uint16_t imm);

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

	// COP
	Assembly& lwc0(Register rs, Register rt, uint16_t imm);
	Assembly& lwc1(Register rs, Register rt, uint16_t imm);
	Assembly& lwc2(Register rs, Register rt, uint16_t imm);
	Assembly& lwc3(Register rs, Register rt, uint16_t imm);

	Assembly& swc0(Register rs, Register rt, uint16_t imm);
	Assembly& swc1(Register rs, Register rt, uint16_t imm);
	Assembly& swc2(Register rs, Register rt, uint16_t imm);
	Assembly& swc3(Register rs, Register rt, uint16_t imm);




	


	// MemoryDevice
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
};
