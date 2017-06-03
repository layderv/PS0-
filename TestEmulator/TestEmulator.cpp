// TestEmulator.cpp : Defines the entry point for the console application.
//
#include "Cpu.h"
#include "MemoryMap.h"
#include "MemoryDevice.h"
#include "Registers.h"
#include "Assembly.h"

using R = Register;

int main()
{
	constexpr size_t memory_size = 1024 * 1024 * 2;
	constexpr uint32_t code_base_addr = 0xf0000000;
	auto memory = std::make_shared<MemoryBlock<memory_size>>();
	auto code = std::make_shared<Assembly>();

	(*code)
		.nop()
		.lw(R(1), R(0), 0)
		.lw(R(2), R(0), 4)
		.lb(R(3), R(0), 8)
		.lbu(R(4), R(0), 8)
		.lh(R(5), R(0), 4)
		.lhu(R(6), R(0), 4)
		.jal(0)
		;

	auto mmap = std::make_shared<MemoryMap>();
	mmap->add_map(memory, 0, memory_size, 0, 0);
	mmap->add_map(code, code_base_addr, code->size()*4, 0, 0);
	Cpu cpu(mmap,nullptr,nullptr,nullptr,nullptr);
	cpu.Reset(code_base_addr);

	mmap->store32(0, 0xfefefefe);
	mmap->store32(4, 0x10cacca);
	mmap->store32(8, 0xff0000ee);


	while (1) {
		cpu.Clock();
		printf("Program Counter: %#010x %#010x\n", cpu.DebugGetPC(), cpu.DebugGetPC() - code_base_addr);
		printf("%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n",
			cpu.DebugGetReg(R(0)),
			cpu.DebugGetReg(R(1)),
			cpu.DebugGetReg(R(2)),
			cpu.DebugGetReg(R(3)),
			cpu.DebugGetReg(R(4)),
			cpu.DebugGetReg(R(5)),
			cpu.DebugGetReg(R(6)),
			cpu.DebugGetReg(R(7))
		);
		printf("%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n",
			cpu.DebugGetReg(R(8)),
			cpu.DebugGetReg(R(9)),
			cpu.DebugGetReg(R(10)),
			cpu.DebugGetReg(R(11)),
			cpu.DebugGetReg(R::GP),
			cpu.DebugGetReg(R::FP),
			cpu.DebugGetReg(R::SP),
			cpu.DebugGetReg(R::RA)
		);
		printf("%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n",
			mmap->read32(0),
			mmap->read32(4),
			mmap->read32(8),
			mmap->read32(12),
			mmap->read32(16),
			mmap->read32(20),
			mmap->read32(24),
			mmap->read32(28)
		);
		printf("\n");
	}

    return 0;
}

