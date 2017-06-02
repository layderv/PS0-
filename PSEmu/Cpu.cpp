#include "Cpu.h"


Cpu::Cpu(std::shared_ptr<MemoryMap> baseMemory, std::unique_ptr<Coprocessor>&& cop0, std::unique_ptr<Coprocessor>&& cop1, std::unique_ptr<Coprocessor>&& cop2, std::unique_ptr<Coprocessor>&& cop3)
	:memory(baseMemory),coprocessors{std::move(cop0),std::move(cop1),std::move(cop2),std::move(cop3)}
{
}

Cpu::~Cpu()
{
}

void Cpu::Reset(uint32_t pc)
{
	programCounter = pc;
	for (auto &r : registers) {
		r = 0;
	}
}

bool Cpu::check_overflow(int64_t val) {
	if (val > 0xffffffff || val + 0xffffffff < 0) {
		//todo: set overflow flag
		return true;
	}
	return false;

}
void Cpu::Clock()
{
	// TODO: check endianness
	uint8_t primary_opcode;
	uint8_t param1;
	uint8_t param2;
	uint8_t param3;
	uint8_t param4;
	uint8_t secondary_opcode;
	uint16_t imm16;
	uint32_t imm26;
	uint32_t comment;

	// Fetch:
	auto current_instruction = memory->read32(programCounter);
	// Decode:
	uint32_t v = current_instruction;
	secondary_opcode = v & 0x3f;
	imm16 = v & 0xffff;
	imm26 = v & 0x3ffffff;
	v >>= 6;
	param4 = v & 0x1f;
	comment = v & 0xfffff;
	v >>= 5;
	param3 = v & 0x1f;
	v >>= 5;
	param2 = v & 0x1f;
	v >>= 5;
	param1 = v & 0x1f;
	v >>= 5;
	primary_opcode = v & 0x3f;
	v >>= 6;
	//assert(v == 0);

	// Exec:
	switch (primary_opcode) {

	// Loads
	case 0x20: //lb
		auto val = memory->read8(registers[param1] + imm16);
		reinterpret_cast<int32_t&>(registers[param2]) =	reinterpret_cast<int8_t&>(val);
		break;
	case 0x21: //lh
		auto val = memory->read16(registers[param1] + imm16);
		reinterpret_cast<int32_t&>(registers[param2]) = reinterpret_cast<int16_t&>(val);
		break;
	case 0x23: //lw
		registers[param2] = memory->read32(registers[param1] + imm16);
		break;
	case 0x24: //lbu
		registers[param2] = memory->read8(registers[param1] + imm16);
		break;
	case 0x25: //lhu
		registers[param2] = memory->read16(registers[param1] + imm16);
		break;
	case 0x22: //lwl
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// load left(upper) bytes of reg
		memory->read(addr * 4, 1 + left, reg+(3-left));
		break;

	case 0x26: //lwr
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// load right(lower) bytes of reg
		memory->read(addr * 4 + left, 4 - left, reg);
		break;

	// Stores
	case 0x28: //sb
		memory->store8(registers[param1] + imm16, reinterpret_cast<uint8_t*>(&registers[param2])[0]);
		break;
	case 0x29: //sh
		memory->store16(registers[param1] + imm16, reinterpret_cast<uint16_t*>(&registers[param2])[0]);
		break;
	case 0x2b: //sw
		memory->store32(registers[param1] + imm16, registers[param2]);
		break;
	case 0x2a: //swl
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// store left(upper) bytes of reg
		memory->store(addr * 4, 1 + left, reg + (3 - left));
		break;
	case 0x2e: //swr
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// store right(lower) bytes of reg
		memory->store(addr * 4 + left, 4 - left, reg);
		break;

	//Arithmetic
	case 0x08: //addi
		int64_t result = static_cast<int64_t> (registers[param1]) + reinterpret_cast<int16_t const&>(imm16);
		if (!check_overflow(result))
			registers[param2] = static_cast<uint32_t> (result);
		break;
		// rs rt
	case 0x09: //addiu
		// rs rt
		int64_t result = static_cast<int64_t> (registers[param1]) + +reinterpret_cast<int16_t const&>(imm16);
		registers[param2] = static_cast<uint32_t> (result);
		break;

	//Special:
	case 0x00:
		switch (secondary_opcode) {
			//Arithmetic
		case 0x20: //add
			int64_t result = static_cast<int64_t> (registers[param1]) + static_cast<int64_t> (registers[param2]);
			if (!check_overflow(result))
				registers[param3] = static_cast<uint32_t>(result);
			break;
		case 0x21: //addu
			int64_t result = static_cast<int64_t> (registers[param1]) + static_cast<int64_t> (registers[param2]);
			registers[param3] = static_cast<uint32_t>(result);
			break;
		case 0x22: //sub
			int64_t result = static_cast<int64_t> (registers[param1]) - static_cast<int64_t> (registers[param2]);
			if (!check_overflow(result))
				registers[param3] = static_cast<uint32_t>(result);
			break;
		case 0x23: //subu
			int64_t result = static_cast<int64_t> (registers[param1]) - static_cast<int64_t> (registers[param2]);
			registers[param3] = static_cast<uint32_t>(result);
			break;
		}
		break;
	}

}

Coprocessor & Cpu::GetCoprocessor(int copNumber)
{
	// TODO: insert return statement here
}

void Cpu::DebugSetReg(int number, uint32_t value)
{
}

void Cpu::DebugSetPC(uint32_t value)
{
}

uint32_t Cpu::DebugGetReg(int number)
{
	return uint32_t();
}

uint32_t Cpu::DebugGetPC()
{
	return uint32_t();
}
