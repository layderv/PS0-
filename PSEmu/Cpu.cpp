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
	programCounter = (pc / 4) * 4;
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
	uint32_t &rs = (&registers[0])[param1];
	uint32_t &rt = (&registers[0])[param2];
	uint32_t &rd = (&registers[0])[param3];
	int32_t &rsi = reinterpret_cast<int32_t&>(rs);
	int32_t &rti = reinterpret_cast<int32_t&>(rt);
	int32_t &rdi = reinterpret_cast<int32_t&>(rd);
	//assert(v == 0);

	// Exec:
	switch (primary_opcode) {

		////////////
		// Loads
		////////////
	case 0x20: //lb
	{
		auto val = memory->read8(rs + imm16);
		reinterpret_cast<int32_t&>(rt) = reinterpret_cast<int8_t&>(val);
		break;
	}
	case 0x21: //lh
	{
		auto val = memory->read16(rs + imm16);
		reinterpret_cast<int32_t&>(rt) = reinterpret_cast<int16_t&>(val);
		break;
	}
	case 0x23: //lw
		rt = memory->read32(rs + imm16);
		break;
	case 0x24: //lbu
		rt = memory->read8(rs + imm16);
		break;
	case 0x25: //lhu
		rt = memory->read16(rs + imm16);
		break;
	case 0x22: //lwl
	{
		uint32_t addr = (rs + imm16) / 4;
		uint32_t left = (rs + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&rt);
		// load left(upper) bytes of reg
		memory->read(addr * 4, 1 + left, reg + (3 - left));
		break;
	}
	case 0x26: //lwr
	{
		uint32_t addr = (rs + imm16) / 4;
		uint32_t left = (rs + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&rt);
		// load right(lower) bytes of reg
		memory->read(addr * 4 + left, 4 - left, reg);
		break;
	}
	////////////
	// Stores
	////////////

	case 0x28: //sb
		memory->store8(rs + imm16, reinterpret_cast<uint8_t*>(&rt)[0]);
		break;
	case 0x29: //sh
		memory->store16(rs + imm16, reinterpret_cast<uint16_t*>(&rt)[0]);
		break;
	case 0x2b: //sw
		memory->store32(rs + imm16, rt);
		break;
	case 0x2a: //swl
	{
		uint32_t addr = (rs + imm16) / 4;
		uint32_t left = (rs + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&rt);
		// store left(upper) bytes of reg
		memory->store(addr * 4, 1 + left, reg + (3 - left));
		break;
	}
	case 0x2e: //swr
	{
		uint32_t addr = (rs + imm16) / 4;
		uint32_t left = (rs + imm16) % 4;
		uint8_t *reg = reinterpret_cast<uint8_t*>(&rt);
		// store right(lower) bytes of reg
		memory->store(addr * 4 + left, 4 - left, reg);
		break;
	}

	////////////
	//Arithmetic
	////////////

	case 0x08: //addi
	{
		int64_t result = static_cast<int64_t> (rs) + reinterpret_cast<int16_t const&>(imm16);
		if (!check_overflow(result))
			rt = static_cast<uint32_t> (result);
		break;
	}
	case 0x09: //addiu
	{
		int64_t result = static_cast<int64_t> (rs) + +reinterpret_cast<int16_t const&>(imm16);
		rt = static_cast<uint32_t> (result);
		break;
	}


	////////////
	//Comparison
	////////////

	case 0x0a: //slti
	{
		int32_t rsi = reinterpret_cast<int32_t&>(rs);
		int32_t val = reinterpret_cast<int16_t&>(imm16);
		rt = rsi > val ? 1 : 0;
	}
	case 0x0b: //sltiu
	{
		uint32_t val = imm16;
		rt = rs > val ? 1 : 0;
	}


	////////////
	//Logical
	////////////
	case 0x0c: //andi
		rt=rs&imm16;
	case 0x0d: //ori
		rt = rs|imm16;
	case 0x0e: //xori
		rt = rs^imm16;

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
		if (rs == rt)
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x05: //bne
		if (rs != rt)
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x07: //bgtz
		if (reinterpret_cast<int32_t&>(rs) > 0)
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x06: //blez
		if (reinterpret_cast<int32_t&>(rs) <= 0)
			programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
		break;
	case 0x01: //other jumps
		switch (param2) {
		case 0x00: //bltz
			if (reinterpret_cast<int32_t&>(rs) < 0)
				programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
			break;
		case 0x01: //bgez
			if (reinterpret_cast<int32_t&>(rs) >= 0)
				programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
			break;
		case 0x10: //bltzal
			if (reinterpret_cast<int32_t&>(rs) < 0) {
				registers[31] = programCounter + 8;
				programCounter += 4 + reinterpret_cast<int16_t&>(imm16) * 4;
			}
			break;
		case 0x11: //bgezal
			if (reinterpret_cast<int32_t&>(rs) >= 0) {
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
			int64_t result = static_cast<int64_t> (rs) + static_cast<int64_t> (rt);
			if (!check_overflow(result))
				rd = static_cast<uint32_t>(result);
			break;
		}
		case 0x21: //addu
		{
			int64_t result = static_cast<int64_t> (rs) + static_cast<int64_t> (rt);
			rd = static_cast<uint32_t>(result);
			break;
		}
		case 0x22: //sub
		{
			int64_t result = static_cast<int64_t> (rs) - static_cast<int64_t> (rt);
			if (!check_overflow(result))
				rd = static_cast<uint32_t>(result);
			break;
		}
		case 0x23: //subu
		{
			int64_t result = static_cast<int64_t> (rs) - static_cast<int64_t> (rt);
			rd = static_cast<uint32_t>(result);
			break;
		}

		////////////
		//Special Comparison
		////////////

		case 0x2a: //slt
		{
			int32_t rsi = reinterpret_cast<int32_t&>(rs);
			int32_t val = reinterpret_cast<int16_t&>(imm16);
			rt = rsi > val ? 1 : 0;
			break;
		}
		case 0x2b: //sltu
		{
			uint32_t val = imm16;
			rt = rs > val ? 1 : 0;
			break;
		}

		////////////
		//Special Logical
		////////////
		case 0x24: //and
			rd = rs & rt;
			break;
		case 0x25: //or
			rd = rs | rt;
			break;
		case 0x26: //xor
			rd = rs ^ rt;
			break;
		case 0x27: //nor
			rd = ~(rs | rt);
			break;


		////////////
		//Special Jumps
		////////////

		case 0x08: //jr
			programCounter = rs;
			break;
		case 0x09: //jalr
		{
			rd = programCounter;
			programCounter = rs;
			break;
		}
		
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
