#include <variant>
#include <random>
#include <stdexcept>
#include <iostream> //TESTING ONLY; REMOVE

#ifndef RANDOMIZER_H
#define RANDOMIZER_H

struct Randomizer {
	
	
	template<unsigned bitwidth, typename type>
	static type constexpr randInt() 
	{
		std::variant<int64_t, int32_t, int16_t, int8_t> val;
		if constexpr (bitwidth == 64)
		{
			val = static_cast<int64_t>(dist64i(rng));
			return std::get<int64_t>(val);
		}	
		else if constexpr (bitwidth == 32) {
			val = static_cast<int32_t>(dist32i(rng));
			return std::get<int32_t>(val);
		}
		else if constexpr (bitwidth == 16) {
			val = static_cast<int16_t>(dist16i(rng));
			return std::get<int16_t>(val);
		}
		else if constexpr (bitwidth == 8) {
			val = static_cast<int8_t>(dist8i(rng));
			return std::get<int8_t>(val);
		}
		else
			throw std::invalid_argument("randInt must take an int8_t - int64_t type");
	}
	//Function below allows setting bounds
	template<unsigned bitwidth, unsigned upper, typename type>
	static type constexpr randInt() 
	{
		std::variant<int64_t, int32_t, int16_t, int8_t> val;
		if constexpr (bitwidth == 64)
		{
			val = static_cast<int64_t>(dist64i(rng) % upper);
			return std::get<int64_t>(val);
		}	
		else if constexpr (bitwidth == 32) {
			val = static_cast<int32_t>(dist32i(rng) % upper);
			return std::get<int32_t>(val);
		}
		else if constexpr (bitwidth == 16) {
			val = static_cast<int16_t>(dist16i(rng) % upper);
			return std::get<int16_t>(val);
		}
		else if constexpr (bitwidth == 8) {
			val = static_cast<int8_t>(dist8i(rng) % upper);
			return std::get<int8_t>(val);
		}
		else
			throw std::invalid_argument("randInt must take an int8_t - int64_t type");
	}

	private:
	static std::random_device dev;
	static std::mt19937 rng;
	static std::uniform_int_distribution<int64_t> dist64i;
	static std::uniform_int_distribution<int32_t> dist32i;
	static std::uniform_int_distribution<int16_t> dist16i;
	static std::uniform_int_distribution<int8_t> dist8i;
	static int test;
};

int Randomizer::test = 5;
//Initialize static variables of Randomizer class
std::random_device Randomizer::dev = std::random_device();
std::mt19937 Randomizer::rng = std::mt19937(dev());
std::uniform_int_distribution<int64_t> Randomizer::dist64i = std::uniform_int_distribution<int64_t>(-9223372036854775807ll, 9223372036854775807ll);	//For some reason, negative value can not be 2^63, but must be 2^63 - 1
std::uniform_int_distribution<int32_t> Randomizer::dist32i = std::uniform_int_distribution<int32_t>(-2147483648, 2147483647);
std::uniform_int_distribution<int16_t> Randomizer::dist16i = std::uniform_int_distribution<int16_t>(-32768, 32767);
std::uniform_int_distribution<int8_t> Randomizer::dist8i = std::uniform_int_distribution<int8_t>(-128, 127);

#endif
