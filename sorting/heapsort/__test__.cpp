#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>
#include <cassert>

#include "heapsort.hpp"

auto rng = std::random_device {}();

struct Point2D {
	static bool cmp_x(Point2D const& left, Point2D const& right) {
		return left.x < right.x;
	}

	double x;
	double y;
	Point2D(double x, double y): x(x), y(y) {}
};

std::chrono::duration<double> test_struct(std::size_t n) {
	std::mt19937_64 cyki(rng);
	std::uniform_real_distribution<double> dist(-1000000, 1000000);
	std::vector<Point2D> points;
	points.reserve(n);
	for(std::size_t i = 0; i < n; ++i) {
		points.emplace_back(dist(cyki), dist(cyki));
	}
	auto start = std::chrono::system_clock::now();	
	heapsort(points.begin(), points.end(), Point2D::cmp_x);
	auto end = std::chrono::system_clock::now();
	assert(std::is_sorted(points.begin(), points.end(), Point2D::cmp_x));
	return end - start;
}

std::chrono::duration<double> test_ints(std::size_t n) {
	std::vector<int> numbers(n);
	std::generate(numbers.begin(), numbers.end(), std::mt19937_64(rng));
	auto start = std::chrono::system_clock::now();
	heapsort(numbers.begin(), numbers.end());
	auto end = std::chrono::system_clock::now();
	assert(std::is_sorted(numbers.begin(), numbers.end()));
	return end - start;
}

int main() {
	// std::cout << test_struct().count() << std::endl;
	for(auto n : { 100, 1000, 10000, 100000 }) {
		auto duration = test_struct(n);
		std::cout << n << " -> " << duration.count() << std::endl;
	}
	// std::cout << sizeof(std::size_t) << " " << sizeof(Point2D) << std::endl;
}
