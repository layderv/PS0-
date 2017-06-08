#pragma once
#include <cstdint>
#include "MemoryMap.h"

struct MemoryOpcodes {
	static comp_result Lb(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		return u(m.readi8(addr));
	}
	static comp_result Lbu(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		return m.read8(addr);
	}
	static comp_result Lh(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		if (addr % 2) return bad_addrl(addr); return u(m.readi16(addr));
	}
	static comp_result Lhu(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		if (addr % 2) return bad_addrl(addr); return m.read16(addr);
	}
	static comp_result Lw(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		 if (addr % 4) return bad_addrl(addr); return m.read32(addr);
	}
	static comp_result Lw(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		if (addr % 4) return bad_addrl(addr); return m.read32(addr);
	}
	// left = most significant part. Semantic of lwl and lwr changes with endianness
	// This implementation assumes little endian memory, thus loading a word is lwl(addr+3), lwr(addr)
	static comp_result Lwr(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		// load least significant part.
		uint32_t word_address = addr/4;
		int misalignment = addr % 4;
		int bytes_to_copy = 4 - misalignment;
		uint32_t mask = ~0;
		mask << bytes_to_copy * 8;
		uint32_t mem = m.read32(word_address);
		mem >> misalignment * 8;
		return rt & mask | mem;
	}
	static comp_result Lwl(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m)
	{
		// load least significant part.
		uint32_t word_address = addr / 4;
		int misalignment = addr % 4;
		int bytes_to_copy = 4 - misalignment;
		uint32_t mask = ~0;
		mask >> bytes_to_copy * 8;
		uint32_t mem = m.read32(word_address);
		mem << misalignment * 8;
		return rt & mask | mem;
	}

	static comp_result load_dispatch(instruction i, uint32_t addr, uint32_t rt, MemoryMap& m) {
		switch (i.code) {
		case 0x20: return Lb(i, addr, rt, m);
		case 0x21: return Lh(i, addr, rt, m);
		case 0x22: return Lwl(i, addr, rt, m);
		case 0x23: return Lw(i, addr, rt, m);
		case 0x24: return Lbu(i, addr, rt, m);
		case 0x25: return Lhu(i, addr, rt, m);
		case 0x26: return Lwr(i, addr, rt, m);
		}
		return 
	}
private:
	static constexpr comp_result bad_addrl(uint32_t address) { return { address,4 << 2 }; }
	static constexpr comp_result bad_addrs(uint32_t address) { return { address,5 << 2 }; }
	static constexpr int64_t b(uint32_t r) { return s(r); }
	static constexpr uint32_t u(int32_t r) { return static_cast<uint32_t>(r); }
	static constexpr int16_t s(uint16_t r) { return static_cast<int16_t>(r); }
	static constexpr uint16_t l(uint32_t r) { return r & 0xffff; }
	static constexpr uint16_t h(uint32_t r) { return r >> 16; }
	static constexpr int16_t sl(uint32_t r) { return s(l(r)); }
	static constexpr int16_t sh(uint32_t r) { return s(h(r)); }
};