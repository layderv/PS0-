#include "MemoryMap.h"
#include <algorithm>
#include <vector>

#include "MemoryDevice.h"

std::vector<std::pair<uint32_t,uint32_t>> MemoryMap::find_maps_for_range(uint32_t begin, uint32_t end)
{
	std::vector<std::pair<uint32_t,uint32_t>> relevant_maps;
	uint32_t address = begin;
	auto marker_it = markers.begin();
	while (address < end){
		marker_it = std::find_if(marker_it, markers.end(), [&](auto &e) { return e.first <= address; });
		if (marker_it != markers.end()) {
			auto marker = *marker_it++;
			if (marker.second > address)
			{
				address = std::min(end, marker.second);
				relevant_maps.push_back(marker);
			}
		}
		else
			break;
	}
	return relevant_maps;
}
template<typename T>
void MemoryMap::store(uint32_t address, T const & data)
{
	store(address, sizeof(T), reinterpret_cast<uint8_t const *>(&data));
}

void MemoryMap::store8(uint32_t address, uint8_t data)
{
	store<uint8_t>(address,data);
}

void MemoryMap::store16(uint32_t address, uint16_t data)
{
	store<uint16_t>(address, data);
}

void MemoryMap::store32(uint32_t address, uint32_t data)
{
	store<uint32_t>(address, data);
}

uint8_t const* MemoryMap::write_single_range(uint32_t map_start, uint32_t write_begin, uint32_t write_end, uint8_t const *input_data) {
	std::shared_ptr<MemoryDevice> device;
	uint32_t mapBaseAddress, mapSize, deviceOffset, flags;
	std::tie(device, mapBaseAddress, mapSize, deviceOffset, flags) = maps[map_start];

	device->Write(write_begin-map_start+deviceOffset, write_end-write_begin, input_data);
	return input_data + (write_end-write_begin);
}

uint8_t * MemoryMap::read_single_range(uint32_t map_start, uint32_t write_begin, uint32_t write_end, uint8_t * input_data)
{
	std::shared_ptr<MemoryDevice> device;
	uint32_t mapBaseAddress, mapSize, deviceOffset, flags;
	std::tie(device, mapBaseAddress, mapSize, deviceOffset, flags) = maps[map_start];

	device->Read(write_begin - map_start + deviceOffset, write_end - write_begin, input_data);
	return input_data + (write_end - write_begin);
}

template<typename Iterator>
void MemoryMap::store_in_ranges(Iterator begin,Iterator end, uint32_t address_begin, uint32_t address_end, uint8_t const * data) {
	auto src_ptr = data;
	for (auto it = begin; it != end; it++) {
		std::pair<uint32_t, uint32_t> i = *it;
		if (address_begin < i.first) {
			// TODO: handle page fault
			return;
		}
		auto write_end = std::min(i.second, address_end);
		src_ptr = write_single_range(i.first, address_begin, write_end , src_ptr);
		address_begin = write_end;
	}
	if (address_begin!= address_end)
	{
		// TODO: handle page fault
		return;
	}
}

template<typename T>
T MemoryMap::read(uint32_t address)
{
	T out;
	read(address, sizeof(T), reinterpret_cast<uint8_t*>(&out));
	return out;
}

template<typename Iterator>
void MemoryMap::read_in_ranges(Iterator begin, Iterator end, uint32_t address_begin, uint32_t address_end, uint8_t * data)
{
	auto src_ptr = data;
	for (auto it = begin; it != end; it++) {
		std::pair<uint32_t, uint32_t> i = *it;
		if (address_begin < i.first) {
			// TODO: handle page fault
			return;
		}
		auto write_end = std::min(i.second, address_end);
		src_ptr = read_single_range(i.first, address_begin, write_end, src_ptr);
		address_begin = write_end;
	}
	if (address_begin != address_end)
	{
		// TODO: handle page fault
		return;
	}
}

void MemoryMap::store(uint32_t start_address, uint32_t size, uint8_t const * data)
{
	auto range = find_maps_for_range(start_address, start_address + size);
	store_in_ranges(range.begin(), range.end(), start_address, start_address + size, data);
}

uint8_t MemoryMap::read8(uint32_t address)
{
	return read<uint8_t>(address);
}

uint16_t MemoryMap::read16(uint32_t address)
{
	return read<uint16_t>(address);
}

uint32_t MemoryMap::read32(uint32_t address)
{
	return read<uint32_t>(address);
}

void MemoryMap::read(uint32_t start_address, uint32_t size, uint8_t * out_data)
{
	auto range = find_maps_for_range(start_address, start_address + size);
	read_in_ranges(range.begin(), range.end(), start_address, start_address + size, out_data);
}

bool MemoryMap::add_map(map_type const &map)
{
	std::shared_ptr<MemoryDevice> device;
	uint32_t mapBaseAddress, mapSize, deviceOffset, flags;
	std::tie(device, mapBaseAddress, mapSize, deviceOffset, flags) = map;
	if (find_maps_for_range(mapBaseAddress, mapBaseAddress + mapSize).size() > 0)
		return false;
	else {
		maps.emplace(mapBaseAddress, map);
		//markers.emplace(mapBaseAddress, mapBaseAddress);
		markers.emplace(mapBaseAddress, mapBaseAddress+mapSize);
		return true;
	}
}

bool MemoryMap::remove_map(uint32_t baseAddress)
{
	auto mapIt = maps.find(baseAddress);
	if (mapIt != maps.end()) {
		auto map = std::move(mapIt->second);
		maps.erase(mapIt);
		markers.erase(baseAddress);
		return true;
	}
	else
		return false;
}

MemoryMap::MemoryMap(std::initializer_list<std::tuple<std::shared_ptr<MemoryDevice>, uint32_t,uint32_t, uint32_t, uint32_t>> init_list)
{
	for ( auto e:init_list) {
		add_map(e);
	}
}

MemoryMap::~MemoryMap()
{
}
