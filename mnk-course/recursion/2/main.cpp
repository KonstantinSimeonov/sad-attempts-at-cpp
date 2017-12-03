#include <iostream>
#include <vector>

uint64_t pow_mod(uint64_t n, uint64_t p, uint64_t mod) {
	if(p == 0)
		return 1;

	if(p % 2)
		return n * pow_mod(n, p - 1, mod) % mod;

	auto half = pow_mod(n, p / 2, mod) % mod;
	return half * half;
}

int main() {
	uint64_t m, n, y;
	std::cin >> m >> n >> y;
	bool x_exists = false;

	for(uint64_t x = 0; x < m; ++x) {
		if(pow_mod(x, n, m) == y) {
			std::cout << x << std::endl;
			x_exists = true;
		}
	}

	if(!x_exists)
		std::cout << -1 << std::endl;
}
