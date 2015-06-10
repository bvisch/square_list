// your code here...
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
	using head_iterator = typename list<iterator>::iterator;
	using const_iterator = typename list<T>::const_iterator;
	using const_head_iterator = typename list<iterator>::const_iterator;
	using difference_type = std::ptrdiff_t;
	using reverse_iterator = typename list<T>::reverse_iterator;
	using const_reverse_iterator = typename list<T>::const_reverse_iterator;

private:
	list<iterator> _head_list;
	list<T> _body_list;
	size_type side_length;
	size_type size;
	size_type head_size;

	void resquare(head_iterator start);
	iterator search(value_type element);

public:
	square_list() : side_length(0), size(0), head_size(0) {}
	//~square_list();
	iterator		binsert(iterator position, value_type element);
	head_iterator	hinsert(head_iterator position, iterator element);
	iterator		insert(value_type element);
	iterator		begin() { return _body_list.begin(); }
	iterator		end() { return _body_list.end(); }
};

//template <typename T>
//square_list<T>::square_list() {
//	_head_list = new list<typename list<T>::iterator>;
//	_body_list = new list<T>;
//}
//
//template <typename T>
//square_list<T>::~square_list() {
//	delete _head_list;
//	delete _body_list;
//}

template <typename T>
void square_list<T>::resquare(head_iterator start) {
	if (head_size <= side_length) {
		for (square_list::head_iterator it_head = start; it_head != _head_list.end(); ++it_head)
			*it_head--;
	}
	else if (head_size > side_length) {
		_head_list.clear();
		unsigned count = 0;
		for (square_list::iterator it_body = _body_list.begin(); it_body != _body_list.end(); ++it_body) {
			if (count % side_length == 0) {
				_head_list.push_back(it_body);
			}
			++count;
		}
	}
}

//template <typename T>
//typename square_list<T>::iterator square_list<T>::search(value_type element) {
//	for (square_list::head_iterator it_head = _head_list.begin(); it_head != _head_list.end(); ++it_head) {
//		if (**it_head >= element) {
//			for (square_list::reverse_iterator it_body = square_list::reverse_iterator(*it_head); it_body != _body_list.rend(); ++it_body) {
//				if (*it_body < element) {
//					return it_body--.base();
//				}
//			}
//		}
//	}
//}

template <typename T>
typename square_list<T>::iterator square_list<T>::binsert(typename square_list<T>::iterator position, typename square_list<T>::value_type element) {
	++size;
	if (size > pow(head_size, 2)) {
		++side_length;
	}
	return _body_list.insert(position, element);
}

template <typename T>
typename square_list<T>::head_iterator square_list<T>::hinsert(typename square_list<T>::head_iterator position, typename square_list<T>::iterator element) {
	++head_size;
	return _head_list.insert(position, element);
}

template <typename T>
typename square_list<T>::iterator square_list<T>::insert(typename square_list<T>::value_type element) {
	if (_body_list.begin() == _body_list.end()) {
		binsert(_body_list.begin(), element);
		hinsert(_head_list.begin(), _body_list.begin());
		return _body_list.begin();
	}
	//binsert(search(element), element);


	for (square_list::head_iterator it_head = _head_list.begin(); it_head != _head_list.end(); ++it_head) {
		if (**it_head > element) {
			for (square_list::reverse_iterator it_body = square_list::reverse_iterator(*it_head); it_body != _body_list.rend(); ++it_body) {
				auto asdf = prev(_body_list.rend());
				if (*it_body < element) {
					binsert(--it_body.base(), element);
					resquare(it_head);
					return it_body.base();
				}
				else if (it_body == prev(_body_list.rend())) {
					binsert(_body_list.begin(), element);
					resquare(it_head);
					return _body_list.begin();
				}
			}
		}
		else if (it_head == prev(_head_list.end())) {
			for (square_list::iterator it_body = *it_head; it_body != _body_list.end(); ++it_body) {
				if (*it_body > element) {
					binsert(--it_body, element);
					resquare(it_head);
					return it_body;
				}
			}
		}
	}
}

#endif