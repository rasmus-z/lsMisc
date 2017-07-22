#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>

#include "CommandLineParser.h"

#ifdef __linux__ 
#elif _WIN32
	#include <Shellapi.h>
#else
#endif

using std::vector;

namespace Ambiesoft {

	COption::COption(tstring option, unsigned long acf) : hadOption_(false)
	{
		options_.push_back(option);
		argcountflag_ = acf;
	}
	COption::COption(tstring option) : hadOption_(false)
	{
		options_.push_back(option);
		argcountflag_ = 0;
	}

	tstring COption::getFirstOption() const
	{
		return options_[0];
	}
	bool COption::hadOption() const {
		return hadOption_;
	}
	bool COption::hadValue() const {
		return !values_.empty();
	}

	tstring COption::getValueStrings() const {
		tstring ret;
		bool looped = false;
		for (std::vector<tstring>::const_iterator it = values_.begin(); it != values_.end(); ++it)
		{
			if (looped)
			{
				ret += _T(" ");
			}
			looped = true;
			ret += *it;
		}
		return ret;
	}
	tstring COption::getFirstValue() const {
		tstring ret;
		if (values_.empty())
			return ret;
		return values_[0];
	}
	size_t COption::getValueCount() const {
		return values_.size();
	}
	void COption::AddValue(const tstring& value)
	{
		setHadOption();
		values_.push_back(value);
	}

	void COption::setHadOption()
	{
		hadOption_ = true;
	}

	bool COption::isMatchOption(const tstring& option)
	{
		vector< tstring >::iterator cIter = find(options_.begin(), options_.end(), option);
		return cIter != options_.end();
	}
}

} // namespace