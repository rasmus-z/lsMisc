//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>


namespace Ambiesoft {

	//            if(itFirst->IsBeginParen())
	//            {
	//                // find inner
	//                size_t numberOfBegin = 0;
	//                TokenVectorT::iterator itStart = tv.begin() + 1;
	//                TokenVectorT::iterator it = itStart;
	//                for(  ; it != tv.end(); ++it)
	//                {
	//                    // Token<T>& testToken =
	//                    if(it->IsBeginParen())
	//                    {
	//                        ++numberOfBegin;
	//                        continue;
	//                    }
	//                    else if(it->IsEndParen())
	//                    {
	//                        if(numberOfBegin==0)
	//                        {
	//                            // found begin and end paren
	//                            break;
	//                        }
	//                    }
	//                }
	//                if(numberOfBegin != 0)
	//                {
	//                    // paren not match
	//                    throw std::exception();
	//                }
	//                TokenVectorT insideParenVec(itStart, it);

	//            }


	enum TokenType
	{
		TOKEN_BEGINNING_PAREN,
		TOKEN_ENDING_PAREN,
		TOKEN_OR,
		TOKEN_AND,

		TOKEN_WORD,

		TOKEN_PARENT,
	};

	template<typename T>
	class Token
	{
		using TokenT = Token<T>;
		using TokenVectorT = std::vector<TokenT>;

		TokenType tt_;
		T word_;
		TokenVectorT subtokens_;

	public:
		explicit Token(TokenType tt) :
			tt_(tt) {}
		explicit Token(const T& word) :
			tt_(TOKEN_WORD),
			word_(word) {}

		// Ctor for parent token which contains sub tokens
		explicit Token(const TokenVectorT& v) :
			tt_(TOKEN_PARENT),
			subtokens_(v) {}

		// copy
		explicit Token(const TokenT& that) :
			tt_(that.tt_),
			word_(that.word_),
			subtokens_(that.subtokens_)
		{}

		// Equal Operator
		TokenT& operator=(const TokenT& that) {
			if (this != &that)
			{
				this->tt_ = that.tt_;
				this->word_ = that.word_;
				this->subtokens_ = that.subtokens_;
			}
			return *this;
		}

		bool IsOperator() const {
			return tt_ == TOKEN_AND || tt_ == TOKEN_OR;
		}
		bool IsAnd() const {
			return tt_ == TOKEN_AND;
		}
		bool IsOr() const {
			return tt_ == TOKEN_OR;
		}
		bool IsBeginParen() const {
			return tt_ == TOKEN_BEGINNING_PAREN;
		}
		bool IsEndParen() const {
			return tt_ == TOKEN_ENDING_PAREN;
		}
		bool IsWord() const {
			return tt_ == TOKEN_WORD;
		}
		bool IsParent() const {
			return tt_ == TOKEN_PARENT;
		}

		const T& word() const {
			return word_;
		}

		const TokenVectorT& subTokens() const {
			assert(IsParent());
			return subtokens_;
		}
		TokenVectorT subTokensCopy() const {
			assert(IsParent());
			return subtokens_;
		}
	};

	template<typename T>
	class OpParser
	{
		using TokenT = Token<T>;
		using TokenVectorT = std::vector<TokenT>;
		
		const bool defaultNullResult_;

		TokenVectorT tokens_;
		std::function<bool(const T&)> evaluator_;

	public:
		OpParser(std::function<bool(const T&)> evaluator, bool defaultNullResult = false) :
			evaluator_(evaluator),
			defaultNullResult_(defaultNullResult)
		{

		}

		void AddOperator(TokenType tt) {
			tokens_.push_back(TokenT(tt));
		}
		void AddWord(const T& word) {
			tokens_.push_back(TokenT(word));
		}

		bool Evaluate() const {
			TokenVectorT vTmp(tokens_);
			return Evaluate(vTmp);
		}

