#pragma once

class CInt {
	int n_;
public:
	CInt() : n_(0) {}
	operator int() { return n_; }
	CInt& operator=(int n) {
		n_ = n;
		return *this;
	}
};