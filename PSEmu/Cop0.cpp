#include "Cop0.h"
#include "Cpu.h"
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

uint32_t Cop0::raise_exception(uint32_t current_instruction_addr, uint8_t ExcCode, uint32_t param0)
{
	r_sys.IEo = r_sys.IEp;
	r_sys.KUo = r_sys.KUp;
	r_sys.IEp = r_sys.IEc;
	r_sys.KUp = r_sys.KUc;
	//disable interrupt, go to kernel mode
	r_sys.IEc = r_sys.IEp = false;
	bool delay_slot = current_instruction_addr & 1;
	current_instruction_addr &= ~1;
	if (delay_slot) //if delay slot
	{
		r_cause.BD = true;
		r_epc = current_instruction_addr - 4;
	}
	else
	{
		r_epc = current_instruction_addr;
	}
	switch (r_cause.excode = ExcCode)
	{
	case 11: // coprocessor unusable
		r_cause.CE = param0;
		break;
	case 4: // address error (load/fetch)
	case 5: // address error (store)
		r_badaddr = param0;
		break;
	}
	//jump to exception table
	return (r_sys.BEV ? eh_rom_addr: eh_ram_addr)+0x80;

}
