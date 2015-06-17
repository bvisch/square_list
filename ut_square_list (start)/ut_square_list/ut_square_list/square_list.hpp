#if !defined(square_list_hpp)
#define square_list_hpp

#include <list>
using namespace std;

template <typename T>
class square_list {
public:
	using size_type = std::size_t;
	using value_type = T;
	using reference = T&;
	using const_reference = T const&;
	using pointer = T*;
	using const_pointer = T const*;
	using iterator = typename list<T>::iterator;
	using const_iterator = typename list<T>::const_iterator;
	using difference_type = std::ptrdiff_t;
	using reverse_iterator = typename list<T>::reverse_iterator;
	using const_reverse_iterator = typename list<T>::const_reverse_iterator;

private:
	using head_iterator = typename list<iterator>::iterator;
	using const_head_iterator = typename list<iterator>::const_iterator;

	class search_result {
	public:
		head_iterator head_pos;
		iterator pos;
		bool found;

		search_result(head_iterator hp, iterator p, bool f) : head_pos(hp), pos(p), found(f) {}
	};

	list<iterator> _head_list;
	list<T> _body_list;
	size_type side_length;
	search_result _find(value_type element);
	bool validateSort();

public:
	square_list() : side_length(0) {}
	size_type					size() { return _body_list.size(); }
	size_type					max_size() { return std::numeric_limits<size_type>::max(); }
	bool						empty() { return _body_list.empty(); }

	iterator					insert(value_type const& element);
	iterator					insert(value_type&& element);
	void						erase(iterator position);
	void						clear();

	iterator					begin() { return _body_list.begin(); }
	iterator					end() { return _body_list.end(); }
	const_iterator				begin() const { return _body_list.begin(); }
	const_iterator				end() const { return _body_list.end(); }
	const_iterator				cbegin() { return _body_list.cbegin(); }
	const_iterator				cend() { return _body_list.cend(); }
	reverse_iterator			rbegin() { return _body_list.rbegin(); }
	reverse_iterator			rend() { return _body_list.rend(); }
	const_reverse_iterator		rbegin() const { return _body_list.crbegin(); }
	const_reverse_iterator		rend() const { return _body_list.crend(); }
	const_reverse_iterator		crbegin() const { return _body_list.crbegin(); }
	const_reverse_iterator		crend() const { return _body_list.crend(); }


};

template <typename T>
bool square_list<T>::validateSort() {
	T temp = _body_list.front();
	for (auto it = ++begin(); it != end(); ++it) {
		if (*it < temp) { return false; }
		temp = *it;
	}
	return true;
}

template <typename T>
typename square_list<T>::search_result square_list<T>::_find(typename square_list<T>::value_type element) {
	if (size() == 0) { return search_result(_head_list.begin(), _body_list.begin(), false); }
	else if (_body_list.front() >= element) { return search_result(_head_list.begin(), _body_list.begin(), _body_list.front() == element); }
	else if (_body_list.back() == element) { return search_result(_head_list.end(), --_body_list.end(), true); }
	else if (_body_list.back() < element) { return search_result(_head_list.end(), _body_list.end(), false); }

	square_list::head_iterator it_head = _head_list.begin();
	while (it_head != _head_list.end() && **it_head < element) { ++it_head; }
	
	square_list::iterator it_body = *--it_head;
	while (*it_body <= element) { ++it_body; }

	return search_result(++it_head, it_body, *it_body == element);
}

template <typename T>
typename square_list<T>::iterator square_list<T>::insert(typename square_list<T>::value_type const& element) {
	if (size() == 0) { 
		_body_list.push_back(element);
		_head_list.push_back(_body_list.begin());
		++side_length;
		return _body_list.begin();
	}

	search_result res = _find(element);
	_body_list.insert(res.pos, element);

	while (res.head_pos != _head_list.end())
		--(*res.head_pos++);

	if (size() > pow(side_length, 2)) {
		++side_length;
		head_iterator it_head = _head_list.begin();
		for (unsigned i = 0; i < side_length - 1; ++i)
			advance(*it_head++, i);
	}
	else if (size() % side_length == 1) {
		_head_list.push_back(--_body_list.end());
	}

	return res.pos;
}

template <typename T>
typename square_list<T>::iterator square_list<T>::insert(typename square_list<T>::value_type&& element) {
	if (size() == 0) {
		_body_list.push_back(element);
		_head_list.push_back(_body_list.begin());
		++side_length;
		return _body_list.begin();
	}

	search_result res = _find(element);
	_body_list.insert(res.pos, element);

	while (res.head_pos != _head_list.end())
		--(*res.head_pos++);

	if (size() > pow(side_length, 2)) {
		++side_length;
		head_iterator it_head = _head_list.begin();
		for (size_type i = 0; i < side_length - 1; ++i)
			advance(*it_head++, i);
	}
	else if (size() % side_length == 1) {
		_head_list.push_back(--_body_list.end());
	}

	return res.pos;
}

template <typename T>
void square_list<T>::erase(typename square_list<T>::iterator position) {
	if (&_body_list.front() == &_body_list.back()) { return clear(); }

	head_iterator it_head = _head_list.begin();
	while (it_head != _head_list.end() && **it_head < *position) { ++it_head; }		// find possible nearest head position
	if (it_head != _head_list.begin()) { --it_head; }

	// find the actual closest position within the list so reflow can occur
	// worst case scenario: O(n)
	// this might be redundant... an alternative would be to simply find the first occurrence of the element and remove it
	iterator it_body;
	bool pos_found = *it_head == position;
	while (it_head != _head_list.end() && !pos_found) {
		it_body = *it_head++;
		while (it_body != _body_list.end() && it_body != position && it_head != _head_list.end() && it_body != *it_head && !pos_found) { 
			pos_found = ++it_body == position; 
		}
	}

	while (it_head != _head_list.end()) { ++(*it_head++); }

	_body_list.erase(position);

	if (size() <= pow(side_length - 1, 2)) {
		--side_length;
		head_iterator head_pos = _head_list.begin();
		for (long i = 0; i < side_length; ++i)
			advance(*head_pos++, -i); // signed-unsigned mismatch. fix this.
	}
	else if (size() % side_length == 0) {
		_head_list.erase(--_head_list.end());
	}

	return;
}

template <typename T>
void square_list<T>::clear() {
	_head_list.clear();
	_body_list.clear();
	side_length = 0;
	return;
}



#endif