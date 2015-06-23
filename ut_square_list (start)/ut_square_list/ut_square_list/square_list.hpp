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

	list<iterator>				_head_list;
	list<T>						_body_list;
	size_type					side_length;

	search_result				_find(value_type const& element);
	bool						validateSort();

public:
	//constructors
	square_list() : side_length(0) {}
	square_list(square_list const& sq) : _body_list(sq._body_list), side_length(sq.side_length) {
		if (side_length == 0) { return; }
		iterator it_body = _body_list.begin();
		size_type head_count = (size() - 1) / side_length;

		_head_list.push_back(it_body);
		for (size_type i = 0; i < head_count; ++i) { std::advance(it_body, side_length); _head_list.push_back(it_body); }
	}
	square_list(square_list&& sq) : _head_list(std::move(sq._head_list)), _body_list(std::move(sq._body_list)), side_length(std::move(sq.side_length)) {}
	template <typename InputIt>
	square_list(InputIt begin, InputIt end) : side_length(0) { insert(begin, end); }
	square_list(std::initializer_list<value_type> const& init) : side_length(0) { insert(init); }


	//assignment operators
	square_list& operator = (square_list const& rhs) {
		_body_list = rhs._body_list;
		side_length = rhs.side_length;

		if (side_length == 0) { return *this; }
		iterator it_body = _body_list.begin();
		size_type head_count = size() / side_length;

		_head_list.push_back(it_body);
		for (size_type i = 0; i < head_count; ++i) { std::advance(it_body, side_length); _head_list.push_back(it_body); }

		return *this;
	}

	square_list& operator = (square_list&& rhs) { _head_list = std::move(rhs._head_list); _body_list = std::move(rhs._body_list); side_length = std::move(rhs.side_length); return *this; }


	//size operations
	size_type						size() const { return _body_list.size(); }
	size_type						max_size() { return std::numeric_limits<size_type>::max(); }
	size_type						count(value_type const& key);
	bool							empty() { return _body_list.empty(); }

	//insert
	iterator						insert(value_type const& element);
	iterator						insert(value_type&& element);
	template <typename InputIt>
	void							insert(InputIt begin, InputIt end);
	void							insert(std::initializer_list<value_type> init);

	//erase
	void							erase(iterator position);
	iterator						erase(iterator begin, iterator end);
	value_type						erase(value_type key);

	void							clear();
	void							swap(square_list& sq);
	
	//exposed find
	iterator						find(value_type const& element);
	const_iterator					find(value_type const& element) const;

	std::pair<iterator, iterator>	equal_range(value_type const& key);

	//front & back
	const_reference					front() const { return _body_list.front(); }
	const_reference					back() const { return _body_list.back(); }

	//iterators
	iterator						begin() { return _body_list.begin(); }
	iterator						end() { return _body_list.end(); }
	const_iterator					begin() const { return _body_list.begin(); }
	const_iterator					end() const { return _body_list.end(); }
	const_iterator					cbegin() { return _body_list.cbegin(); }
	const_iterator					cend() { return _body_list.cend(); }
	reverse_iterator				rbegin() { return _body_list.rbegin(); }
	reverse_iterator				rend() { return _body_list.rend(); }
	const_reverse_iterator			rbegin() const { return _body_list.crbegin(); }
	const_reverse_iterator			rend() const { return _body_list.crend(); }
	const_reverse_iterator			crbegin() const { return _body_list.crbegin(); }
	const_reverse_iterator			crend() const { return _body_list.crend(); }
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

//Returns the first head and body iterators greater than or equal to the supplied element
template <typename T>
typename square_list<T>::search_result square_list<T>::_find(typename square_list<T>::value_type const& element) {
	if (size() == 0) { return search_result(_head_list.begin(), _body_list.begin(), false); }
	else if (_body_list.front() >= element) { return search_result(_head_list.begin(), _body_list.begin(), _body_list.front() == element); }

	square_list::head_iterator it_head = _head_list.begin(), it_head_end = _head_list.end();
	while (!(it_head == it_head_end || **it_head >= element)) { ++it_head; } //Until
	
	square_list::iterator it_body = *--it_head, it_body_end = _body_list.end();
	while (!(it_body == it_body_end || *it_body >= element)) { ++it_body; } //Until

	return search_result(++it_head, it_body, it_body != it_body_end && *it_body == element);
}

