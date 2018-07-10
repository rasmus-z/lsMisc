#include "stdafx.h"

#include <gtest/gtest.h>

#include "../OpParser.h"

using namespace Ambiesoft::OpParser;
using namespace std;

namespace {
	vector<wstring> gcalledWords;
	int gcallcount;
	bool myEvaluator(const wstring& word)
	{
		gcalledWords.push_back(word);
		++gcallcount;
		if (word == L"true")
			return true;
		return false;
	}
}

TEST(OpParser, Null)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 0);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator, true);
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 0);
	}
}
TEST(OpParser, ParenOnly)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddEndingParenthesis();
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 0);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 0);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 0);
	}
}
TEST(OpParser, BasicSingle)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"false");
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
}
TEST(OpParser, BasicAnd)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"true");

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"false");

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"true");

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"false");

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
}

TEST(OpParser, BasicParenAnd)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
}
TEST(OpParser, BasicOr)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddWord(L"true");

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddWord(L"false");

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"true");

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"false");

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
}
TEST(OpParser, BasicParenOr)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
}
TEST(OpParser, BasicMultiParen)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}

	// Same as above but 'OR'
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();

		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
}

TEST(OpParser, ComplexNullParen)
{
	// "() and ()"
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddEndingParenthesis();
		opp.AddAnd();
		opp.AddBeginningParenthesis();
		opp.AddEndingParenthesis();
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 0);
	}

	// "(() and ())"
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddBeginningParenthesis();
		opp.AddEndingParenthesis();
		opp.AddAnd();
		opp.AddBeginningParenthesis();
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 0);
	}
}

TEST(OpParser, ComplexSingleWord)
{
	// "true and true and false"
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"false");
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 3);
	}

	// "false and true and true"
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"true");
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}

	// "true and false and true"
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"false");
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}
	
	// "true or false and true" => true
	// Evalution is once
	{
		// First, try with C++'s || and &&
		gcalledWords.clear();
		gcallcount = 0;
		EXPECT_TRUE(myEvaluator(L"true") || myEvaluator(L"false") && myEvaluator(L"true"));
		EXPECT_EQ(gcallcount, 1);
		EXPECT_EQ(gcalledWords.size(), (size_t)1);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"true");

		// Must be same with C++ 
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"true");
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
		EXPECT_EQ(gcalledWords.size(), (size_t)1);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"true");
	}

	// "false or false and true" => false
	// Evalution is twice.
	{
		// First, try with C++'s || and &&
		gcalledWords.clear();
		gcallcount = 0;
		EXPECT_FALSE(myEvaluator(L"false") || myEvaluator(L"false") && myEvaluator(L"true"));
		EXPECT_EQ(gcallcount, 2);
		EXPECT_EQ(gcalledWords.size(), (size_t)2);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[1].c_str(), L"false");

		// Must be same with C++ 
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"true");
		EXPECT_FALSE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
		EXPECT_EQ(gcalledWords.size(), (size_t)2);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[1].c_str(), L"false");

	}
}
TEST(OpParser, ComplexParen)
{
	// "true or (false and true)" => true
	// Evalution is once
	{
		// First, try with C++'s || and &&
		gcalledWords.clear();
		gcallcount = 0;
		EXPECT_TRUE(myEvaluator(L"true") || ( myEvaluator(L"false") && myEvaluator(L"true") ));
		EXPECT_EQ(gcallcount, 1);
		EXPECT_EQ(gcalledWords.size(), (size_t)1);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"true");

		// Must be same with C++ 
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
		EXPECT_EQ(gcalledWords.size(), (size_t)1);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"true");
	}

	// "(false or true) and true" => true
	// Evalution is twice.
	{
		// First, try with C++'s || and &&
		gcalledWords.clear();
		gcallcount = 0;
		EXPECT_TRUE((myEvaluator(L"false") || myEvaluator(L"true")) && myEvaluator(L"true"));
		EXPECT_EQ(gcallcount, 3);
		EXPECT_EQ(gcalledWords.size(), (size_t)3);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[1].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[2].c_str(), L"true");

		// Must be same with C++ 
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();
		opp.AddAnd();
		opp.AddWord(L"true");
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 3);
		EXPECT_EQ(gcalledWords.size(), (size_t)3);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[1].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[2].c_str(), L"true");
	}

}

