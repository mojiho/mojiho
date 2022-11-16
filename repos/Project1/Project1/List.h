#pragma once
#include <iostream>

template<typename T>
struct ListNode
{
	T data;
	ListNode<T>* Next;
	ListNode<T>* Prev;

	ListNode()
		:data(), Next(nullptr), Prev(nullptr)
	{
	
	}

	ListNode(const T& _data, ListNode<T>* _Prev, ListNode<T>* _Next)
		:data(_data), Prev(_Prev), Next(_Next)
	{
		
	}

};

template <typename T>
class CList
{
private:
	ListNode<T>* Head;
	ListNode<T>* Tail;
	int Count;

public:
	void push_back(const T& data);
	void push_front(const T& data);

public:
	CList();
	~CList();
};

template<typename T>
void CList<T>::push_back(const T& data)
{
	ListNode<T>* NewNode = new ListNode<T>(data, nullptr, nullptr);

	if (nullptr == Head)
	{
		Head = NewNode;
		Tail = NewNode;
	}
	else
	{
		Tail->Next = NewNode;
		NewNode->Prev = Tail;
		Tail = NewNode;
	}
	++Count;
}

template<typename T>
void CList<T>::push_front(const T& data)
{
	ListNode<T>* NewNode = new ListNode<T>(data, nullptr, Head);

	Head->Prev = NewNode;

	Head = NewNode;

	++Count;
}

template<typename T>
CList<T>::CList()
	:Head(nullptr),Tail(nullptr), Count(0)
{
	
}

template<typename T>
CList<T>::~CList()
{
	ListNode<T>* Deletenode = Head;

	while(Deletenode)
	{
		ListNode<T>* Next = Deletenode->Next;
		delete(Deletenode);
		Deletenode = Next;
	}
}