#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>

#include "merge_sort.hpp"

struct Point3D {
	static bool cmp_x(Point3D const& first, Point3D const& second) {
		return first.xyz[0] < second.xyz[0];
	}

	double* xyz;
	Point3D() = delete;
	Point3D(double x, double y, double z) {
		xyz = new double[3]{ x, y, z };
	}

	Point3D(Point3D const& from): Point3D(from.xyz[0], from.xyz[1], from.xyz[2]) {}
	Point3D(Point3D&& from): xyz(from.xyz) {
		from.xyz = nullptr;
	}

	Point3D& operator=(Point3D const& from) {
		_destroy();
		xyz = new double[3] { from.xyz[0], from.xyz[1], from.xyz[2] };
		return *this;
	}

	Point3D& operator=(Point3D&& from) {
		_destroy();
		xyz = from.xyz;
		from.xyz = nullptr;
		return *this;
	}

	~Point3D() {
		_destroy();
	}
	
private:
	void _destroy() {
		delete[] xyz;
		xyz = nullptr;
	}
};

std::chrono::duration<double> test_struct() {
	auto n = 1 << 25;
	std::mt19937_64 cyki(std::random_device {}());
	std::uniform_real_distribution<double> dist(-1000000, 1000000);
	std::vector<Point3D> points;
	points.reserve(n);
	for(int i = 0; i < n; ++i) {
		points.emplace_back(dist(cyki), dist(cyki), dist(cyki));
	}
	std::cout << "sorting points\n";	
	auto start = std::chrono::system_clock::now();	
	merge_sort(&points[0], &points[n], 4, Point3D::cmp_x);
	auto end = std::chrono::system_clock::now();
	
	return end - start;
}

std::chrono::duration<double> test_ints() {
	std::vector<int> numbers(1 << 5);
	std::generate(numbers.begin(), numbers.end(), std::mt19937_64(std::random_device {}()));
	std::cout << "sorting ints\n";
	auto start = std::chrono::system_clock::now();
	merge_sort(&numbers[0], &numbers[numbers.size()], std::thread::hardware_concurrency());
	auto end = std::chrono::system_clock::now();
	return end - start;
}

int main() {
	std::cout << test_struct().count() << std::endl;
	// std::cout << test_ints().count() << std::endl;
	std::cout << sizeof(std::size_t) << " " << sizeof(Point3D) << std::endl;
}