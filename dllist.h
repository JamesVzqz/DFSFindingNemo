#ifndef CS20A_DLLIST_H
#define CS20A_DLLIST_H

#include<iostream> // do not use namespace in header files
#include<assert.h>
// Linked DLList object that maintains both head and tail pointers
// and the size of the list.  Note that you have to keep the head,
// tail and size consistent with the intended state of the DLList 
// otherwise very bad things happen. 
template<typename Item>
class DLList {
public:

	DLList();

	DLList(const DLList<Item>& other);

	DLList<Item>& operator=(const DLList<Item>& other);

	~DLList();

	void	print() const;
	bool	empty() const;

	void	push_front(const Item &item);
	void	push_rear(const Item &item);
	void	push_at(int idx, const Item &item);

	// Note that the user must first ensure the list is not empty
	// prior to calling these functions. 
	Item	front() const;
	Item	rear() const;
	Item	at(int idx) const;

	int		count() const;
	int		find(const Item &item) const;

	bool	pop_front();
	bool	pop_rear();
	bool	pop_at(int idx);
	int		pop_item(const Item &item);

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare the nodes for our DLList.
	// Will be implemented along with list
	// member functions
	struct Node;

	// We'll have both head and tail points for 
	// Fast insertion/deletion from both ends.
	Node*	head;
	Node*	tail;

	// Keep track of number of nodes in the list
	int		size;
};


/* DLList Implementation
//
//  Since DLList is a template class (which is not an actual 
//  class yet, not until we actually instantiate the list) 
//  we need to keep the implementation together with 
//  the definition.
*/

/* Node definition
*/
template<typename Item>
struct DLList<Item>::Node {
	Node() :next(nullptr), prev(nullptr) {}
	Node(Item it, Node* p, Node* n) : item(it), next(n), prev(p) {}

	Item  item;
	Node* next;
	Node* prev;
};

/* DLList default constructor
//		Set head and tail pointer to point to nothing, size is zero
//		Already implemented, nothing to do.
*/
template<typename Item>
DLList<Item>::DLList() :head(nullptr), tail(nullptr), size(0) {
}

/* Copy constructor  
*/
template<typename Item>
DLList<Item>::DLList(const DLList<Item>& other) {

	if (other.head == nullptr) {
		head = nullptr;
		tail = nullptr;
		size = 0;
	}
	else {
		Node* p = other.head;

		while (p->next != nullptr) {
			push_rear(p->item);
			p = p->next;
		}
		push_rear(p->item);
	}

}

/* Overloaded assignment operator 
*/
template<typename Item>
DLList<Item>& DLList<Item>::operator=(const DLList<Item>& other) {

	Node* p = other.head;
	Node* q = nullptr;
	Node* r = nullptr;

	if (other.head == nullptr) {
		head = nullptr;
		tail = nullptr;
		size = 0;
	}
	else {
		head = new Node(p->item, nullptr, nullptr);
		q = head;
		p = p->next;

		while (p != nullptr) {
			r = new Node(p->item, q, nullptr);
			q->next = r;
			q = r;
			p = p->next;
		}
		tail = q;
		size = other.size;
	}

	return *this;

}

/* DLList destructor
*/
template<typename Item>
DLList<Item>::~DLList() {

	Node* p = head;
	Node* q = nullptr;
	while (p != nullptr) {
		q = p->next;
		delete p;
		p = q;
	}
	head = nullptr;
	tail = nullptr;
	size = 0;

}

/* DLList print
*/		
template<typename Item>
void DLList<Item>::print() const {

	if (empty()) return;

	Node* p; // Node pointer to iterate
	p = head; // p point to 1st node

	if (p->next == nullptr) {
		std::cout << p->item;
	}
	else {
		// cycle through the nodes (transversal)
		while (p->next != nullptr) {
			std::cout << p->item << " "; // prints item
			p = p->next; // point to next
		}
		std::cout << p->item;
	}
	
}

/* DLList empty
*/
template<typename Item>
bool DLList<Item>::empty() const {

	bool dummyReturnValue = false;

	if (size == 0) {
		dummyReturnValue = true;
	}

	return dummyReturnValue;

}

/* DLList push_front
*/
template<typename Item>
void DLList<Item>::push_front(const Item &item) {

	Node* p;                                 // Node pointer
	p = new Node;                            // points to allocated temporary node
	p->item = item;                          // put item into node

	// new Node
	p->next = head;                          // set new top node next points to old top node
	p->prev = nullptr;                       // set new top node prev points to nullptr

	if (head != nullptr) {
		// old Node
		head->prev = p;                      // update old top node prev points to new top node
	}

	// head
	head = p;                                // update head to point to new top node
	// tail
	tail = head;                             // tail point to first element
		while (tail->next != nullptr)        // traverse tail to the last node of the list
			tail = tail->next;

	size++;                                  // add 1 to size

}

/* DLList push_rear
*/
template<typename Item>
void DLList<Item>::push_rear(const Item &item) {

	if (head == nullptr) {                   // if linked list is empty, push to front
		push_front(item);
	}
	else {
		//tail
		tail = head;                         // tail point to first element
		while (tail->next != nullptr)        // traverse tail to the last node of the list
			tail = tail->next;

		Node* p = new Node;                  // allocate new node p
		p->item = item;                      // put item into node

		// old Node
		tail->next = p;                      // update old last node next points to new last node

		// new Node
		p->prev = tail;                      // set new last node prev points to old last node
		p->next = nullptr;                   // set new last node prev points to nullptr

		tail = head;
		while (tail->next != nullptr)
			tail = tail->next;

		size++;                              // add 1 to size
	}

}

