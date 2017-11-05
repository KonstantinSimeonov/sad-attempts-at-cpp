#include "linked_list.hpp"
#include <iostream>

int main() {
	linked_list<int> test;

	for(int i = 0; i < 5; ++i) {
		test.push_back(i);
		// std::cout << i << "azsymnakon\n";
	}

	for(int i = 1; i < 5; ++i) {
		test.push_front(-i);
	}

	test.push_front(-5);
	test.push_back(5);

	test.reverse();
	
	test.pop_back();
	test.pop_back();
	test.pop_front();
	test.pop_front();

	test.reverse();
	test.splice(3, 3);

	std::cout << test.size() << "\n" << test.last() << "\n";

	for(auto x : test) {
		std::cout << x << " mn dobyr kon\n";
	}

	// for(int i = 0; i < 5; ++i) {
	// 	std::cout << test.last() << "\n";
	// 	test.pop_back();
	// }

	return 0;
}