TEST(OpParser, ComplexMultiParen)
{
	// "true or (false and (false and true))" => true
	// Evalution is once
	{
		// First, try with C++'s || and &&
		gcalledWords.clear();
		gcallcount = 0;
		EXPECT_TRUE(
			myEvaluator(L"true") || 
				(myEvaluator(L"false") && 
					(myEvaluator(L"false") && myEvaluator(L"true")) 
				) 
		);
		EXPECT_EQ(gcallcount, 1);
		EXPECT_EQ(gcalledWords.size(), (size_t)1);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"true");

		// Must be same with C++ 
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		opp.AddOr();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddAnd();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();

		
		
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
		EXPECT_EQ(gcalledWords.size(), (size_t)1);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"true");
	}

	// "(False or True) and (False or (True and False))" => False
	// Evalution is twice.
	{
		// First, try with C++'s || and &&
		gcalledWords.clear();
		gcallcount = 0;
		EXPECT_FALSE(
			(myEvaluator(L"false") || myEvaluator(L"true")) &&
				(myEvaluator(L"false") ||
					(myEvaluator(L"true") && myEvaluator(L"false"))
				)
		);
		EXPECT_EQ(gcallcount, 5);
		EXPECT_EQ(gcalledWords.size(), (size_t)5);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[1].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[2].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[3].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[4].c_str(), L"false");

		// Must be same with C++ 
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		// "" => False
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();
		opp.AddAnd();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();


		EXPECT_FALSE(opp.Evaluate());

		EXPECT_EQ(gcallcount, 5);
		EXPECT_EQ(gcalledWords.size(), (size_t)5);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[1].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[2].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[3].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[4].c_str(), L"false");

	}

}

TEST(OpParser, ImplicitAndError)
{
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator);
		opp.AddWord(L"true");
		EXPECT_THROW(opp.AddWord(L"true"), std::exception);

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 1);
	}
	{
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator, false, true);
		opp.AddWord(L"true");
		opp.AddWord(L"true");

		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gcallcount, 2);
	}

	// Same as above, but use implicitAnd.
	// "(False or True) (False or (True False))" => False
	// Evalution is twice.
	{
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator,false,true);
		// "" => False
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddWord(L"true");
		opp.AddEndingParenthesis();
		// opp.AddAnd();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		// opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();


		EXPECT_FALSE(opp.Evaluate());

		EXPECT_EQ(gcallcount, 5);
		EXPECT_EQ(gcalledWords.size(), (size_t)5);
		EXPECT_STREQ(gcalledWords[0].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[1].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[2].c_str(), L"false");
		EXPECT_STREQ(gcalledWords[3].c_str(), L"true");
		EXPECT_STREQ(gcalledWords[4].c_str(), L"false");

	}

}

TEST(OpParser, UnmatchedParenthesisError)
{
	{
		OpParser<wstring> opp(myEvaluator, false, true);
		opp.AddBeginningParenthesis();
		EXPECT_THROW(opp.TryEvaluate(), OpParserError);
	}
	{
		gcalledWords.clear();
		gcallcount = 0;
		OpParser<wstring> opp(myEvaluator, false, true);
		// "" => False
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();

		// illegal
		opp.AddBeginningParenthesis();

		opp.AddWord(L"true");
		opp.AddEndingParenthesis();
		// opp.AddAnd();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"false");
		opp.AddOr();
		opp.AddBeginningParenthesis();
		opp.AddWord(L"true");
		// opp.AddAnd();
		opp.AddWord(L"false");
		opp.AddEndingParenthesis();
		opp.AddEndingParenthesis();

		EXPECT_THROW(opp.TryEvaluate(), OpParserError);
	}
}

