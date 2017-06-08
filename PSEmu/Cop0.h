#pragma once
#include <array>
// Coprocessor 0 handles exception and irq
class Cpu;
class Cop0
{
	Cpu& cpu;
	// processor ID
	const uint32_t r_procid = 0;
	//breakpoint
	uint32_t r_bpc, r_bpcm;
	uint32_t r_bda, r_bdam;
	uint32_t r_dcic;
	//exception
	static const uint32_t eh_reset_addr = 0xBFC00000;
	static const uint32_t eh_rom_addr   = 0xBFC00100;
	static const uint32_t eh_ram_addr   = 0x80000000;
	uint32_t r_badaddr{ 0 };
	uint32_t r_epc{ 0 };
	union {
		uint32_t raw;
		struct {
			bool z0 : 2; // 0-1
			unsigned int excode : 5; //2-6
			bool z1 : 1; // 7
			uint8_t Ip : 8; //8-15 interrupt pending 
			bool z2 : 12; // 16-27
			unsigned int CE : 2; // 28-29 coprocessor exception bit
			bool z3 : 1; //30
			bool BD : 1; //31 branch delay bit
		};
	} r_cause;
	union {
		uint32_t raw;
		struct {
			bool IEc : 1; //interrupt enable current
			bool KUc : 1; //kernel/user mode
			bool IEp : 1; //previous
			bool KUp : 1;
			bool IEo : 1; //old
			bool KUo : 1;
			bool z0 : 2;
			uint8_t Im : 8; //interrupt mask
			bool Isc : 1; // isolate cache
			bool Swc : 1; // swap cache
			bool PZ : 1; // cache parity bit set to zero
			bool CM : 1; // set if isolated cache match memory
			bool PE : 1; // cache parity error
			bool TS : 1; // TLB shutdown
			bool BEV : 1; // boot exception vectors ram/rom
			bool z1 : 2;
			bool RE : 1; // reverse endiannness -> read and write from memory are big endian
			bool z2 : 2;
			// coprocessor enable. For CU0, disabled means enabled only in kernel mode
			bool CU0 : 1;
			bool CU1 : 1;
			bool CU2 : 1;
			bool CU3 : 1;
		};
	} r_sys;

	uint32_t read_reg(int number)
	{
		switch (number){
		case 0:
		case 1:
		case 2:
			break;
		case 3: // breakpoint on execute
			return r_bpc;
		case 4:
			break;
		case 5: // breakpoint on data access
			return r_bda;
		case 6: // JUMPDEST register
			return 0x0;
		case 7: // breakpoint control
			return r_dcic;
		case 8: // page fault register (no memory)
			return r_badaddr;
		case 9: // breakpoint on data access mask
			return r_bdam;
		case 10:
			break;
		case 11: // breakpoint on execute mask
			return r_bpcm;
		case 12: // system status
			return r_sys.raw;
		case 13: // exception cause
			return r_cause.raw;
		case 14: // exception return address
			return r_epc;
		case 15: // processor_id
			return r_procid;
		}
	}
public:
	/*
	  2-6   Excode Describes what kind of exception occured:
                 00h INT     Interrupt
                 01h MOD     Tlb modification (none such in PSX)
                 02h TLBL    Tlb load         (none such in PSX)
                 03h TLBS    Tlb store        (none such in PSX)
                 04h AdEL    Address error, Data load or Instruction fetch
                 05h AdES    Address error, Data store
                             The address errors occur when attempting to read
                             outside of KUseg in user mode and when the address
                             is misaligned. (See also: BadVaddr register)
                 06h IBE     Bus error on Instruction fetch
                 07h DBE     Bus error on Data load/store
                 08h Syscall Generated unconditionally by syscall instruction
                 09h BP      Breakpoint - break instruction
                 0Ah RI      Reserved instruction
                 0Bh CpU     Coprocessor unusable
                 0Ch Ov      Arithmetic overflow
                 0Dh-1Fh     Not used
	*/
	/// returns jump target
	uint32_t raise_exception(uint32_t current_instruction_addr, uint8_t ExcCode, uint32_t param0);
};

