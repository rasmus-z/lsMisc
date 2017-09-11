#pragma once

namespace Ambiesoft {

template<class T>
class BasicType
{
	T value_;
public:
	BasicType() : value_(0) {}
	operator T() const {
		return value_;
	}
	const T& operator=(const T& value) {
		value_=value;
		return *this;
	}
	T* operator&() {
		return &value_;
	}
};

class CBool {
	BOOL b_;
public:
	CBool() : b_(FALSE) {}
	operator BOOL() const { return b_; }
	CBool& operator=(BOOL b) {
		b_ = b;
		return *this;
	}
	BOOL* operator&() {
		return &b_;
	}
};

}