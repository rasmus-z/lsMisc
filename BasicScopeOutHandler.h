#pragma once

template<typename U>
class BasicScopeOutHandler {
	U& target_;
	U valueToSet_;

public:
	BasicScopeOutHandler(U& target, const U& valueToSet) :
		target_(target), 
		valueToSet_(valueToSet) {}
	~BasicScopeOutHandler()
	{
		target_ = valueToSet_;
	}
};

template<typename U>
BasicScopeOutHandler<U> makeBasicScopeOutHandler(U& target, const U& valueToSet)
{
	return BasicScopeOutHandler<U>(target, valueToSet);
}
template<typename U>
BasicScopeOutHandler<U> makeBasicScopeOutHandler(U& target, const char* valueToSet)
{
	return BasicScopeOutHandler<U>(target, valueToSet);
}
