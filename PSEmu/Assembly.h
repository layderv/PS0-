#pragma once
#include <cstdint>
#include <vector>

class Assembly: public std::vector<uint32_t>
{
public:
	Assembly();
	~Assembly();
};
