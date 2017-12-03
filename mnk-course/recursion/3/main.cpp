#include <fstream>
#include <array>
#include "solutions.cpp"

constexpr uint32_t TILE_COUNT = 21;
std::array<int8_t, 1 << TILE_COUNT> solutions { SOLUTIONS };

int main() {
	std::size_t k;
	ifstream in("./game.inp");
	ofstream out("./game21.out");
	in >> k;

	while(k--) {
		uint32_t board = 0;
		std::size_t tiles_left = TILE_COUNT;
		while(tiles_left--) {
			uint32_t bit;
			in >> bit;
			board *= 2;
			board += bit;
		}

		out << std::endl << (int)solutions[board] << std::endl;
	}
}
