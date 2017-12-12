#include <random>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <cassert>
#include <iostream>
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
	assert(std::is_sorted(input.begin(), input.end()));
	auto end_time = std::chrono::system_clock::now();
	std::chrono::duration<double> duration = end_time - begin_time;

	std::cout << "N = " << input.size() << " : " << duration.count() << " : " << algorithm_name << std::endl;
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

int main() {
	for(auto n : { 10, 100, 1000, 10000, 100000, 1000000 }) {
		std::vector<uint64_t> numbers;
		generate_n(std::back_inserter(numbers), n, rng);

		std::cout << "[uint32_t]\n";
		mk_bench("qsort", qsort_wrapper, numbers);
		mk_bench("std::sort", std::sort, numbers);
		mk_bench("std::stable_sort", std::stable_sort, numbers);
		mk_bench("timsort", timsort_wrapper, numbers);
		mk_bench("radix sort", radix_sort, numbers);

		std::cout << std::endl;
	}
}