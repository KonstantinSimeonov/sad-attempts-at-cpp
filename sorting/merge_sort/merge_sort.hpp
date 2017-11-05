#pragma once

#include <cstdlib>
#include <thread>

template<typename T, typename C>
void insertion_sort(T* start, T* end, C cmp) {
	for(auto i = start + 1; i < end; ++i) {
		auto j = i - 1;
		auto v = std::move(*i);
		while(j >= start && cmp(v, *j)) {
			*(j + 1) = std::move(*j);
			--j;
		}
		*(j + 1) = std::move(v);
	}
}

template<typename T, typename C>
void merge(T* start, T* mid, T* end, T* workspace, C cmp) {
	auto l = start;
	auto r = mid;
	auto i = 0;
	while(l < mid && r < end) {
		if(cmp(*l, *r)) {
			workspace[i] = std::move(*l);
			++l;
		} else {
			workspace[i] = std::move(*r);
			++r;
		}

		++i;
	}

	while(l < mid) {
		workspace[i] = std::move(*l);
		++l;
		++i;
	}

	while(r < end) {
		workspace[i] = std::move(*r);
		++r;
		++i;
	}

	std::move(workspace, workspace + i, start);
}

template<typename T, typename C>
void merge_sort(T* start, T* end, T* workspace, std::size_t threads, C cmp) {
	auto chunk_size = end - start;
	if(chunk_size <= 10) {
		insertion_sort(start, end, cmp);
		return;
	}

	auto mid = start + chunk_size / 2;
	if(threads > 1) {
		std::thread left(merge_sort<T, C>, start, mid, workspace, threads / 2, cmp);
		merge_sort(mid, end, workspace + (mid - start), threads / 2, cmp);
		left.join();
	} else {
		merge_sort(start, mid, workspace, 0, cmp);
		merge_sort(mid, end, workspace + (mid - start), 0, cmp);
	}
	merge(start, mid, end, workspace, cmp);
}

template<typename T, typename C>
inline void merge_sort(T* start, T* end, std::size_t threads, C cmp) {
	auto n = end - start;
	T* const workspace = (T*)std::malloc(n * sizeof(T));
	merge_sort(start, end, workspace, threads, cmp);
	std::free(workspace);
}

template<typename T>
inline void merge_sort(T* start, T* end, std::size_t threads = 0) {
	merge_sort(start, end, threads, std::less<T>());
}
