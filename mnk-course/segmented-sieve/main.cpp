#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <fstream>

class bit_vector {
private:
	static constexpr std::size_t BITS_COUNT = sizeof(std::size_t) * 8;
	
	std::size_t* _bits;
	std::size_t _size;
	std::size_t _count;

public:
	bit_vector(std::size_t size) {
		_size = size / BITS_COUNT + 1;
		_count = size;
		_bits = new std::size_t[_size];
	}

	inline void set(std::size_t index) {
		auto at = index / BITS_COUNT;
		auto bit = index & (BITS_COUNT - 1);
		_bits[at] |= (std::size_t)1 << bit;
	}

	inline std::size_t get(std::size_t index) {
		std::size_t at = index / BITS_COUNT;
		std::size_t bit = index & (BITS_COUNT - 1);
		return _bits[at] & ((std::size_t)1 << bit);
	}

	inline void clear(std::size_t to) {
		std::size_t length = std::min(to, _size);
		for(std::size_t i = 0; i < length; ++i)
			_bits[i] = 0;
	}

	inline std::size_t size() {
		return _count;
	}

	~bit_vector() {
		delete[] _bits;
	}
};

void simple_sieve(uint64_t n, bit_vector& workspace) {
	uint64_t sqrt_n = (uint64_t)((double)n);
	uint64_t j = 0;
	for(uint64_t p = 2; p <= sqrt_n; ++p) {
		if(!workspace.get(p)) {
			for(uint64_t i = p * p; i <= n; i += p * 2) {
				workspace.set(i);
			}
		}

		if(++j % 1024 == 0)
			std::cout << j << "/" << n << std::endl;
	}
}

void segment(uint32_t low, uint32_t high, bit_vector& workspace, std::vector<uint32_t>& primes) {
	uint32_t size = high - low;
	workspace.clear(size);

	for(auto p: primes) {
		uint32_t start = floor(low / p) * p;
		if(start < low)
			start += p;

		for(uint32_t i = start; i < high; i += p * 2) {
			workspace.set(i - low);
		}
	}

	for(uint32_t j = 0; j < size; ++j) {
		if(!workspace.get(j)) {
			primes.push_back(low + j);
		}
	}
}

void segmented_sieve(uint32_t n, std::vector<uint32_t>& primes) {
	uint32_t segment_size = 8192;
	uint32_t low = segment_size;
	uint32_t high = low + segment_size;
	bit_vector workspace(segment_size + 1);
	
	simple_sieve(low, workspace);

	while(low < n) {
		segment(low, high, workspace, primes);
		low += segment_size;
		high += segment_size;
	}
}

int main() {
	uint64_t N = 1ul << 31;
	bit_vector workspace(N);
	std::cout << "Sieving...\n";
	simple_sieve(N, workspace);
	
	std::cout << "\nDone, writing to file." << std::endl;
	std::ofstream output("./primes");
	output << 2 << std::endl;
	for(uint64_t i = 3; i < N; i += 2) {
		if(!workspace.get(i)) {
			output << i << std::endl;
		}
	}
}
