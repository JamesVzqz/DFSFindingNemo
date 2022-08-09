#ifndef STACK_H_
#define STACK_H_

#include"dllist.h"

// Stack ADT using linked list as primary data structure.
// 
template<typename Item>
class Stack {
public:

	Stack();

	// Basic stack operations
	void	push(Item item);
	void	pop();
	Item	peek() const; 

	// Check to see if there is anything on the stack
	bool	empty() const;

	// Use to see what is in the stack for debugging
	void	print() const;

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif

	DLList<Item> m_list;
};

// Stack Implementation
//

// Stack() Default constrcutor:
//		Call the defualt constructor for its List.
//		Already implemented.
template<typename Item>
Stack<Item>::Stack():m_list() {}


template<typename Item>
void Stack<Item>::push(Item item) {
	
	m_list.push_front(item);

}


template<typename Item>
void Stack<Item>::pop() {
	
	m_list.pop_front();

}


template<typename Item>
bool Stack<Item>::empty() const {
	
	return m_list.empty();

}


template<typename Item>
Item Stack<Item>::peek() const {
	
	return m_list.front();

}


template<typename Item>
void Stack<Item>::print() const {
	
	m_list.print();

}




























#endif//STACK_H__X_XMMXX