#pragma once

#include <thread>
#include <functional>

/*------------------------------------
 *			 ThreadManager
 ------------------------------------*/

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void Launch(function<void(void)> callback);
	void Join();

	void static InitTLS();
	void static DestroyTLS();
private:
	Mutex _lock;
	vector<thread> _threads;
};

