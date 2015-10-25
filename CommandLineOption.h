#include <windows.h>
#include <vector>
#include "tstring.h"

class COption
{
	tstring option_;
	size_t argcount_;
protected:
	COption(){}
public:
	COption(tstring option, size_t argcount) :
		option_(option),
		argcount_(argcount) {}
	COption(const COption& option) {
		*this = option;
	}
	const COption& operator=(const COption& option)
	{
		this->option_ = option.option_;
		this->argcount_ = option.argcount_;
		return *this;
	}

};

class CommandLineOptionIterator;
typedef std::vector<COption> OptionVector;
class CommandLineOption
{
	OptionVector theOptions_;
public:
	CommandLineOption() 
	{

	}

	void AddOption(LPCTSTR pOption, size_t argcount);
	void Parse(int argc, TCHAR** targv);
	bool ForEachOption(CommandLineOptionIterator& it);

};

class CommandLineOptionIterator : public COption
{
	size_t cur_;
	const CommandLineOptionIterator& operator=(const COption& op)
	{
		*(COption*)this = op;
		return *this;
	}
public:
	CommandLineOptionIterator():cur_(0){}

	friend CommandLineOption;
};
