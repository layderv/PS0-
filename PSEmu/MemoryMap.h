#pragma once

#include <initializer_list>
#include <tuple>
#include <cstdint>
#include <memory>
#include <map>
#include <unordered_map>

template<typename D, typename W> void endian_change(D& buf) {
	union {
		W word[2];
		D dword;
	} dw;
	dw.dword = buf;
	dw.word[0] ^= dw.word[1];
	dw.word[1] ^= dw.word[0];
	dw.word[0] ^= dw.word[1];
	buf=dw.dword;
}

class MemoryDevice;
class MemoryMap
{
	using map_type = std::tuple<std::shared_ptr<MemoryDevice>, uint32_t, uint32_t, uint32_t, uint32_t>;
	std::unordered_map<uint32_t, map_type> maps;
	std::map<uint32_t, uint32_t> markers;
	std::vector<std::pair<uint32_t, uint32_t>> find_maps_for_range(uint32_t begin,uint32_t end);
	uint8_t const* write_single_range(uint32_t map_start, uint32_t write_begin, uint32_t write_end, uint8_t const *input_data);
	uint8_t * read_single_range(uint32_t map_start, uint32_t write_begin, uint32_t write_end, uint8_t * input_data);

	template<typename T>
	void store(uint32_t address, T const & data);
	template<typename Iterator>
	void store_in_ranges(Iterator begin, Iterator end, uint32_t address_begin, uint32_t address_end, uint8_t const * data);
	template<typename T>
	T read(uint32_t address);
	template<typename Iterator>
	void read_in_ranges(Iterator begin, Iterator end, uint32_t address_begin, uint32_t address_end, uint8_t * data);

public:
	void store8(uint32_t address, uint8_t data);
	void store16(uint32_t address, uint16_t data);
	void store32(uint32_t address, uint32_t data);
	void store(uint32_t start_address, uint32_t size, uint8_t const * data);
	uint8_t read8(uint32_t address);
	uint16_t read16(uint32_t address);
	uint32_t read32(uint32_t address);
	int8_t readi8(uint32_t address);
	int16_t readi16(uint32_t address);
	int32_t readi32(uint32_t address);
	void read(uint32_t start_address, uint32_t size, uint8_t * out_data);
	bool add_map(
		std::shared_ptr<MemoryDevice> device,
		uint32_t mapBaseAddress,
		uint32_t mapSize,
		uint32_t deviceOffset,
		uint32_t flags
	) {
		return add_map(std::tie(device, mapBaseAddress, mapSize, deviceOffset, flags));
	}
	bool add_map(map_type const & map);
	bool remove_map(uint32_t mapBaseAddress);
	MemoryMap() = default;
	MemoryMap(std::initializer_list < map_type > init_list);
	~MemoryMap();
};