template <typename T>
typename square_list<T>::iterator square_list<T>::find(typename square_list<T>::value_type const& element) {
	search_result res = _find(element);
	if (!res.found) { return _body_list.end(); }
	return res.pos;
}

template <typename T>
typename square_list<T>::const_iterator square_list<T>::find(typename square_list<T>::value_type const& element) const {
	search_result res = _find(element);
	if (!res.found) { return _body_list.cend(); }
	return const_iterator(res.pos);
}

template <typename T>
std::pair<typename square_list<T>::iterator, typename square_list<T>::iterator> square_list<T>::equal_range(typename square_list<T>::value_type const& key) {
	return std::equal_range(begin(), end(), key);
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
		_body_list.push_back(std::move(element));
		_head_list.push_back(_body_list.begin());
		++side_length;
		return _body_list.begin();
	}

	search_result res = _find(element);
	_body_list.insert(res.pos, std::move(element));

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
template <typename InputIt>
void square_list<T>::insert(InputIt begin, InputIt end) {
	while (begin != end) { insert(*begin++); }
}

template <typename T>
void square_list<T>::insert(std::initializer_list<value_type> init) {
	std::initializer_list<value_type>::iterator it = init.begin();
	while (it != init.end()) { insert(*it++); }
}

template <typename T>
void square_list<T>::erase(typename square_list<T>::iterator position) {
	if (size() == 1) { return clear(); }

	search_result res = _find(*position);
	
	while (!(res.pos == position)) {
		if (res.pos == *res.head_pos) { res.head_pos++; }
		res.pos++;
	}

	while (!(res.head_pos == _head_list.end())) {
		(*res.head_pos++)++;
	}

	//head_iterator it_head = _head_list.begin();
	//while (it_head != _head_list.end() && **it_head < *position) { ++it_head; }		// find possible nearest head position
	//if (it_head != _head_list.begin()) { --it_head; }

	//// find the actual closest position within the list so reflow can occur
	//// worst case scenario: O(n)
	//// this might be redundant... an alternative would be to simply find the first occurrence of the element and remove it
	//iterator it_body;
	//bool pos_found = *it_head == position;
	//while (it_head != _head_list.end() && !pos_found) {
	//	it_body = *it_head++;
	//	while (it_body != _body_list.end() && it_body != position && it_head != _head_list.end() && it_body != *it_head && !pos_found) { 
	//		pos_found = ++it_body == position; 
	//	}
	//}

	//while (it_head != _head_list.end()) { ++(*it_head++); }

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
typename square_list<T>::iterator square_list<T>::erase(iterator begin, iterator end) {	//come back to this, maybe use templating
	while (begin != end) { erase(begin++); }
	return begin;
}

template <typename T>
typename square_list<T>::value_type square_list<T>::erase(value_type key) {
	size_type count = 0;
	search_result res = _find(key);
	iterator it_body_end = _body_list.end();

	while (it_body_end != res.pos && *res.pos == key) {
		iterator temp = res.pos++;
		erase(temp);
		count++; 
	}

	return count;
}

template <typename T>
void square_list<T>::clear() {
	_head_list.clear();
	_body_list.clear();
	side_length = 0;
	return;
}

template <typename T>
void square_list<T>::swap(square_list& sq) {
	std::swap(*this, sq);
}

template <typename T>
void swap(square_list<T> const& lhs, square_list<T> const& rhs) { return lhs.swap(rhs); }

template <typename T>
inline bool operator < (square_list<T> const& lhs, square_list<T> const& rhs) { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

template <typename T>
inline bool operator == (square_list<T> const& lhs, square_list<T> const& rhs) { return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin()); }

template <typename T>
inline bool operator > (square_list<T> const& lhs, square_list<T> const& rhs) { return rhs < lhs; }

template <typename T>
inline bool operator <= (square_list<T> const& lhs, square_list<T> const& rhs) { return !(rhs < lhs); }

template <typename T>
inline bool operator >= (square_list<T> const& lhs, square_list<T> const& rhs) { return !(lhs < rhs); }

template <typename T>
inline bool operator != (square_list<T> const& lhs, square_list<T> const& rhs) { return !(lhs == rhs); }

template <typename T>
typename square_list<T>::size_type square_list<T>::count(value_type const& key) {
	search_result res = _find(key);
	if (!res.found) { return 0; }
	size_type count = 0;
	while (res.pos != _body_list.end() && *res.pos == key) { ++res.pos; ++count; }
	return count;
}

#endif