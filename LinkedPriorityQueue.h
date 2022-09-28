
/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

#pragma once
#include "Node.h"
#include "QueueADT.h"
#include <vector>
using namespace std;


template <typename T>
class LinkedPriorityQueue :public QueueADT<T>
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	LinkedPriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool enqueue(const T& newEntry, float inputPriority);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	int getCount();
	void print();
	~LinkedPriorityQueue();

	//copy constructor
	LinkedPriorityQueue(const LinkedPriorityQueue<T>& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool LinkedPriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/
template <typename T>
bool LinkedPriorityQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	return true;
} // end enqueue


template <typename T>
bool LinkedPriorityQueue<T>::enqueue(const T& newEntry, float inputPriority)
{
	Node<T>* newNodePtr = new Node<T>(newEntry, inputPriority);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else if (newNodePtr->getPriority() > frontPtr->getPriority()) //Special case if the priority of the new node is greater than the first node
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
	}
	else // base case
	{
		Node<T>* temp = frontPtr;
		while (temp->getNext()) // finding the appropriate position of the new node
		{
			if (newNodePtr->getPriority() > temp->getNext()->getPriority())
				break;
			temp = temp->getNext();
		}
		newNodePtr->setNext(temp->getNext()); // Setting the position of the new node
		temp->setNext(newNodePtr);
	}
	return true;

} // end enqueue


  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*Function: dequeue
  Removes the front of this queue. That is, removes the item that was added
  earliest.

  Input: None.
  Output: True if the operation is successful; otherwise false.
  */

template <typename T>
bool LinkedPriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
template <typename T>
bool LinkedPriorityQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////
/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
template <typename T>
LinkedPriorityQueue<T>::~LinkedPriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: Copy constructor
To avoid shallow copy,
copy constructor is provided

Input: LinkedQueue<T>: The Queue to be copied
Output: none
*/

template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue(const LinkedPriorityQueue<T>& LQ)
{
	Node<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	Node<T>* ptr = new Node<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
}

//gets count of elements in queue
template <typename T>
int LinkedPriorityQueue<T> ::getCount()
{
	int count = 0;
	Node<T>* temp = frontPtr;
	while (temp)
	{
		count++;
		temp = temp->getNext();
	}
	return count;
}
//prints elements of queue
template <typename T>
void LinkedPriorityQueue<T> ::print()
{
	Node<T>* temp = frontPtr;
	while (temp)
	{
		cout << temp->getItem();
		if (temp->getNext())
			cout << ", ";
		temp = temp->getNext();
	}
}