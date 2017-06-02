#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Coprocessor.h"
#include "MemoryMap.h"

class Cpu
{
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

