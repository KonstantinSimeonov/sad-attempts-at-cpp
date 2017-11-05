template<typename T>
void apply_permutation(T* start, T* end, std::size_t* permutation) {
	auto size = end - start;
	for(std::size_t i = 0; i < size; ++i) {
		auto current_index = i;
		auto next_index = permutation[current_index];
		
		if(current_index != next_index) {
			auto first_value = std::move(start[i]);
			while(true) {
				if(next_index == permutation[next_index]) {
					start[current_index] = std::move(first_value);
					permutation[current_index] = current_index;
					break;
				}
				
				start[current_index] = std::move(start[next_index]);
				permutation[current_index] = current_index;
				current_index = next_index;
				next_index = permutation[next_index];
			}
		}
	}
}
