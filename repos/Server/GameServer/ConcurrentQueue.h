#pragma once

#include <mutex>

template<typename T>
class LockQueue
{
public:
	LockQueue() {}

	LockQueue(const LockQueue&) = delete;
	LockQueue& operator=(const LockQueue&) = delete;


	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_queue.push(std::move(value));
		_condVar.notify_one();
	}


	bool Trypop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_queue.empty())
			return false;

		value = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	bool WaitPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _queue.empty() == false; });
		value = std::move(_queue.front());
		_queue.pop();
	}
private:
	queue<T> _queue;
	mutex _mutex;
	condition_variable _condVar;
};

template<typename T>
class LockFreeQueue
{
	struct Node;

	struct CountednodePtr
	{
		int32 externalCount;
		Node* ptr = nullptr;
	};

	struct NodeCounter
	{
		uint32 internalCount : 30;
		uint32 externalCountRemaining : 2;
	};

	struct Node
	{
		Node()
		{
			NodeCounter newCount;
			newCount.internalCount = 0;
			newCount.externalCountRemaining = 2;
			count.store(newCount);

			next.ptr = nullptr;
			next.externalCount = 0;
		}

		void ReleaseRef()
		{
			
		}

		atomic<T> data;
		atomic<NodeCounter> count;
		CountednodePtr next;

	};

public:
	LockFreeQueue() : _head(new Node), _tail(_head)
	{
		
	}

	LockFreeQueue(const LockFreeQueue&) = delete;
	LockFreeQueue& operator=(const LockFreeQueue&) = delete;

	void Push(const T& value)
	{
		unique_ptr<T> newData = make_unique<T>(value);

		CountednodePtr dummy;
		dummy.ptr = new Node;
		dummy.externalCount = 1;

		CountednodePtr oldTail = _tail.load();

		while (true)
		{
			IncreaseExternalCount(_tail, _oldTail);

			T* oldData = nullptr;
			if (oldTail.ptr->data.compare_exchange_strong(oldData, newData.get()))
			{
				oldTail.ptr->next = dummy;
				oldData = _tail.exchange(dummy);

				newData.release();
				break;
			}

		}

		/*shared_ptr<T> newData = make_shared<T>(value);

		Node* dummy = new Node();

		Node* oldTail = _tail;
		oldTail->data.swap(newData);
		oldTail->next = dummy;

		_tail = dummy;*/
	}

	shared_ptr<T> TryPop()
	{
		Node* oldHead = PopHead();
		if (oldHead == nullptr)
			return shared_ptr<T>();

		shared_ptr<T> res(oldHead->data);
		delete oldHead;
		return res;
	}
private:
	/*Node* PopHead()
	{
		Node* oldHead = _head;
		if (oldHead == _tail)
			return nullptr;

		_head = oldHead->next;
		return oldHead;
	}*/
	static void IncreaseExternalCount(atomic<CountednodePtr>& counter, CountednodePtr& oldCounter)
	{
		while (true)
		{
			CountednodePtr newCounter = oldCounter;
			newCounter.externalCount++;

			if(counter.compare_exchange_strong(oldCounter,newCounter))
			{
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}
private:
	/*
	Node* _head = nullptr;
	Node* _tail = nullptr;*/
	atomic<CountednodePtr> _head;
	atomic<CountednodePtr> _tail;
};
