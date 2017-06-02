#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <array>

#include "Coprocessor.h"
#include "MemoryMap.h"

class Cpu
{
	using instruction_code = uint32_t;
	std::array < std::unique_ptr<Coprocessor>,4 >  coprocessors;
	std::array < uint32_t ,34 > registers;
	uint32_t programCounter;
	std::shared_ptr<MemoryMap> memory;
	uint32_t next_instruction;

	bool check_overflow(int64_t val);
public:
	Cpu(std::shared_ptr<MemoryMap> baseMemory, 
		std::unique_ptr<Coprocessor>&& cop0,
		std::unique_ptr<Coprocessor>&& cop1,
		std::unique_ptr<Coprocessor>&& cop2,
		std::unique_ptr<Coprocessor>&& cop3);
	~Cpu();

	void Reset(uint32_t pc);
	void Clock();
	Coprocessor& GetCoprocessor(int copNumber);

	void DebugSetReg(int number, uint32_t value);
	void DebugSetPC(uint32_t value);
	uint32_t DebugGetReg(int number);
	uint32_t DebugGetPC();

};

