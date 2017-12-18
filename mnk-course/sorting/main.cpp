#include <random>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <iomanip>
#include "timsort.hpp"
#include "radix_sort.hpp"

std::mt19937_64 rng(std::random_device{}());

template<typename Fn, typename... TArgs>
auto bench(Fn action, TArgs... args) {
	auto begin = std::chrono::system_clock::now();
	action(args...);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> duration = end - begin;
	return duration.count();
}

template<typename T, typename It = typename std::vector<T>::iterator>
void mk_bench(const char* algorithm_name, void (*sort_function)(It, It), std::vector<T> input) {
	auto begin_time = std::chrono::system_clock::now();
	sort_function(input.begin(), input.end());
	// assert(std::is_sorted(input.begin(), input.end()));
	for(auto i = 0; i < 10; ++i) {
		std::cout << input[i] << "     "
	}
	auto end_time = std::chrono::system_clock::now();
	std::chrono::duration<double> duration = end_time - begin_time;

	std::cout
		<< std::left << std::setw(20) << algorithm_name
		<< "(" << input.size() << ") -> " << duration.count() << std::endl;
}

template<typename It>
inline void timsort_wrapper(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;
	timsort(begin, end, std::less<T>());
}

template<typename It>
inline void qsort_wrapper(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;
	auto placha = [](const void* x, const void* y) -> int {
		return *reinterpret_cast<const T*>(x) >= *reinterpret_cast<const T*>(y);
	};
	qsort(&*begin, end - begin, sizeof(T), placha);
}

template <typename R>
std::vector<std::string> generate_strings(int str_count, int str_length, R& rng) {
	std::uniform_int_distribution<> index_dist(0, str_length);

	std::string s;
	s.reserve(str_length);
	generate_n(back_inserter(s), str_length, rng);

	std::vector<std::string> strs = {s};
	strs.reserve(str_count);

	for(int i = 0; i < str_count - 1; ++i) {
		int si = std::uniform_int_distribution<>(0, i)(rng);
		s = strs[si];

		int a = index_dist(rng), b;
		do b = index_dist(rng);
		while(a == b);
		if(a > b) std::swap(a, b);

		generate(s.begin() + a, s.begin() + b, rng);

		strs.push_back(std::move(s));
	}

	return strs;
}

int main() {
	// std::cout << "[uint32_t]\n";
	// for(auto n : { 10, 50, 100, 1000, 100000, 1000000 }) {
	// 	std::vector<uint32_t> numbers;
	// 	generate_n(std::back_inserter(numbers), n, rng);

	// 	// mk_bench("qsort", qsort_wrapper, numbers);
	// 	mk_bench("std::sort", std::sort, numbers);
	// 	// mk_bench("std::stable_sort", std::stable_sort, numbers);
	// 	mk_bench("timsort", timsort_wrapper, numbers);
	// 	// mk_bench("radix sort", radix_sort, numbers);

	// 	std::cout << std::endl;
	// }

	// std::cout << "[uint64_t]\n";
	// for(auto n : { 10, 50, 100, 1000, 100000, 1000000 }) {
	// 	std::vector<uint64_t> numbers;
	// 	generate_n(std::back_inserter(numbers), n, rng);

	// 	// mk_bench("qsort", qsort_wrapper, numbers);
	// 	mk_bench("std::sort", std::sort, numbers);
	// 	// mk_bench("std::stable_sort", std::stable_sort, numbers);
	// 	mk_bench("timsort", timsort_wrapper, numbers);
	// 	// mk_bench("radix sort", radix_sort, numbers);

	// 	std::cout << std::endl;
	// }

	std::cout << "[std::string]\n";
	std::random_device rd;
	std::mt19937_64 rng(rd());
	for(auto n : { 10, 50, 100, 1000, 10000, 100000 }) {
		auto strs = generate_strings(n, 20, rng);

		mk_bench("qsort", qsort_wrapper, strs);
		mk_bench("std::sort", std::sort, strs);
		mk_bench("timsort", timsort_wrapper, strs);
		std::cout << std::endl;
	}
}