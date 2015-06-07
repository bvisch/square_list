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

	void resquare(head_iterator start);

public:
	square_list();
	~square_list();
	iterator	insert(T element);
	iterator	begin() { return _body_list.begin(); }
	iterator	end() { return _body_list.end(); }
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
	for (square_list::head_iterator it_head = start; it_head != _head_list.end(); ++it_head)
		*it_head--;
}

template <typename T>
typename square_list<T>::iterator square_list<T>::insert(T element) {
	for (square_list::head_iterator it_head = _head_list.begin(); it_head != _head_list.end(); ++it_head) {
		if (**it_head > element) {
			for (square_list::reverse_iterator it_body = square_list::reverse_iterator(*it_head); it_body != _body_list.rend(); ++it_body) {
				if (*it_body < element) {
					_body_list.insert(it_body--.base(), element);
					resquare(it_head);
					return (it_body + 1).base();
				}
			}
		}
	}
}

#endif