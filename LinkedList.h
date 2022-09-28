#pragma once


#include "Node.h"

template <typename T>
class LinkedList
{
protected:
	Node<T>* Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
public:


	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{
		cout << "\nprinting list contents:\n\n";
		Node<T>* p = Head;

		while (p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}

	bool isEmpty()
	{
		if (!Head)
			return true;
		else
			return false;
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}

	//InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data)
	{

		Node<T>* newNode = new Node<T>(data);
		newNode->setNext(nullptr);
		if (Head == nullptr)
		{
			Head = newNode;
		}
		else
		{

			Node<T>* temp = Head;
			while (temp->getNext() != NULL)
				temp = temp->getNext();
			temp->setNext(newNode);
		}

	}

	//RemoveFirst
	//Removes the first node in the list

	void RemoveFirst(T& item)
	{
		if (!Head)
			return;
		Node<T>* tempPtr = Head;
		Head = Head->getNext();
		item = tempPtr->getItem();
		delete tempPtr;
		tempPtr = nullptr;
	}

	// DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value)
	{
		Node<T>* prev = Head;
		if (!Head)
			return false;
		Node<T>* delptr = Head->getNext();
		if (prev->getItem() == value)
		{
			Head = delptr;
			delete prev;
			prev = nullptr;
			return true;
		}
		while (delptr)
		{
			if (delptr->getItem() == value)
			{
				prev->setNext(delptr->getNext());
				delete delptr;
				delptr = nullptr;
				return true;
			}
			prev = delptr;
			delptr = delptr->getNext();
		}
		return false;
	}
	//Retrieve
	//Removes Item from List and returns it without deleting it

	bool Retrieve(const T& value, T& retrievedItem)
	{
		Node<T>* prev = Head;
		if (!Head)
			return false;
		Node<T>* retptr = Head->getNext();
		if (prev->getItem() == value)
		{
			retrievedItem = prev->getItem();
			Head = retptr;
			delete prev;
			prev = nullptr;
			return true;
		}
		while (retptr)
		{
			if (retptr->getItem() == value)
			{
				retrievedItem = retptr->getItem();
				prev->setNext(retptr->getNext());
				delete retptr;
				retptr = nullptr;
				return true;
			}
			prev = retptr;
			retptr = retptr->getNext();
		}
		return false;
	}
	//GetCount
	//returns count of elements in list
	int getCount()
	{
		int count = 0;
		Node<T>* temp = Head;
		while (temp)
		{
			count++;
			temp = temp->getNext();
		}
		return count;
	}
	//Print
	//prints contents of list
	void print()
	{
		Node<T>* temp = Head;
		while (temp)
		{
			cout << temp->getItem();
			if (temp->getNext())
				cout << ", ";
			temp = temp->getNext();
		}
	}
};