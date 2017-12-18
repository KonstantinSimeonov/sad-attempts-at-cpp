#include <vector>
#include <algorithm>
#include <memory>

std::size_t calculate_min_run(std::size_t size) {
	while(size >= 64)
		size = (size + 1) >> 1;
	return size;
}

template<typename It, typename T, typename Cmp> class TimSort {
public:
	TimSort(Cmp cmp): _cmp(cmp) {}

	std::size_t _min_gallop = 7;
	std::vector<It> _runs;
	std::unique_ptr<T[]> _merge_buffer;
	const Cmp _cmp;

	void _merge(It left, It mid, It right) {
		left = upper_bound(left, mid, *mid, _cmp);
		right = lower_bound(mid, right, *(mid - 1), _cmp);

		const auto LSize = mid - left;
		const auto RSize = right - mid;
		const auto buffer = _merge_buffer.get();

		// std::size_t gallop_c_left = 0;
		// std::size_t gallop_c_right = 0;
		
		if(LSize <= RSize) {
			std::move(left, mid, buffer);
			auto dest = left;
			auto l = buffer;
			auto r = mid;
			auto buffer_mid = buffer + (mid - left);
			// std::size_t step = 1;
		
			while(r < right) {
				if(_cmp(*l, *r)) {
					*dest++ = std::move(*l++);
					// ++gallop_c_left;
					// gallop_c_right = 0;
					// if(gallop_c_left >= _min_gallop) {
					// 	goto start_gallop_left;
					// }
				}
				else {
					*dest++ = std::move(*r++);
					// ++gallop_c_right;
					// gallop_c_left = 0;
					
					// if(gallop_c_right >= _min_gallop) {
					// 	goto start_gallop_right;
					// }
				}


// 				continue;

// 				do {
// 					_min_gallop--;

// start_gallop_right:
// 					{
// 						step = 1;
// 						std::size_t max_step_right = right - r;
// 						while(step < max_step_right && _cmp(*(r + step), *l)) {
// 							step = step * 2 + 1;
// 						}

// 						auto last_step = step / 2;
// 						step = std::min(step, max_step_right);

// 						auto blq = std::lower_bound(r + last_step, r + step, *l);

// 						dest = std::move(r, blq, dest);
// 						r = blq;
// 					}

// 					if(_min_gallop <= step) break;
// 					_min_gallop--;
// start_gallop_left:
// 					{
// 						step = 1;
// 						std::size_t max_step_left = buffer_mid - l;
// 						while(step < max_step_left && _cmp(*(l + step), *r)) {
// 							step = step * 2 + 1;
// 						}

// 						auto last_step = step / 2;
// 						step = std::min(step, max_step_left);

// 						auto blq2 = std::upper_bound(l + last_step, l + step, *r);
// 						dest = std::move(l, blq2, dest);
// 						l = blq2;
// 					}
// 				} while(_min_gallop <= step);

// 				if(_min_gallop < 0) _min_gallop = 1;
// 				++_min_gallop;
			}

			std::move(l, buffer_mid, dest);
		} else {
			std::move(mid, right, buffer);
		
			auto l = mid - 1;
			auto r = buffer + RSize - 1;
			auto dest = right;
			while(l + 1 != left) {
				if(_cmp(*r, *l)) *--dest = std::move(*l--);
				else *--dest = std::move(*r--);
			}
		
			std::move_backward(buffer, r + 1, dest);
		}
	}

	inline void _maintain_invariant(It current_end) {
		while(_runs.size() >= 3) {
			auto x = _runs[_runs.size() - 1];
			auto y = _runs[_runs.size() - 2];
			auto z = _runs[_runs.size() - 3];

			if(y - z <= current_end - y) {
				_merge(z, y, x);
				_runs[_runs.size() - 2] = x;
				_runs.pop_back();
			} else if(y - z <= current_end - x) {
				_merge(y, x, current_end);
				_runs.pop_back();
			} else {
				break;
			}
		}
	}

	void operator()(It begin, It end) {
		const auto SIZE = end - begin;

		if(SIZE < 64) {
			for(auto i = begin; i != end; ++i) {
				auto v = std::move(*begin);
				auto insert_ptr = std::upper_bound(begin, i, v, std::less<T>());

				for(auto curr = i; curr != insert_ptr; --curr)
					*curr = std::move(*(curr - 1));

				*insert_ptr = std::move(v);
			}
			return;
		}

		_merge_buffer = std::make_unique<T[]>(SIZE / 2);
		const auto runLength = calculate_min_run(SIZE);

		for(auto i = begin; i != end;) {
			auto begin_run = i;
			_runs.push_back(begin_run);
			if(end - i == 1) {
				_maintain_invariant(end);
				break;
			}

			std::size_t len = 2;
			bool descending = _cmp(*(i + 1), *i);
			i += 2;
			while(i != end && descending == _cmp(*i, *(i - 1))) {
				++i;
				++len;
			}

			if(descending) std::reverse(begin_run, i);

			while(i != end && len < runLength) {
				if(_cmp(*i, *(i - 1))) {
					auto v = std::move(*i);
					auto insert_ptr = std::upper_bound(begin_run, i, v, std::less<T>());
					
					for(auto curr = i; curr != insert_ptr; --curr)
						*curr = std::move(*(curr - 1));

					*insert_ptr = std::move(v);
				}

				++i;
				++len;
			}

			_maintain_invariant(i);
		}

		for(auto x = _runs.size() - 1; x > 0; --x) {
			_merge(_runs[x - 1], _runs[x], end);
		}
	}
};

template<typename It, typename Cmp>
inline void timsort(It begin, It end, Cmp cmp) {
	using T = std::remove_reference_t<decltype(*begin)>;
	TimSort<It, T, Cmp> { cmp }(begin, end);
}
