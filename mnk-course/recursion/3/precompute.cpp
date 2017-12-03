#include <array>
#include <fstream>

constexpr uint32_t TILE_COUNT = 21;
constexpr uint32_t PATTERNS_COUNT = 15;
constexpr uint32_t PATTERN_COMINATION_COUNT = 1 << 15;
constexpr uint32_t POSSIBLE_INPUTS_COUNT = 1 << TILE_COUNT;

constexpr uint32_t bottom_square = 7 + (7 << 4) + (7 << 9);
constexpr uint32_t middle_square = (7 << 3) + (7 << 8) + (7 << 13);
constexpr uint32_t top_square = (7 << 18) + (7 << 14) + (7 << 9);
constexpr uint32_t bottom_cross = 1 + (7 << 3) + (1 << 9);
constexpr uint32_t middle_cross = (1 << 4) + (7 << 8) + (1 << 14);
constexpr uint32_t top_cross = (1 << 18) + (7 << 13) + (1 << 9);

const uint32_t patterns[15] {
	POSSIBLE_INPUTS_COUNT - 1,
	top_square,
	middle_square, middle_square << 1, middle_square << 2,
	bottom_square,
	top_cross, top_cross << 1, top_cross << 2,
	middle_cross, middle_cross << 1, middle_cross << 2,
	bottom_cross, bottom_cross << 1, bottom_cross << 2
};

std::array<int8_t, POSSIBLE_INPUTS_COUNT> solutions;

void compute_possible_boards() {
	solutions.fill(-1);
	solutions[0] = 0;

	std::array<uint32_t, PATTERN_COMINATION_COUNT> boards;
	boards[0] = 0;

	for(uint32_t combination = 1; combination < PATTERN_COMINATION_COUNT; ++combination) {
		uint32_t last_bit = __builtin_ctz(combination);
		uint32_t without_last = combination & (combination - 1);
		boards[combination] = boards[without_last] ^ patterns[last_bit];

		int8_t current_steps = solutions[boards[without_last]] + 1;
		int8_t& current_value = solutions[boards[combination]];

		if(current_value == -1 || current_value > current_steps) {
			current_value = current_steps;
		}
	}
}

int main() {
	compute_possible_boards();
	std::ofstream f("./solutions.cpp");
	f << "#define SOLUTIONS ";
	for(std::size_t i = 0; i < POSSIBLE_INPUTS_COUNT; ++i) {
		f << (int)solutions[i];
		if(i != POSSIBLE_INPUTS_COUNT - 1) f << ",";
	}
}
