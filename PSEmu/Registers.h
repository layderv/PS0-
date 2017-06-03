#pragma once
#include <cstdint>

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
	AT=R01,
	SP=R29,
	FP=R30,
	RA=R31,
	GP=R28
};