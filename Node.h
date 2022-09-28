#pragma once

#include <iostream>
using namespace std;

template < typename T>
class Node
{
private:
	T item; // A data item
	float priority; // Indicates priority of node (for priority queue)
	Node<T>* next; // Pointer to next node
public:
	Node();
	Node(const T& r_Item);
	Node(const T& r_Item, Node<T>* nextNodePtr);
	Node(const T& r_Item, Node<T>* nextNodePtr, float inputPriority);
	Node(const T& r_Item, float inputPriority);
	void setItem(const T& r_Item);
	void setNext(Node<T>* nextNodePtr);
	void setPriority(float inputPriority);
	T getItem() const;
	float getPriority() const;
	Node<T>* getNext() const;
}; // end Node

template < typename T>
Node<T>::Node()
{
	next = nullptr;
	priority = 0;
}

template < typename T>
Node<T>::Node(const T& r_Item)
{
	item = r_Item;
	next = nullptr;
	priority = 0;
}

template < typename T>
Node<T>::Node(const T& r_Item, Node<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
	priority = 0;
}

template < typename T>
Node<T>::Node(const T& r_Item, Node<T>* nextNodePtr, float inputPriority)
{
	item = r_Item;
	next = nextNodePtr;
	priority = inputPriority;
}

template < typename T>
Node<T>::Node(const T& r_Item, float inputPriority)
{
	item = r_Item;
	next = nullptr;
	priority = inputPriority;
}

template < typename T>
void Node<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
void Node<T>::setPriority(float inputPriority)
{
	priority = inputPriority;
}

template < typename T>
T Node<T>::getItem() const
{
	return item;
}

template < typename T>
float Node<T>::getPriority() const
{
	return priority;
}

template < typename T>
Node<T>* Node<T>::getNext() const
{
	return next;
}