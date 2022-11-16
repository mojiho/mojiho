#include "pch.h"
#include "DeadLockProfiler.h"

/*------------------------------------
 *		DeadLockProfiler
 ------------------------------------*/

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	//아이디를 찾거나 발급한다.
	int32 lockId = 0;

	auto findIt = _nameTold.find(name);
	if (findIt == _nameTold.end())
	{
		lockId = static_cast<int32>(_nameTold.size());
		_nameTold[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	// 잡고 있는 락이 있었다면
	if(_lockStack.empty() == false)
	{
		//기존에 발견되지 않은 케이스라면 데드락 여부 다시 확인
		const int32 prevId = _lockStack.top();
		if(lockId != prevId)
		{
			set<int32>& history = _lockHistory[prevId];
			if(history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if(_lockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = _nameTold[name];
	if (_lockStack.top() != lockId)
		CRASH();

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameTold.size());
	_discoverdOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	//연산이 끝난 후 정리

	_discoverdOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	if(_discoverdOrder[here] != -1)
		return;

	_discoverdOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회 한다.
	auto findIt = _lockHistory.find(here);
	if(findIt == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;
	for(int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if(_discoverdOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		//here가 there보다 먼저 발견되었다면, there는 here의 후손이다. (순방향 간선)
		if(_discoverdOrder[here] < _discoverdOrder[there])
			continue;

		// 순방향이 아니고, Dfs(there)가 아직 종료하지 않았다면, there는 here의 선조이다. (역방향 간선)
		if(_finished[there] == false)
		{
			printf("%s -> %s", _idToName[here], _idToName[there]);

			int32 now = here;
			while(true)
			{
				printf("%s -> %s\n", _idToName[now], _idToName[now]);
				now = _parent[now];
				if(now == there)
					break;
			}
			CRASH("DEADLOCK_DETECTED");
		}
	}

	_finished[here] = true;
}
