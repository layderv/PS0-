#pragma once

struct instruction {
	union { // 0-31
		uint32_t raw;
		struct {
			unsigned int code : 6;
			unsigned int p1 : 5;
			unsigned int p2 : 5;
			unsigned int p3 : 5;
			unsigned int p4 : 5;
			unsigned int xcode : 6;
		};
		struct {
			unsigned int : 6;
			unsigned int imm26 : 26;
		};
		struct {
			unsigned int : 16;
			unsigned int imm16 : 16;
		};
		struct {
			unsigned int : 16;
			unsigned int immi16 : 16;
		};
	};
	uint32_t address;
};
static_assert(sizeof(instruction) == 8, "too big, packing wrong");

struct comp_result {
	uint32_t res;
	uint32_t exc;
	constexpr comp_result(uint32_t res) :res(res), exc(0) {}
	constexpr comp_result(uint32_t res, uint32_t exc) : res(res), exc(exc) {}
};