/* DLList push_at
*/
template<typename Item>
void DLList<Item>::push_at(int idx, const Item &item) {

	if (head == nullptr) {
		push_front(item);
	}
	else if (idx <= 0) {
		push_front(item);
	}
	else if (idx >= size) {
		push_rear(item);
	}
	else {
		int i = 1;
		Node* p = head;
		for (int i = 0; i < idx - 1; i++) 
        {
            p = p->next;
        }
        Node* q = new Node(item, p, p->next);
        p->next->prev = q;
        p->next = q;

		size++;
	}

}

/*  DLList front
*/
template<typename Item>
Item DLList<Item>::front() const {

	// This should never be tripped, this is just here to let
	// you know that you messed up with your usage, this function
	// is never called on an empty list.
	assert(head != nullptr); 
	
	Item dummyReturnValue;

	dummyReturnValue = head->item;

	return dummyReturnValue;

}

/* DLList rear
*/
template<typename Item>
Item DLList<Item>::rear() const {

	// This should never be tripped, this is just here to let
	// you know that you messed up with your usage, this function
	// is never called on an empty list.
	assert(tail != nullptr);

	Item dummyReturnValue;

	dummyReturnValue = tail->item;

	return dummyReturnValue;
	
}

/* DLList at
*/
template<typename Item>
Item DLList<Item>::at(int idx) const {

	// Assert that index is correct before getting the Item
	assert(idx >= 0 && idx < size);

	Item dummyReturnValue;

	int i = 0;
	Node* p = head;
	while (p != nullptr) {
		if (i == idx)
			break;
		p = p->next;
		i++;
	}

	if (p != nullptr) {
		return dummyReturnValue = p->item;
	}

}

/* DLList count
*/
template<typename Item>
int DLList<Item>::count() const {

	int dummyReturnValue = 9000;

	dummyReturnValue = size;

	return dummyReturnValue;

}

/* DLList find
*/
template<typename Item>
int DLList<Item>::find(const Item &item) const {

	int dummyReturnValue = 8675309;

	int i = 1;

	if (head->item == item) {
		return dummyReturnValue = 0;
	}
	Node* p = head;
	while (p != nullptr) {
		if (p->next != nullptr && p->next->item == item)
			break;
		p = p->next;
		i++;
	}
	if (p == nullptr) {
		i = -1;
	}

	dummyReturnValue = i;

	return dummyReturnValue;

}

/* DLList pop_front
*/		
template<typename Item>
bool DLList<Item>::pop_front() {

	bool dummyReturnValue = false;

	if (head == nullptr)
		return dummyReturnValue;
	else if (size == 1) {
		Node* killMe = head;
		head = nullptr;
		tail = nullptr;

		delete killMe;

		dummyReturnValue = true;

		size--;
	}
	else {
		Node* killMe = head;
		head = killMe->next;
		killMe->next->prev = nullptr;

		delete killMe;

		tail = head;
		while (tail->next != nullptr)
			tail = tail->next;

		dummyReturnValue = true;

		size--;
	}

	return dummyReturnValue;

}

/* DLList pop_rear
*/	
template<typename Item>
bool DLList<Item>::pop_rear() {

	bool dummyReturnValue = false;

	if (head == nullptr)
		return dummyReturnValue;
	else if (size == 1) {
		pop_front();
	}
	else {
		Node* killMe = tail;
		tail = killMe->prev;
		killMe->prev->next = nullptr;

		delete killMe;

		dummyReturnValue = true;

		size--;
	}

	return dummyReturnValue;

}

/* DLList pop_at on index
*/
template<typename Item>
bool DLList<Item>::pop_at(int idx) {

	bool dummyReturnValue = false;

	if (head == nullptr)
		return dummyReturnValue;
	else if (size == 1) {
		pop_front();
	}
	else {
		int i = 1;
		if (idx == 0) {
			Node* killMe = head;
			head = killMe->next;
			head->prev = nullptr;

			delete killMe;

			size--;

			return dummyReturnValue = true;
		}
		Node* p = head;
		while (p != nullptr) {
			if (p->next != nullptr && i == idx)
				break;
			p = p->next;
			i++;
		}
		if (p != nullptr) {
			Node* killMe = p->next;
			p->next = killMe->next;
			killMe->next->prev = p;

			delete killMe;

			dummyReturnValue = true;

		}


		size--;
	}

	return dummyReturnValue;

}

/* DLList pop_item on item
*/
template<typename Item>
int DLList<Item>::pop_item(const Item &item) {

	int dummyReturnValue = -2;

	if (head == nullptr)
		return dummyReturnValue;
	else if (size == 1) {
		pop_front();
		dummyReturnValue = 0;
	}
	else {
		int i = 1;

		if (head->item == item) {
			Node* killMe = head;
			head = killMe->next;
			head->prev = nullptr;

			delete killMe;

			size--;

			return dummyReturnValue = 0;
		} 
		Node* p = head;
		while (p != nullptr) {
			if (p->next != nullptr && p->next->item == item)
				break;
			p = p->next;
			i++;
		}
		if (p != nullptr) {
			Node* killMe = p->next;
			p->next = killMe->next;
			if (p->next != nullptr)
				p->next->prev = p;

			delete killMe;
		}
		else if (p == nullptr) {
			return dummyReturnValue = -1;
		}

		dummyReturnValue = i;

		size--;
	}

	return dummyReturnValue;

}

#endif



























































































// _X_XMXXII