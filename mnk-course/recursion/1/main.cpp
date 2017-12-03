#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <limits>

std::size_t lower_bound(std::vector<int> const& sorted, int value, std::size_t low, std::size_t high) {
	if(low == high)
		return low;
	
	auto mid = (low + high) / 2;

	if(sorted[mid] > value)
		high = mid;
	else
		low = mid + 1;
	
	return lower_bound(sorted, value, low, high);
}

void solve_greedily_iter(int n, std::vector<int> const& cubes, std::vector<int>& ans) {
	while(n) {
		int lb = lower_bound(cubes, n, 0, cubes.size());
		if(lb > 0)
			--lb;

		// std::cout << lb << " " <<  cubes[lb] << " " << n << std::endl;

		ans.push_back(cubes[lb]);
		n -= cubes[lb];
	}
}


struct stuff {
	int cube, score;
};

std::vector<int> cubes;
std::map<int, stuff> memory;
int upper_bound = std::numeric_limits<int>::max() / 2;
int calls = 0;

int upper_depth_bound(int n) {
	if(n == 0)
		return 0;

	int lb_index = lower_bound(cubes, n, 0, cubes.size());
	if(lb_index > 0)
		--lb_index;

	return upper_depth_bound(n - cubes[lb_index]) + 1;
}

int solve(int n, int current_depth = 0) {
	++calls;
	if(n < 0 || upper_bound < current_depth)
		return std::numeric_limits<int>::max() / 2;

	if(n == 0) {
		return 0;
	}

	auto current = memory[n];
	if(current.cube) {
		return current.score;
	}

	stuff min { cubes[cubes.size() - 1], solve(n - cubes[cubes.size() - 1], current_depth + 1) + 1 };
	for(int i = cubes.size() - 2; i >= 0; --i) {
		auto score = solve(n - cubes[i], current_depth + 1) + 1;

		if(score < min.score) {
			min.cube = cubes[i];
			min.score = score;
		}
	}

	memory[n] = min;
	return min.score;
}

int main() {
	int n;
	std::cin >> n;

	auto third_root = ceil(pow((double)n, 1.0 / 3.0));

	std::vector<int> cubes_copy;
	for(int i = 1; i < third_root; ++i) {
		cubes.push_back(i * i * i);
		cubes_copy.push_back(i * i * i);
	}

	upper_bound = upper_depth_bound(n);
	std::cout << upper_bound << std::endl;

	std::vector<int> ans;
	int i = 10;
	std::size_t min = 100000000;
	while(i-- && cubes_copy.size() > 2) {
		ans.clear();
		solve_greedily_iter(n, cubes_copy, ans);

		min = std::min(min, ans.size());
		cubes_copy.pop_back();
	}

	std::cout << min << std::endl;
	upper_bound = min;
	solve(n);
	std::cout << "Map size: " << memory.size() << std::endl
			<< "Recursive calls: " << calls << std::endl;

	// for(auto x : memory) {
	// 	std::cout << x.first << " " << x.second.cube << ":" << x.second.score << std::endl;
	// }
	for(int i = n; i; i -= memory[i].cube) {
		std::cout << memory[i].cube << " ";
	}
}