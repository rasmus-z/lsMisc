#pragma once


#ifdef UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif


namespace Ambiesoft {


class COption
{
	std::vector<tstring> options_;

	unsigned long argcountflag_;
	std::vector<tstring> values_;
	bool hadOption_;
	void AddValue(const tstring& value);
	void setHadOption();
	bool isMatchOption(const tstring& option) const;
	tstring getFirstOption() const;
public:
	COption(tstring option, unsigned long acf);
	COption(tstring option1, tstring option2, unsigned long acf);
	COption(tstring option);

	friend class CCommandLineParser;
	
	bool hadOption() const;
	bool hadValue() const;

	tstring getValueStrings() const;
	tstring getFirstValue() const;
	size_t getValueCount() const;
};


class CCommandLineParser
{
	static tstring GetToken(LPCTSTR p);
			
	typedef std::vector<COption*> POPTIONARRAY;
	typedef std::vector<COption> OPTIONARRAY;
	POPTIONARRAY availables_;
	OPTIONARRAY unknowns_;
	bool empty_;

	COption* FindAvailableCL(const tstring& option);


public:
	CCommandLineParser();
	bool isEmpty() const;
	tstring getUnknowOptionStrings() const;
	bool hadUnknownOption() const;
	void AddOption(COption* cli);
#ifdef _WIN32
#ifdef UNICODE
	void Parse();
#endif
#endif
	void Parse(int argc, LPTSTR* targv);

};


} // namespace