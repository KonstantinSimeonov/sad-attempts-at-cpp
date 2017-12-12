#include <memory>

constexpr auto Base = 8;

template<typename It>
void radix_sort(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;
	const auto N = end - begin;
	if(N < 2) return;

	auto m_index = std::make_unique<std::size_t[][1 << Base]>(sizeof(T));
	auto helper_array = std::make_unique<T[]>(N);

	for(std::size_t i = 0; i < N; ++i) {
		auto current = begin[i];

		for(std::size_t j = 0; j < sizeof(T); ++j) {
			m_index[j][current & 0xFF]++;
			current >>= Base;
		}
	}

	for(std::size_t i = 0; i < sizeof(T); ++i) {
		T sum = 0;

		for(T j = 0; j < (1 << Base); ++j) {
			auto tmp = m_index[i][j];
			m_index[i][j] = sum;
			sum += tmp;
		}
	}

	for(std::size_t i = 0; i < sizeof(T); ++i) {
		bool odd = i & 1;

		for(std::size_t j = 0; j < N; ++j) {
			auto current = odd ? helper_array[j] : begin[j];
			auto index_queue = (current >> (i * 8)) & 0xFF;
			if(odd) {
				begin[m_index[i][index_queue]++] = current;
			} else {
				helper_array[m_index[i][index_queue]++] = current;
			}
		}
	}
}
