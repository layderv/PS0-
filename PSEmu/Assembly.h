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
	Assembly& addiu(Register rt, Register rs, int16_t imm);

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

	// Secondary OPCodes (when (op >> 26) == 0)
	/*
		  00h=SLL   08h=JR      10h=MFHI 18h=MULT  20h=ADD  28h=N/A  30h=N/A  38h=N/A
		  01h=N/A   09h=JALR    11h=MTHI 19h=MULTU 21h=ADDU 29h=N/A  31h=N/A  39h=N/A
		  02h=SRL   0Ah=N/A     12h=MFLO 1Ah=DIV   22h=SUB  2Ah=SLT  32h=N/A  3Ah=N/A
		  03h=SRA   0Bh=N/A     13h=MTLO 1Bh=DIVU  23h=SUBU 2Bh=SLTU 33h=N/A  3Bh=N/A
		  04h=SLLV  0Ch=SYSCALL 14h=N/A  1Ch=N/A   24h=AND  2Ch=N/A  34h=N/A  3Ch=N/A
		  05h=N/A   0Dh=BREAK   15h=N/A  1Dh=N/A   25h=OR   2Dh=N/A  35h=N/A  3Dh=N/A
		  06h=SRLV  0Eh=N/A     16h=N/A  1Eh=N/A   26h=XOR  2Eh=N/A  36h=N/A  3Eh=N/A
		  07h=SRAV  0Fh=N/A     17h=N/A  1Fh=N/A   27h=NOR  2Fh=N/A  37h=N/A  3Fh=N/A
	*/

	Assembly& sll(Register rd, Register rt, uint8_t imm);
	Assembly& srl(Register rd, Register rt, uint8_t imm);
	Assembly& sra(Register rd, Register rt, uint8_t imm);
	Assembly& sllv(Register rd, Register rt, uint8_t imm);
	Assembly& srlv(Register rd, Register rt, uint8_t imm);
	Assembly& srav(Register rd, Register rt, uint8_t imm);

	Assembly& jr(Register rs);
	Assembly& jalr(Register rs, Register rd);

	Assembly& sys(uint32_t comm);
	Assembly& brk(uint32_t comm);
	
	Assembly& mfhi(Register rd);
	Assembly& mthi(Register rs);
	Assembly& mflo(Register rd);
	Assembly& mtlo(Register rs);

	Assembly& mult(Register rs, Register rt);
	Assembly& multu(Register rs, Register rt);
	Assembly& div(Register rs, Register rt);
	Assembly& divu(Register rs, Register rt);
	Assembly& add(Register rd, Register rs, Register rt);
	Assembly& addu(Register rd, Register rs, Register rt);
	Assembly& sub(Register rd, Register rs, Register rt);
	Assembly& subu(Register rd, Register rs, Register rt);
	Assembly& And(Register rd, Register rs, Register rt);
	Assembly& Or(Register rd, Register rs, Register rt);
	Assembly& Xor(Register rd, Register rs, Register rt);
	Assembly& nor(Register rd, Register rs, Register rt);
	Assembly& slt(Register rd, Register rs, Register rt);
	Assembly& sltu(Register rd, Register rs, Register rt);



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
