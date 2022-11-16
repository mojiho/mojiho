#pragma once
#include "Types.h"

/*------------------------------------
 *		Read Write SpinLock
 ------------------------------------*/

 /*--------------------------------------------------------------
  * [WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
  * W : WriteFlag (Exclusive Lock Owner ThreadId)
  *	R : ReadFlag (Shared Lock Count)
  -------------------------------------------------------------*/

/* Lock 정책*/
// W -> R (o)
// R -> W (x)

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIME_TICK = 10000, // 최대 대기 틱
		MAX_SPIN_COUNT = 5000, // 스핀을 최대 몇번까지 돌것인지
		WRITE_THREAD_MASK = 0xFFFF'0000, // 상위 16 비트를 가져오기 위한 마스크
		READ_COUNT_MASK = 0x0000'FFFF, // 하위 16 비트를 가져오기 위한 마스크
		EMPTY_FLAG = 0x0000'0000 // 초기 플래그 비어있는 상태
	};

public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/*------------------------------------
 *		     LockGuards
 ------------------------------------*/


class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;
};


class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;
};