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

template <typename RETTYPE, typename _1ARG> struct EveryCloser1
{
	typedef RETTYPE (*FUNCTYPE)(_1ARG);

	FUNCTYPE f_;
	_1ARG a1_;
	EveryCloser1(FUNCTYPE f, _1ARG a1) {
		f_ = f;
		a1_ = a1;
	}
	~EveryCloser1() {
		f_(a1_);
	}
};

template <typename RETTYPE> struct WINAPIEveryCloser0
{
	typedef RETTYPE (WINAPI*FUNCTYPE)();

	FUNCTYPE f_;
	WINAPIEveryCloser0(FUNCTYPE f) {
		f_ = f;
	}
	~WINAPIEveryCloser0() {
		f_();
	}
};

template <typename RETTYPE, typename _1ARG> struct WINAPIEveryCloser1
{
	typedef RETTYPE (WINAPI*FUNCTYPE)(_1ARG);

	FUNCTYPE f_;
	_1ARG a1_;
	WINAPIEveryCloser1(FUNCTYPE f, _1ARG a1) {
		f_ = f;
		a1_ = a1;
	}
	~WINAPIEveryCloser1() {
		f_(a1_);
	}
};
