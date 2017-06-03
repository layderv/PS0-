#include "Cpu.h"


Cpu::Cpu(std::shared_ptr<MemoryMap> baseMemory, std::unique_ptr<Coprocessor>&& cop0, std::unique_ptr<Coprocessor>&& cop1, std::unique_ptr<Coprocessor>&& cop2, std::unique_ptr<Coprocessor>&& cop3)
	:coprocessors{ { std::move(cop0),std::move(cop1),std::move(cop2),std::move(cop3) } }
	, memory(baseMemory)
{
}

Cpu::~Cpu()
{
}

void Cpu::Reset(uint32_t pc)
{
	programCounter = (pc/4)*4;
	next_instruction = 0;// memory->read32(programCounter);
//	programCounter += 4;
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
	//fetch is pipelined. If program counter changes, the next instruction has already been fetched
	auto current_instruction = next_instruction;
	next_instruction = memory->read32(programCounter + 4);

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

		////////////
		// Loads
		////////////
	case 0x20: //lb
	{
		auto val = memory->read8(registers[param1] + imm16);
		reinterpret_cast<int32_t&>(registers[param2]) = reinterpret_cast<int8_t&>(val);
		break;
	}
	case 0x21: //lh
	{
		auto val = memory->read16(registers[param1] + imm16);
		reinterpret_cast<int32_t&>(registers[param2]) = reinterpret_cast<int16_t&>(val);
		break;
	}
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
	{
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// load left(upper) bytes of reg
		memory->read(addr * 4, 1 + left, reg + (3 - left));
		break;
	}
	case 0x26: //lwr
	{
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// load right(lower) bytes of reg
		memory->read(addr * 4 + left, 4 - left, reg);
		break;
	}
		////////////
		// Stores
		////////////

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
	{
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// store left(upper) bytes of reg
		memory->store(addr * 4, 1 + left, reg + (3 - left));
		break;
	}
	case 0x2e: //swr
	{
		uint32_t addr = (registers[param1] + imm16) / 4;
		uint32_t left = (registers[param1] + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&registers[param2]);
		// store right(lower) bytes of reg
		memory->store(addr * 4 + left, 4 - left, reg);
		break;
	}

		////////////
		//Arithmetic
		////////////

	case 0x08: //addi
	{
		int64_t result = static_cast<int64_t> (registers[param1]) + reinterpret_cast<int16_t const&>(imm16);
		if (!check_overflow(result))
			registers[param2] = static_cast<uint32_t> (result);
		break;
	}
	case 0x09: //addiu
	{
		int64_t result = static_cast<int64_t> (registers[param1]) + +reinterpret_cast<int16_t const&>(imm16);
		registers[param2] = static_cast<uint32_t> (result);
		break;
	}
		////////////]
		//Jumps
		////////////

	case 0x02: //j
		programCounter &= 0xf0000000;
		programCounter += imm26 * 4;
		break;
	case 0x03: //jal
		registers[31] = programCounter + 8;
		programCounter &= 0xf0000000;
		programCounter += imm26 * 4;
		break;
	case 0x04: //beq
		if (registers[param1] == registers[param2])
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x05: //bne
		if (registers[param1] != registers[param2])
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x07: //bgtz
		if (reinterpret_cast<int32_t&>(registers[param1]) > 0)
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x06: //blez
		if (reinterpret_cast<int32_t&>(registers[param1]) <= 0)
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x01: //other jumps
		switch (param2) {
		case 0x00: //bltz
			if (reinterpret_cast<int32_t&>(registers[param1]) < 0)
				programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
			break;
		case 0x01: //bgez
			if (reinterpret_cast<int32_t&>(registers[param1]) >= 0)
				programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
			break;
		case 0x10: //bltzal
			if (reinterpret_cast<int32_t&>(registers[param1]) < 0) {
				registers[31] = programCounter + 8;
				programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
			}
			break;
		case 0x11: //bgezal
			if (reinterpret_cast<int32_t&>(registers[param1]) >= 0) {
				registers[31] = programCounter + 8;
				programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
			}
			break;
		}
		break;

		////////////
		// Special
		////////////

	case 0x00:
		switch (secondary_opcode) {
			////////////
			//Special Arithmetic
			////////////

		case 0x20: //add
		{
			int64_t result = static_cast<int64_t> (registers[param1]) + static_cast<int64_t> (registers[param2]);
			if (!check_overflow(result))
				registers[param3] = static_cast<uint32_t>(result);
			break;
		}
		case 0x21: //addu
		{
			int64_t result = static_cast<int64_t> (registers[param1]) + static_cast<int64_t> (registers[param2]);
			registers[param3] = static_cast<uint32_t>(result);
			break;
		}
		case 0x22: //sub
		{
			int64_t result = static_cast<int64_t> (registers[param1]) - static_cast<int64_t> (registers[param2]);
			if (!check_overflow(result))
				registers[param3] = static_cast<uint32_t>(result);
			break;
		}
		case 0x23: //subu
		{
			int64_t result = static_cast<int64_t> (registers[param1]) - static_cast<int64_t> (registers[param2]);
			registers[param3] = static_cast<uint32_t>(result);
			break;
		}
			////////////
			//Special Jumps
			////////////
		case 0x08: //jr
			programCounter = registers[param1];
			break;
		case 0x09: //jalr
			registers[param3] = programCounter;
			programCounter = registers[param1];
			break;

			////////////
			//Exceptions/debug
			////////////

		case 0x0c: //syscall
			// TODO syscall exception
			break;
		case 0x0d: //break
			// TODO breakpoint exception
			break;
		}
		break;
	}
	programCounter += 4;
}

Coprocessor & Cpu::GetCoprocessor(int copNumber)
{
	return *coprocessors[copNumber];
	// TODO: insert return statement here
}

void Cpu::DebugSetReg(Register number, uint32_t value)
{
	registers[uint8_t(number)] = value;
}

void Cpu::DebugSetPC(uint32_t value)
{
	programCounter = value;
}

uint32_t Cpu::DebugGetReg(Register number)
{
	return registers[uint8_t(number)];
}

uint32_t Cpu::DebugGetPC()
{
	return programCounter;
}
