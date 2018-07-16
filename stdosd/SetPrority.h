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

#pragma once

#include <string>
#include <stdint.h>

namespace Ambiesoft {
	namespace stdosd {
		namespace Process {

			enum CPUPRIORITY : int {
				CPU_UNKNOWN = -100,

				CPU_NONE = 0,

				CPU_HIGH,
				CPU_ABOVENORMAL,
				CPU_NORMAL,
				CPU_BELOWNORMAL,
				CPU_IDLE,
			};
			enum IOPRIORITY : int {
				IO_UNKNOWN = -100,

				IO_NONE = 0,

				IO_HIGH,
				IO_ABOVENORMAL,
				IO_NORMAL,
				IO_BELOWNORMAL,
				IO_IDLE,
			};
			enum MEMORYPRIORITY : int {
				MEMORY_UNKNOWN = -100,

				MEMORY_NONE = 0,

				MEMORY_HIGH,
				MEMORY_ABOVENORMAL,
				MEMORY_NORMAL,
				MEMORY_BELOWNORMAL,
				MEMORY_IDLE,
			};

			int GetPriority(uint64_t  pid,
				CPUPRIORITY* cpuPriority,
				IOPRIORITY* ioPriority,
				MEMORYPRIORITY* memPriority);
			int SetProirity(uint64_t  pid,
				CPUPRIORITY cpuPriority,
				IOPRIORITY ioPriority,
				MEMORYPRIORITY memPriority);
		}
	}
}