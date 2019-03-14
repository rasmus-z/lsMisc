#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <cassert>

#include <gtest/gtest.h>

#include "../Lock.h"
#include "debug.h"

using namespace Ambiesoft;
using namespace std;

TEST(Locker, APITest)
{
	{
		CReadWriteLock<CriticalSectionTrait> lock;
		CReadWriteLock<CriticalSectionTrait>::CReadWriteLocker locker(lock);
	}
	{
		CReadWriteLock<SRWTrait> lock;
		CReadWriteLock<SRWTrait>::CReadWriteLocker locker(lock);
	}
	
	{
		CReadWriteLock<DynamicTrait> lock;
		CReadWriteLock<DynamicTrait>::CReadWriteLocker locker(lock);
	}
	{
		using MyLock = CReadWriteLock<DynamicTrait>;
		MyLock lock;
		MyLock::CReadWriteLocker lockerR1(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
		MyLock::CReadWriteLocker lockerR2(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
		// MyLock::CReadWriteLocker lockerW1(lock, MyLock::CReadWriteLocker::LockType::READWRITE_LOCK);
	}
}
