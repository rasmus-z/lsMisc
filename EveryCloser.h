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
