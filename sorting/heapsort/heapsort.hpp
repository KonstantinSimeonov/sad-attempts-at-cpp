#pragma once

#include <utility>
#include <cstddef>

template<typename It, typename Cmp>
void heapify_down(It const& begin, It const& end, std::size_t offset, Cmp cmp) {
	std::size_t largest = offset;
	std::size_t left = offset * 2 + 1;
	std::size_t right = left + 1;
	auto root = std::move(*(begin + largest));

	while(begin + right < end) {
		largest = left + cmp(*(begin + left), *(begin + right));

		if(cmp(root, *(begin + largest))) {
			*(begin + offset) = std::move(*(begin + largest));
			offset = largest;
	
			left = offset * 2 + 1;
			right = left + 1;
		} else break;
	}

	if(begin + left < end && cmp(*(begin + largest), *(begin + left))) {
		largest = left;
		*(begin + offset) = std::move(*(begin + largest));
	}

	*(begin + largest) = std::move(root);
}

template<typename It, typename Cmp>
void heapsort(It const& begin, It end, Cmp cmp) {
	// build a heap from the middle to the left
	for(auto mid = (end - begin) / 2; mid >= 0; --mid)
		heapify_down(begin, end, mid, cmp);

	// pop the max element and place it at the end
	// while the heap has elements
	while(end-- > begin) {
		std::swap(*begin, *end);
		heapify_down(begin, end, 0, cmp);
	}
}

template<typename It>
inline void heapsort(It const& begin, It const& end) {
	using T = std::remove_reference_t<decltype(*begin)>;
	heapsort(begin, end, std::less<T>());
}
