#include "Cpu.h"


Cpu::Cpu(std::shared_ptr<MemoryMap> baseMemory, std::unique_ptr<Coprocessor>&& cop0, std::unique_ptr<Coprocessor>&& cop1, std::unique_ptr<Coprocessor>&& cop2, std::unique_ptr<Coprocessor>&& cop3)
{
}

Cpu::~Cpu()
{
}

void Cpu::Reset(uint32_t pc)
{
}

void Cpu::Clock()
{
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
