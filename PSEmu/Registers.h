#pragma once
#include <cstdint>
/*

Name       Alias    Common Usage
(R0)       zero     Constant (always 0) (this one isn't a real register)
R1         at       Assembler temporary (destroyed by some pseudo opcodes!)
R2-R3      v0-v1    Subroutine return values, may be changed by subroutines
R4-R7      a0-a3    Subroutine arguments, may be changed by subroutines
R8-R15     t0-t7    Temporaries, may be changed by subroutines
R16-R23    s0-s7    Static variables, must be saved by subs
R24-R25    t8-t9    Temporaries, may be changed by subroutines
R26-R27    k0-k1    Reserved for kernel (destroyed by some IRQ handlers!)
R28        gp       Global pointer (rarely used)
R29        sp       Stack pointer
R30        fp(s8)   Frame Pointer, or 9th Static variable, must be saved
R31        ra       Return address (used so by JAL,BLTZAL,BGEZAL opcodes)
-          pc       Program counter
-          hi,lo    Multiply/divide results, may be changed by subroutines
*/

enum struct Register : uint8_t {
	R00,
	R01,
	R02,
	R03,
	R04,
	R05,
	R06,
	R07,
	R08,
	R09,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	R16,
	R17,
	R18,
	R19,
	R20,
	R21,
	R22,
	R23,
	R24,
	R25,
	R26,
	R27,
	R28,
	R29,
	R30,
	R31,
	ZERO = R00,
	AT = R01,
	V0 = R02,
	V1 = R03,
	A0 = R04,
	A1 = R05,
	A2 = R06,
	A3 = R07,
	SP = R29,
	FP = R30,
	RA = R31,
	GP = R28
};