#pragma once

#include <cstddef>

template<typename T>
class vector {
private:
	using size_t = std::size_t;

	static constexpr size_t MIN_CAPACITY = 4;

	size_t _size;
	size_t _current_capacity;
	T* _elements;

public:
	vector(size_t size): vector() {
		resize(size);
	}

	vector() {
		_size = 0;
		_current_capacity = 0;
		_elements = nullptr;
	}

	vector(vector<T> const& from): vector(from.size()) {
		for(size_t i = 0; i < from.size(); ++i) {
			_elements[i] = from[i];
		}
	}

	~vector() {
		if(_elements) {
			delete[] _elements;
			_elements = nullptr;
			_size = 0;
			_current_capacity = 0;
		}
	}

	inline size_t size() const {
		return _size;
	}

	inline T& back() const {
		return _elements[_size - 1];
	}

	void push_back(T const& item) {
		if(!_elements) {
			_current_capacity = MIN_CAPACITY;
			reserve(_current_capacity);
		} else if(_size >= _current_capacity) {
			_current_capacity *= 2;
			reserve(_current_capacity);
		}

		_elements[_size] = item;
		++_size;
	}

	void pop_back() {
		back().~T();
		--_size;
	}

	void insert(T const& item, size_t at_index) {
		if(_size > _current_capacity) {
			resize(_current_capacity * 2);
		}

		for(size_t i = _size; i > at_index; --i) {
			_elements[i] = _elements[i - 1];
		}

		_elements[at_index] = item;
	}

	void reserve(size_t capacity) {
		if(_size >= capacity) {
			return;
		}

		T* new_elements = new T[capacity];
		for(size_t i = 0; i < _size; ++i) {
			new_elements[i] = _elements[i];
		}

		if(_elements) {
			delete[] _elements;
		}
		_elements = new_elements;
		_current_capacity = capacity;
	}

	void resize(size_t new_size) {
		if(_size > new_size) {
			for(size_t i = new_size; i > _size; --i) {
				pop_back();
			}
		} else {
			reserve(new_size);
			_size = new_size;
		}
	}

	T& operator [](size_t index) const {
		return _elements[index];
	}

	vector<T>& operator =(vector<T> const& other) {
		resize(other.size());
		for(size_t i = 0; i < other.size(); ++i) {
			_elements[i] = other[i];
		}
	}

	class iterator {
	private:
		T* _current;
	public:
		iterator(T* current) {
			_current = current;
		}
		
		T& operator *() {
			return *_current;
		}
		
		bool operator ==(iterator const& it) {
			return _current == it._current;
		}
		
		bool operator !=(iterator const& it) {
			return !(*this == it);
		}
		
		iterator& operator ++() {
			++_current;
			return *this;
		}
	};
	
	iterator begin() {
		return iterator(_elements);
	}

	iterator end() {
		return iterator(_elements + _size);
	}
};
