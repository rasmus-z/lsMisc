//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "StdAfx.h"

#include <Windows.h>

#include <winternl.h>

#include <cassert>

#include "SetPrority.h"

using namespace std;

namespace Ambiesoft {
    namespace {
        struct NativeHandle
        {
            DWORD dwLastError_;
            HANDLE h_;

            NativeHandle(HANDLE h) :
                dwLastError_(::GetLastError()),
                h_(h)
            {}
            ~NativeHandle()
            {
                if(h_)
                    CloseHandle(h_);
            }
            operator bool() const {
                return h_ != NULL;
            }
            DWORD lastError() const {
                return dwLastError_;
            }
            operator HANDLE() const {
                return h_;
            }
        };

        // http://blog.misterfoo.com/2010/07/process-priority-utility.html
        // these values determined by poking around in the debugger - use at your own risk!
        const DWORD ProcessInformationMemoryPriority = 0x27;
        const DWORD ProcessInformationIoPriority = 0x21;

        const DWORD DefaultMemoryPriority = 5;
        const DWORD LowMemoryPriority = 3;

        const DWORD DefaultIoPriority = 2;
        const DWORD LowIoPriority = 1;
        const DWORD VeryLowIoPriority = 0;


        typedef NTSTATUS (NTAPI *FNNtSetInformationProcess)(
                HANDLE process,
                ULONG infoClass,
                void* data,
                ULONG dataSize);
		typedef NTSTATUS (NTAPI *FNNtQueryInformationProcess)(
			 HANDLE               ProcessHandle,
			 PROCESSINFOCLASS     ProcessInformationClass,
			PVOID               ProcessInformation,
			 ULONG                ProcessInformationLength,
			PULONG ReturnLength OPTIONAL
		);
		struct NTFuncs
		{
			FNNtQueryInformationProcess fnNtQueryInformationProcess = NULL;
			FNNtSetInformationProcess fnNtSetInformationProcess = NULL;
			HMODULE hDLL_ = NULL;
			NTFuncs()
			{
				hDLL_ = LoadLibrary(L"ntdll.dll");
				if (hDLL_)
				{
					fnNtQueryInformationProcess = (FNNtQueryInformationProcess)GetProcAddress(
						hDLL_,
						"NtQueryInformationProcess");
					fnNtSetInformationProcess = (FNNtSetInformationProcess)GetProcAddress(
						hDLL_,
						"NtSetInformationProcess");
				}
			}
			~NTFuncs()
			{
				if (hDLL_)
					FreeLibrary(hDLL_);
			}
			int GetPriority(HANDLE hProcess,
				CPUPRIORITY* cpuPriority,
				IOPRIORITY* ioPriority,
				MEMORYPRIORITY* memPriority)
			{
				int firsterr = 0;
				if (cpuPriority)
				{
					DWORD dwPriortyClass = GetPriorityClass(hProcess);
					if (dwPriortyClass == 0)
					{
						firsterr = static_cast<int>(GetLastError());
					}
					switch (dwPriortyClass)
					{
					case HIGH_PRIORITY_CLASS: *cpuPriority = CPU_HIGH; break;
					case ABOVE_NORMAL_PRIORITY_CLASS: *cpuPriority = CPU_ABOVENORMAL; break;
					case NORMAL_PRIORITY_CLASS: *cpuPriority = CPU_NORMAL; break;
					case BELOW_NORMAL_PRIORITY_CLASS: *cpuPriority = CPU_BELOWNORMAL; break;
					case IDLE_PRIORITY_CLASS: *cpuPriority = CPU_IDLE; break;
					default: *cpuPriority = CPU_UNKNOWN; break;
					}
				}

				// TODO: implement
				//if(ioPriority && fnNtQueryInformationProcess)
				//{ 
				//	fnNtQueryInformationProcess(hProcess,
				//		)
				//}
				if (ioPriority)
					*ioPriority = IO_UNKOWN;
				if (memPriority)
					*memPriority = MEMORY_UNKNOWN;
				return firsterr;
			}
            int SetPriority(HANDLE hProcess,
                             CPUPRIORITY cpuPriority,
                             IOPRIORITY ioPriority,
                             MEMORYPRIORITY memPriority)
            {
                DWORD dwProcessPriority = -1;
                switch(cpuPriority)
                {
				case CPU_NONE: break;
                case CPU_HIGH: dwProcessPriority = HIGH_PRIORITY_CLASS;break;
                case CPU_ABOVENORMAL: dwProcessPriority = ABOVE_NORMAL_PRIORITY_CLASS;break;
                case CPU_NORMAL:dwProcessPriority=NORMAL_PRIORITY_CLASS;break;
                case CPU_BELOWNORMAL:dwProcessPriority=BELOW_NORMAL_PRIORITY_CLASS;break;
                case CPU_IDLE:dwProcessPriority=IDLE_PRIORITY_CLASS;break;
				default:assert(false); break;
                }


                int firstError = 0;
                if(!SetPriorityClass(hProcess, dwProcessPriority))
                {
                    DWORD dwLastError = GetLastError();
//                    ss <<
//                             "SetPriorityClass(" <<
//                             dwProcessPriority <<
//                             ") failed with " <<
//                             dwLastError <<
//                             ")" <<
//                             std::endl;
                    if(firstError==0)
                        firstError = static_cast<int>(dwLastError);
                }

                ULONG nativeIOPriority = -1;
                switch(ioPriority)
                {
				case IO_NONE:break;
				case IO_HIGH:
				case IO_ABOVENORMAL:
                case IO_NORMAL: nativeIOPriority = DefaultIoPriority; break;
                case IO_BELOWNORMAL:
                case IO_IDLE: nativeIOPriority = VeryLowIoPriority; break;
				default:assert(false); break;
                }

				ULONG nativeMemoryPriority = -1;
				switch (memPriority)
				{
				case MEMORY_NONE:break;
				case MEMORY_HIGH:
				case MEMORY_ABOVENORMAL:
				case MEMORY_NORMAL:	nativeMemoryPriority = DefaultMemoryPriority;break;
				case MEMORY_BELOWNORMAL:
				case MEMORY_IDLE:nativeMemoryPriority = LowMemoryPriority;break;
				default:assert(false); break;
				}

                if (fnNtSetInformationProcess)
                {
                    if(nativeMemoryPriority != (ULONG)-1)
                    {
                        NTSTATUS ntStatus = fnNtSetInformationProcess(
                                    hProcess,
                                    ProcessInformationMemoryPriority,
                                    &nativeMemoryPriority,
                                    sizeof(nativeMemoryPriority));
                        if(ntStatus != 0)
                        {
//                            ss <<
//                                      "NtSetInformationProcess(" <<
//                                      ProcessInformationMemoryPriority <<
//                                      "," <<
//                                      nativeMemoryPriority <<
//                                      ") failed with " <<
//                                      ntStatus <<
//                                      "." <<
//                                      std::endl;
                            if(firstError==0)
                                firstError=static_cast<int>(ntStatus);
                        }
                    }

                    if(nativeIOPriority != (ULONG)-1)
                    {
                        NTSTATUS ntStatus = fnNtSetInformationProcess(
                                    hProcess,
                                    ProcessInformationIoPriority,
                                    &nativeIOPriority,
                                    sizeof(nativeIOPriority));
                        if(ntStatus != 0)
                        {
//                            ss <<
//                                      "NtSetInformationProcess(" <<
//                                      ProcessInformationIoPriority <<
//                                      "," <<
//                                      nativeIOPriority <<
//                                      ") failed with " <<
//                                      ntStatus <<
//                                      "." <<
//                                      std::endl;
                            if(firstError==0)
                                firstError=static_cast<int>(ntStatus);
                        }
                    }
                }
                return firstError;
            }
        } ;
    } // anonymous namespace

	int GetPriority(uint64_t  pid,
		CPUPRIORITY* cpuPriority,
		IOPRIORITY* ioPriority,
		MEMORYPRIORITY* memPriority)
	{
		NativeHandle handle(OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, (DWORD)pid));
		if (!handle)
		{
			// std::stringstream ss(error);
			// ss << "Failed to OpenProcess(" << pid << ")." << std::endl;
			return handle.lastError();
		}
		static NTFuncs stNTFuncs;
		return stNTFuncs.GetPriority(
			handle,
			cpuPriority,
			ioPriority,
			memPriority);

	}
    int SetProirity(uint64_t pid,
		CPUPRIORITY cpuPriority,
		IOPRIORITY ioPriority,
        MEMORYPRIORITY memPriority)
    {
        NativeHandle handle(OpenProcess(PROCESS_SET_INFORMATION, FALSE, (DWORD)pid));
        if (!handle)
        {
            // std::stringstream ss(error);
            // ss << "Failed to OpenProcess(" << pid << ")." << std::endl;
            return handle.lastError();
        }
        static NTFuncs stNTFuncs;
        return stNTFuncs.SetPriority(
			handle,
			cpuPriority,
			ioPriority,
            memPriority);
    }
} // namespace Ambiesoft
