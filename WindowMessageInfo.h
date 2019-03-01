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

#include <Windows.h>

namespace Ambiesoft {
	template<DWORD M>
	class WindowMessageInfo;

	template<>
	class WindowMessageInfo<WM_MOUSEWHEEL>
	{
	public:
		static short delta(const WPARAM& w, const LPARAM& l)
		{
			return GET_WHEEL_DELTA_WPARAM(w);
		}
		static short delta(System::Windows::Forms::Message^ message)
		{
			return GET_WHEEL_DELTA_WPARAM((WPARAM)message->WParam.ToPointer());
		}
		static System::Drawing::Point point(System::Windows::Forms::Message^ message)
		{
			return System::Drawing::Point(
				(LOWORD((LPARAM)message->LParam.ToPointer())),
				(HIWORD((LPARAM)message->LParam.ToPointer()))
				);
		}
	};
}