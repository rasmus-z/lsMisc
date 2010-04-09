#pragma once

class CBool {
	BOOL b_;
public:
	CBool() : b_(FALSE) {}
	operator BOOL() { return b_; }
	CBool& operator=(BOOL b) {
		b_ = b;
		return *this;
	}
};