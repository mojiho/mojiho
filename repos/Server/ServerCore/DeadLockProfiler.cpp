#include "pch.h"
#include "DeadLockProfiler.h"

/*------------------------------------
 *		DeadLockProfiler
 ------------------------------------*/

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	//���̵� ã�ų� �߱��Ѵ�.
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

	// ��� �ִ� ���� �־��ٸ�
	if(_lockStack.empty() == false)
	{
		//������ �߰ߵ��� ���� ���̽���� ����� ���� �ٽ� Ȯ��
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

	//������ ���� �� ����

	_discoverdOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	if(_discoverdOrder[here] != -1)
		return;

	_discoverdOrder[here] = _discoveredCount++;

	// ��� ������ ������ ��ȸ �Ѵ�.
	auto findIt = _lockHistory.find(here);
	if(findIt == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;
	for(int32 there : nextSet)
	{
		// ���� �湮�� ���� ���ٸ� �湮�Ѵ�.
		if(_discoverdOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		//here�� there���� ���� �߰ߵǾ��ٸ�, there�� here�� �ļ��̴�. (������ ����)
		if(_discoverdOrder[here] < _discoverdOrder[there])
			continue;

		// �������� �ƴϰ�, Dfs(there)�� ���� �������� �ʾҴٸ�, there�� here�� �����̴�. (������ ����)
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
