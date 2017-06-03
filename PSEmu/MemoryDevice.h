#pragma once
#include <cstdint>
#include <array>

/// MemoryDevice: any device which comunicates through write and reads from cpu
class MemoryDevice
{
public:
	virtual ~MemoryDevice() = 0 {};
	virtual void Write(uint32_t address, uint32_t size, uint8_t const * data)=0;
	virtual void Read(uint32_t address, uint32_t size, uint8_t* outData)=0;
	virtual uint8_t* Map(uint32_t address, uint32_t size)=0;
};

#include <memory>
template<size_t size>
struct MemoryBlock : public MemoryDevice
{
	std::array<uint8_t, size> buffer{ 0 };
	virtual void Write(uint32_t address, uint32_t size, uint8_t const * data) {
		std::copy(data, data + size, buffer.begin() + address);
	}
	virtual void Read(uint32_t address, uint32_t size, uint8_t* outData) {
		std::copy(buffer.begin() + address, buffer.begin() + address + size, outData);
	}
	virtual uint8_t* Map(uint32_t address, uint32_t size) {
		return &buffer[0];
	}
};
