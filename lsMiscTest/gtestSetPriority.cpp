#include "stdafx.h"
#include <tchar.h>
#include <string>
#include <gtest/gtest.h>

#include "../SetPrority.h"

using namespace std;
using namespace Ambiesoft;

TEST(SetPriority, Cpu)
{
	int err;
	DWORD id = GetCurrentProcessId();

	CPUPRIORITY cpuPriorityOrig = CPU_NONE;
	CPUPRIORITY cpuPriority = CPU_NONE;
	IOPRIORITY ioPriority;
	MEMORYPRIORITY memPriority;
	err = Ambiesoft::GetPriority(id,
		&cpuPriorityOrig,
		&ioPriority,
		&memPriority);
	EXPECT_EQ(err, 0);
	EXPECT_NE(cpuPriorityOrig, CPU_NONE);

	err = Ambiesoft::SetProirity(id,
		CPUPRIORITY::CPU_ABOVENORMAL,
		IOPRIORITY::IO_NONE,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);
	err = Ambiesoft::GetPriority(id,
		&cpuPriority,
		&ioPriority,
		&memPriority);
	EXPECT_EQ(cpuPriority, CPU_ABOVENORMAL);

	err = Ambiesoft::SetProirity(id,
		cpuPriorityOrig,
		IOPRIORITY::IO_NONE,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);
	err = Ambiesoft::GetPriority(id,
		&cpuPriority,
		&ioPriority,
		&memPriority);
	EXPECT_EQ(cpuPriority, cpuPriorityOrig);

}
TEST(SetPriority, IO)
{
	DWORD id = GetCurrentProcessId();
	int err;
	IOPRIORITY ioPriorityOrig;

	// Get Current IO Priority
	err = Ambiesoft::GetPriority(id,
		NULL,
		&ioPriorityOrig,
		NULL);
	EXPECT_EQ(err, 0);
	EXPECT_NE(ioPriorityOrig, IO_NONE);


	// Set IO as Idle and check
	err = Ambiesoft::SetProirity(id,
		CPUPRIORITY::CPU_NONE,
		IOPRIORITY::IO_IDLE,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);

	IOPRIORITY ioPriority;
	err = Ambiesoft::GetPriority(id,
		NULL,
		&ioPriority,
		NULL);
	EXPECT_EQ(err, 0);
	EXPECT_EQ(ioPriority, IO_IDLE);

	
	// Set back IO andcheck
	err = Ambiesoft::SetProirity(id,
		CPUPRIORITY::CPU_NONE,
		ioPriorityOrig,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);

	err = Ambiesoft::GetPriority(id,
		NULL,
		&ioPriority,
		NULL);
	EXPECT_EQ(err, 0);
	EXPECT_EQ(ioPriorityOrig, ioPriority);



}