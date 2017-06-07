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
	constexpr size_t memory_size = 0x200000;
	constexpr uint32_t code_base_addr = 0xf0000000;
	auto memory = std::make_shared<MemoryBlock<memory_size>>();
	auto code = std::make_shared<Assembly>();

	/*
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
		*/
	/*
		0  : 27bdfff8        addiu   sp, sp, -8
		4  : afbe0004        sw      s8, 4(sp)
		8  : 03a0f025        move    s8, sp
		c  : 8c810000        lw      at, 0(a0)
		10 : 8c82000c        lw      v0, 12(a0)
		14 : 00410821        addu    at, v0, at
		18 : ac810008        sw      at, 8(a0)
		1c : 24220001        addiu   v0, at, 1
		20 : 03c0e825        move    sp, s8
		24 : 8fbe0004        lw      s8, 4(sp)
		28 : 03e00008        jr      ra
		2c : 27bd0008        addiu   sp, sp, 8
		*/
	(*code)
		.label("begin")
		.nop()
		.addiu(R::SP, R::SP, -8) //allocate stack space
		.sw(R::FP, R::SP, 4) // save frame pointer
		.move(R::FP, R::SP) // save stack pointer
		.lw(R::AT, R::A0, 0)  // at = arg0[0];
		.lw(R::V0, R::A0, 12) // v0 = arg0[3];
		.addu(R::AT, R::V0, R::AT) // at+=v0;
		.sw(R::AT, R::A0, 8) // arg0[2] = at;
		.addiu(R::V0, R::AT, 1) // v0 = at+1;
		.move(R::SP, R::FP) //restore stack pointer
		.lw(R::FP, R::SP, 4) //restore frame pointer
		.jr(R::RA) //return from function (executes next first)
		.addiu(R::SP, R::SP, 8) // deallocate stack space
		.nop()
	;
	(*code)
		.label("main")
		.nop()
		.li(R::SP, uint32_t(memory_size - 4))
		.li(R::FP, uint32_t(memory_size))
		.jal((code->labelAddr("begin") + code_base_addr) / 4)
		.li(R::A0, 0x0)
		.sw(R::V0, R(0), 12)
		;

	auto mmap = std::make_shared<MemoryMap>();
	mmap->add_map(memory, 0, memory_size, 0, 0);
	mmap->add_map(code, code_base_addr, code->size()*4, 0, 0);
	Cpu cpu(mmap,nullptr,nullptr,nullptr,nullptr);
	cpu.Reset(code_base_addr+code->labelAddr("main"));

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

