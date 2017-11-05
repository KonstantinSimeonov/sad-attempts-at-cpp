#pragma once

#include <cstddef>
#include <utility>
#include <iostream>

template<typename T>
class list_node {
public:
	using size_t = std::size_t;

	T* node_value;
	list_node<T>* links[2];

	list_node() {
		node_value = nullptr;
		links[0] = nullptr;
		links[1] = nullptr;
	}

	list_node(T const& value) {
		node_value = new T(value);
		links[0] = nullptr;
		links[1] = nullptr;
	}

	list_node(list_node<T> const& from) = delete;

	list_node(list_node<T>&& from) {
		node_value = from.node_value;
		links[0] = from.links[0];
		links[1] = from.links[1];
		from.node_value = nullptr;
		from.links[0] = nullptr;
		from.links[1] = nullptr;
	}

	~list_node() {
		delete node_value;
	}

	void attach(list_node<T>* node, size_t side) {
		links[side] = node;
		node->links[!side] = this;
	}

	void detach(size_t side) {
		auto node = links[side];
		if(node != nullptr) {
			node->links[!side] = nullptr;
		}
		links[side] = nullptr;
	}

	list_node<T>& operator=(list_node<T>&& from) {
		if(node_value != nullptr) {
			delete node_value;
		}

		links[0] = from.links[0];
		links[1] = from.links[1];

		return this;
	}
};

template<typename T>
class linked_list {
private:
	using size_t = std::size_t;

	size_t _disposition;
	size_t _size;
	list_node<T>* _ends[2];

public:
	linked_list() {
		_disposition = 0;
		_size = 0;
		_ends[0] = nullptr;
		_ends[1] = nullptr;
	}

	~linked_list() {
		auto it = begin();
		while(it != end()) {
			auto current = it;
			++it;
			delete current.node();
		}
		_ends[0] = nullptr;
		_ends[1] = nullptr;
	}

	inline size_t size() {
		return _size;
	}

	inline T const& first() {
		return *_ends[_disposition]->node_value;
	}

	inline T const& last() {
		return *_ends[!_disposition]->node_value;
	}

	void push(T const& item, size_t side) {
		auto new_end = new list_node<T>(item);
		++_size;
		
		if(_size == 1) {
			_ends[0] = new_end;
			_ends[1] = new_end;
			return;
		}

		auto attach_side = side ^ _disposition;
		_ends[attach_side]->attach(new_end, attach_side);
		_ends[attach_side] = std::move(new_end);
	}

	void push_back(T const& item) {
		push(item, 1);
	}

	void push_front(T const& item) {
		push(item, 0);
	}

	void pop(size_t side) {
		if(_size == 0) {
			throw "Ne popvai ot prazen list prase";
		}

		auto end = side ^ _disposition;
		auto end_node = _ends[end];
		auto new_end_node = end_node->links[!end];
		if(new_end_node != nullptr) {
			new_end_node->detach(end);
		}
		_ends[end] = new_end_node;
		--_size;
		delete end_node;
	}

	void pop_back() {
		pop(1);
	}

	void pop_front() {
		pop(0);
	}

	void reverse() {
		// haha
		_disposition = !_disposition;
	}

	void splice(size_t index, size_t count) {
		auto it = this->at(index - 1);
		auto start = it.node();
		start->detach(!_disposition);
		
		start->attach(it.node(), !_disposition);
	}

	class iterator {
	private:
		size_t& _disposition;
		list_node<T>* _current;

	public:
		iterator(linked_list<T>* list, list_node<T>* node)
			: _disposition(list->_disposition)
			, _current(node) {}

		T& operator *() {
			return *_current->node_value;
		}

		list_node<T>* node() {
			return _current;
		}

		bool operator ==(iterator const& it) {
			return _current == it._current;
		}
		
		bool operator !=(iterator const& it) {
			return !(*this == it);
		}
		
		iterator& operator ++() {
			_current = _current->links[!_disposition];
			return *this;
		}
	};
	
	iterator begin() {
		return iterator(this, _ends[_disposition]);
	}

	iterator end() {
		return iterator(this, nullptr);
	}

	iterator at(size_t index) {
		auto it = begin();
		while(index--)
			++it;

		return it;
	}
};
