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

#include <vector>

#include "GetChildWindowBy.h"

namespace Ambiesoft {

	class ScopedDialogDisabler
	{
		std::vector<HWND> hwnds_;
	public:
		ScopedDialogDisabler(const std::vector<HWND> hwnds) :
			hwnds_(hwnds)
		{
			disable();
		}
		ScopedDialogDisabler(HWND hwnd, const BOOL bAllChildWindow)
		{
			if (bAllChildWindow)
			{
				hwnds_ = GetChildWindows(hwnd);
			}
			else
			{
				hwnds_.push_back(hwnd);
			}
			disable();
		}
		virtual ~ScopedDialogDisabler()
		{
			enable();
		}
	private:
		void work(const BOOL bEnable)
		{
			for (HWND hwnd : hwnds_)
			{
				EnableWindow(hwnd, bEnable);
			}
		}
		void disable()
		{
			work(FALSE);
		}
		void enable()
		{
			work(TRUE);
		}
	};
}