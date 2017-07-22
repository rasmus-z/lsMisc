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
using std::find;

namespace Ambiesoft {

	COption::COption(tstring option, unsigned long acf) : hadOption_(false)
	{
		options_.push_back(option);
		argcountflag_ = acf;
	}
	COption::COption(tstring option1, tstring option2, unsigned long acf) : hadOption_(false)
	{
		options_.push_back(option1);
		options_.push_back(option2);
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

	bool COption::isMatchOption(const tstring& option) const
	{
		vector< tstring >::const_iterator cIter = find(options_.begin(), options_.end(), option);
		return cIter != options_.end();
	}

















	tstring CCommandLineParser::GetToken(LPCTSTR p)
	{
		tstring ret;
		if (!p || !p[0])
			return ret;

		bool dq = false;
		bool sq = false;

		for (; *p; ++p)
		{
			if (_istspace(*p))
			{
				if (!dq && !sq)
					break;
			}
			else if (*p == _T('"'))
			{
				if (!sq)
				{
					dq = true;
					continue;
				}
			}
			else if (*p == _T('\''))
			{
				if (!dq)
				{
					sq = true;
					continue;
				}
			}

			ret += *p;
		}
		return ret;
	}


	COption* CCommandLineParser::FindAvailableCL(const tstring& option)
	{
		for (POPTIONARRAY::iterator it = availables_.begin(); it != availables_.end(); ++it)
		{
			if ((*it)->isMatchOption(option))
			{
				return *it;
			}
		}
		return NULL;
	}



	CCommandLineParser::CCommandLineParser()
	{
		empty_ = true;
	}
	bool CCommandLineParser::isEmpty() const {
		return empty_;
	}
	tstring CCommandLineParser::getUnknowOptionStrings() const {
		tstring ret;
		for (OPTIONARRAY::const_iterator it = unknowns_.begin(); it != unknowns_.end(); ++it)
		{
			ret += it->getFirstOption();
			if (!it->hadValue())
			{
				ret += _T(" ");
				ret += it->getValueStrings();
			}
		}
		return ret;
	}
	bool CCommandLineParser::hadUnknownOption() const {
		return !unknowns_.empty();
	}
	void CCommandLineParser::AddOption(COption* cli) {
		availables_.push_back(cli);
	}
#ifdef _WIN32
#ifdef UNICODE
	void CCommandLineParser::Parse()
	{
		LPWSTR *szArglist;
		int nArgs;

		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		if (NULL == szArglist)
		{
			return;
		}
		Parse(nArgs, szArglist);
		LocalFree(szArglist);
	}
#endif
#endif
	void CCommandLineParser::Parse(int argc, LPTSTR* targv)
	{
		//argc_ = argc;
		//targv_ = targv;

		for (int i = 1; i < argc; ++i)
		{
			LPCTSTR pArgv = targv[i];
			if (!pArgv || !pArgv[0])
				continue;

			empty_ = false;

			if (pArgv[0] == _T('-') || pArgv[0] == _T('/'))
			{
				COption* pA = FindAvailableCL(pArgv);
				if (!pA)
				{
					//CInputCommandLineInfo icli;
					//icli.nID_ = -1;
					//icli.option_ = pArgv;
					unknowns_.push_back(COption(pArgv));
					continue;
				}

				pA->setHadOption();

				if (pA->argcountflag_ & 0x1)
				{
					++i;
					if (i >= argc)
					{
						break;
					}

					LPCTSTR pArgv2 = targv[i];
					// pA->option_ = pArgv;
					pA->AddValue(pArgv2);
					continue;
				}
			}
			else
			{
				COption* pA = FindAvailableCL(_T(""));
				if (!pA)
				{
					//CInputCommandLineInfo icli;
					//icli.nID_ = -2;
					//icli.option_ = _T("");
					//icli.value_ = pArgv;
					unknowns_.push_back(COption(pArgv));
					continue;
				}
				else
				{
					//CInputCommandLineInfo icli;
					//icli.nID_ = pA->nID_;
					//icli.option_ = _T("");
					pA->AddValue(pArgv);
					// pA.push_back(icli);
					continue;

				}
			}
		}
	}










} // namespace