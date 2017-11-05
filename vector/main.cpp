#include <iostream>
#include "vector.hpp"

int main() {
	vector<int> v(10000000);
	for(auto& x : v) {
		x = 42;
	}

	std::cin.get();

	auto v2 = v;

	std::cin.get();
	
	v2 = v;

	std::cin.get();
	

	return 0;
}