	private:
		// remove paren
		void Unparen(TokenVectorT& tv) const 
		{
			TokenVectorT::iterator it = tv.begin();
			TokenVectorT::iterator itLastHit = tv.end();
			bool found = false;
			for (; it != tv.end(); ++it)
			{
				if (it->IsBeginParen())
				{
					itLastHit = it;
				}
				else if (it->IsEndParen())
				{
					if (itLastHit == tv.end())
					{
						// not found start paren
						throw std::exception();
					}
					found = true;
					it++;
					break;
				}
			}

			if (!found)
			{
				// no paren found
				// now tokes are liner
				return;
			}
			else
			{
				// paren found
				// create new token which contains insides of paren

				// Create an inside paren token
				// fist create a sub token which cointains tokens inside,
				// then create new vector
				//
				// itLastHit: begining of paren
				// it: end of paren
				TokenVectorT::iterator insideStart = itLastHit + 1;
				TokenVectorT::iterator insideEnd = it - 1;


				TokenVectorT insideVecForToken(insideStart, insideEnd);
				TokenT parentToken(insideVecForToken);

				// now create new vector
				TokenVectorT newVec(tv.begin(), insideStart - 1);
				if(!insideVecForToken.empty())
					newVec.push_back(parentToken);
				newVec.insert(newVec.end(), insideEnd + 1, tv.end());

				// reset our vector
				tv = newVec;

				// dont know whether it's liner
				Unparen(tv);
			}
		}
		bool FindAndTripet(typename TokenVectorT::iterator& it1,
			typename TokenVectorT::iterator& itOp,
			typename TokenVectorT::iterator& it2,
			typename TokenVectorT& tv) const
		{
			// already liner
			assert((tv.size() % 2) == 1);

			if (tv.size() == 1)
				return false;

			typename TokenVectorT::iterator it = tv.begin() + 1;
			for (; it != tv.end(); it += 2)
			{
				if (it->IsAnd())
				{
					it1 = it - 1;
					itOp = it;
					it2 = it + 1;
					return true;
				}
			}
			return false;
		}
		void UnAnd(TokenVectorT& tv) const
		{
			// already liner
			assert((tv.size() % 2) == 1);

			TokenVectorT::iterator it1, itOp, it2;
			if (!FindAndTripet(it1, itOp, it2, tv))
			{
				// no more 'and'
				return;
			}

			TokenVectorT insideVecForToken(it1, it2+1);
			TokenT parentToken(insideVecForToken);
			// now create new vector

			TokenVectorT newVec(tv.begin(), it1);
			newVec.push_back(parentToken);
			newVec.insert(newVec.end(), it2+1, tv.end());

			// reset our vector
			tv = newVec;

			// dont know whether it's liner
			UnAnd(tv);
		}
		bool EvaluateToken(const TokenT& token) const
		{
			if (token.IsWord())
				return evaluator_(token.word());
			else if (token.IsParent())
				return Evaluate(token.subTokensCopy());

			makeThrow();
			return false;
		}
		bool EvaluateAnd(const TokenT& left, const TokenT& right) const
		{
			return EvaluateToken(left) && EvaluateToken(right);
		}
		bool EvaluateOr(const TokenT& left, const TokenT& right) const
		{
			return EvaluateToken(left) || EvaluateToken(right);
		}
		void makeThrow() const {
			throw std::exception();
		}
		bool Evaluate(TokenVectorT& tv) const {
			if (tv.empty())
				return defaultNullResult_;

			// make vector liner, or tree structure
			Unparen(tv);
			
			if (tv.empty())
			{
				// paren only
				return defaultNullResult_;
			}


			// make vecotr Un-And, or make vector all connected with 'or'
			UnAnd(tv);

			assert((tv.size() % 2) == 1);

			// now, all vector is all connected with 'or' only or a single token.
			if (tv.size() == 1)
			{
				if(tv[0].IsWord())
				{
					return evaluator_(tv[0].word());
				}
				if (tv[0].IsParent())
				{
					// Todo: remove Copy by add const everywhere
					TokenVectorT tokenVec = tv[0].subTokensCopy();
					if (tokenVec.size() == 1)
						return EvaluateToken(tokenVec[0]);

					assert(tokenVec.size() == 3);
					assert(tokenVec[1].IsOperator());

					if(tokenVec[1].IsAnd())
						return EvaluateAnd(tokenVec[0], tokenVec[2]);
					else if (tokenVec[1].IsOr())
						return EvaluateOr(tokenVec[0], tokenVec[2]);

					makeThrow();
				}
				assert(false);
				makeThrow();
			}
			TokenVectorT::iterator it = tv.begin() + 1;
			for (; it != tv.end(); it += 2)
			{
				TokenVectorT::iterator it1 = it - 1;
				TokenVectorT::iterator it2 = it + 1;

				if (EvaluateOr(*it1, *it2))
				{
					// true means no more evaluation needs
					return true;
				}
			}
			return false;
		}
	};

} // namespace Ambiesoft