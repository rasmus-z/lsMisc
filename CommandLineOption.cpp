
#include "CommandLineOption.cpp.h"

void CommandLineOption::AddOption(LPCTSTR pOption, size_t argcount)
{

	theOptions_.push_back(COption(pOption, argcount));
}

bool CommandLineOption::ForEachOption(CommandLineOptionIterator& it)
{
	if( it.cur_ > theOptions_.size() )
		return false;

	it = theOptions_[it.cur_];
	return true;
}

void CommandLineOption::Parse(int argc, TCHAR** targv)
{
	for(int i=1 ; i < argc ; ++i)
	{
		LPCTSTR pOption = targv[i];
		bool found = false;
		for(OptionVector::iterator it= theOptions_.begin();
									it != theOptions_.end();
									++it)
		{
			ff;
			if(it->option_ == pOption)
			{

		}
}