int gCtorCount;
int gAllocDAllocCounter;
TEST(OpParser, Pod)
{
	struct MyPod
	{
		char* p_ = nullptr;
		MyPod() {
			gCtorCount++;
			gAllocDAllocCounter++;
			p_ = new char[100];
			memset(p_, 0, 100);
		}
		~MyPod() {
			if(p_)
				gAllocDAllocCounter--;
			delete p_;
		}

		//MyPod(const MyPod& that) = delete;
		MyPod(const MyPod& that) {
			for (int i = 0; i < 100; ++i)
				EXPECT_EQ(that.p_[i], 0);
			gCtorCount++;
			gAllocDAllocCounter++;
			p_ = new char[100];
			memcpy(p_, that.p_, 100);
		}
		MyPod(MyPod&& that) {
			for (int i = 0; i < 100; ++i)
				EXPECT_EQ(that.p_[i], 0);
			p_ = that.p_;
			that.p_ = nullptr;
		}

		// const MyPod& operator=(const MyPod& that) = delete;
		const MyPod& operator=(const MyPod& that) {
			if (this != &that)
			{
				memcpy(p_, that.p_, 100);
			}
			return *this;
		}
		const MyPod& operator=(MyPod&& that) {
			p_ = that.p_;
			that.p_ = nullptr;
			return *this;
		}

		static bool evaluate(const MyPod& that) {
			for (int i = 0; i < 100; ++i)
			{
				if (that.p_[i] != 0)
				{
					return false;
				}
			}
			return true;
		}
		static bool evaluate_shared(const shared_ptr<MyPod>& that) {
			for (int i = 0; i < 100; ++i)
			{
				if (that->p_[i] != 0)
				{
					return false;
				}
			}
			return true;
		}
		static bool evaluate_unique(const unique_ptr<MyPod>& that) {
			for (int i = 0; i < 100; ++i)
			{
				if (that->p_[i] != 0)
				{
					return false;
				}
			}
			return true;
		}
		static bool evaluate_pointer(const MyPod* that) {
			for (int i = 0; i < 100; ++i)
			{
				if (that->p_[i] != 0)
				{
					return false;
				}
			}
			return true;
		}
	};

	{
		gCtorCount = 0;
		OpParser<MyPod> opp(&MyPod::evaluate, false, true);
		opp.AddWord(MyPod());
		EXPECT_EQ(gCtorCount, 1);

		gCtorCount = 0;
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gCtorCount, 1);
	}
	{
		gCtorCount = 0;
		OpParser<MyPod> opp(&MyPod::evaluate, false, true);
		MyPod myp;
		opp.AddWord(myp);
		EXPECT_EQ(gCtorCount, 2);

		gCtorCount = 0;
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gCtorCount, 1);
	}

	{
		gCtorCount = 0;
		OpParser<MyPod> opp(&MyPod::evaluate, false, true);
		MyPod myp;
		opp.AddWord(std::move(myp));
		EXPECT_EQ(gCtorCount, 1);

		gCtorCount = 0;
		EXPECT_TRUE(opp.Evaluate());
		EXPECT_EQ(gCtorCount, 1);
	}

	{
		gCtorCount = 0;
		gAllocDAllocCounter = 0;
		{
			OpParser<MyPod> opp(&MyPod::evaluate, false, true);
			opp.AddWord(MyPod());
			opp.AddWord(MyPod());
			opp.AddWord(MyPod());
			EXPECT_GT(gCtorCount, 5);

			gCtorCount = 0;
			EXPECT_TRUE(opp.Evaluate());
			EXPECT_GT(gCtorCount, 5);
		}
		EXPECT_EQ(gAllocDAllocCounter, 0);
	}

	{
		gAllocDAllocCounter = 0;
		{
			gCtorCount = 0;
			OpParser<MyPod*> opp(&MyPod::evaluate_pointer, false, true);
			unique_ptr<MyPod> p1 = make_unique<MyPod>();
			unique_ptr<MyPod> p2 = make_unique<MyPod>();
			unique_ptr<MyPod> p3 = make_unique<MyPod>();
			opp.AddWord(p1.get());
			opp.AddWord(p2.get());
			opp.AddWord(p3.get());
			EXPECT_EQ(gCtorCount, 3);

			gCtorCount = 0;
			EXPECT_TRUE(opp.Evaluate());
			EXPECT_EQ(gCtorCount, 0);
		}
		EXPECT_EQ(gAllocDAllocCounter, 0);
	}

	{
		gAllocDAllocCounter = 0;
		{
			gCtorCount = 0;
			OpParser<shared_ptr<MyPod>> opp(&MyPod::evaluate_shared, false, true);
			opp.AddWord(make_shared<MyPod>());
			opp.AddWord(make_shared<MyPod>());
			opp.AddWord(make_shared<MyPod>());
			EXPECT_EQ(gCtorCount, 3);

			gCtorCount = 0;
			EXPECT_TRUE(opp.Evaluate());
			EXPECT_EQ(gCtorCount, 0);
		}
		EXPECT_EQ(gAllocDAllocCounter, 0);
	}

	new int;
	//opp.AddOr();
	//opp.AddWord(MyPod());
	//opp.AddOr();
	//opp.AddWord(MyPod());

	
}