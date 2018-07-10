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
	namespace OpParser {

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

		namespace {
			enum TokenType
			{
				TOKEN_NONE,

				TOKEN_BEGINNING_PARENTHESIS,
				TOKEN_ENDING_PARENTHESIS,
				TOKEN_OR,
				TOKEN_AND,

				TOKEN_WORD,

				TOKEN_PARENT,
			};
		}

		template<typename T>
		class Token
		{
			using TokenT = Token<T>;
			using TokenVectorT = std::vector<TokenT>;


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
				return tt_ == TOKEN_BEGINNING_PARENTHESIS;
			}
			bool IsEndParen() const {
				return tt_ == TOKEN_ENDING_PARENTHESIS;
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

			// TODO: Remove this in the future.
			TokenVectorT subTokensCopy() const {
				assert(IsParent());
				return subtokens_;
			}

		private:
			// Constructed by ctor from user, or
			// while evaluating, it will become Parent.
			TokenType tt_;

			// User's operand.
			T word_;

			// When token type is Parent, this is set.
			// All '()' and 'A AND B' will be subtoken.
			TokenVectorT subtokens_;
		};

		template<typename T>
		class OpParser
		{
			using TokenT = Token<T>;
			using OpParserT = OpParser<T>;
			using TokenVectorT = std::vector<TokenT>;

			enum ThrowType {
				Throw_Unknow,
				Throw_SysntaxErrorBeginningParenthesis,
				Throw_SysntaxErrorAnd,
				Throw_SysntaxErrorOr,
				Throw_SysntaxErrorEndingParenthesis,
				Throw_SysntaxErrorWord,
			};

			void overMe(const OpParserT& that) {
				nullResult_ = that.nullResult_;
				implicitAnd_ = that.implicitAnd_;
				evaluator_ = that.evaluator_;
				tokens_ = that.tokens_;
				lastAddedTokenType_ = that.lastAddedTokenType_;
			}
		public:
			explicit OpParser() {}
			explicit OpParser(std::function<bool(const T&)> evaluator,
				bool nullResult = false,
				bool implicitAnd = false) :
				evaluator_(evaluator),
				nullResult_(nullResult),
				implicitAnd_(implicitAnd){}
		
			explicit OpParser(const OpParserT& that) {
				overMe(that);
			}
			const OpParserT& operator=(const OpParserT& that) {
				if (this != &that) {
					overMe(that);
				}
				return *this;
			}

			bool empty() const {
				return !evaluator_ && tokens_.empty();
			}
			void AddBeginningParenthesis() {
				// 'A ( ...' is illegal.
				// 'implicit: 'A AND ( ...' is legal
				if (lastAddedTokenType_ == TOKEN_WORD)
				{
					if (implicitAnd_)
						AddAnd();
					else
						makeThrow(Throw_SysntaxErrorBeginningParenthesis);
				}
				// ') (' is illegal.
				// implicit: ') and (' is legal.
				if(lastAddedTokenType_ == TOKEN_ENDING_PARENTHESIS)
				{ 
					if (implicitAnd_)
						AddAnd();
					else
						makeThrow(Throw_SysntaxErrorBeginningParenthesis);
				}

				tokens_.push_back(TokenT(lastAddedTokenType_=TOKEN_BEGINNING_PARENTHESIS));
			}
			void AddAnd() {
				// 'AND ...' with 'AND' is illegal.
				// '( AND ...' is illegal.
				if (lastAddedTokenType_ == TOKEN_NONE ||
					lastAddedTokenType_ == TOKEN_BEGINNING_PARENTHESIS)
				{
					makeThrow(Throw_SysntaxErrorAnd);
				}

				tokens_.push_back(TokenT(lastAddedTokenType_ = TOKEN_AND));
			}
			void AddOr() {
				// 'OR ...' is illegal.
				// implicit: 'AND OR ...' is still illegal.
				if (lastAddedTokenType_ == TOKEN_NONE)
				{
					makeThrow(Throw_SysntaxErrorOr);
				}
				
				// '( OR ...' is illegal.
				// '( AND OR ...' is still illegal.
				if(lastAddedTokenType_ == TOKEN_BEGINNING_PARENTHESIS)
				{
					makeThrow(Throw_SysntaxErrorOr);
				}
				tokens_.push_back(TokenT(lastAddedTokenType_ = TOKEN_OR));
			}
			void AddEndingParenthesis() {
				// ') ...' is illegal.
				// implicat: 'AND ) ...' is still illegal.
				if (lastAddedTokenType_ == TOKEN_NONE)
					makeThrow(Throw_SysntaxErrorEndingParenthesis);

				// 'AND )' is illegal.
				// implicit: 'AND AND )' is illegal.
				if (lastAddedTokenType_ == TOKEN_AND)
					makeThrow(Throw_SysntaxErrorEndingParenthesis);

				tokens_.push_back(TokenT(lastAddedTokenType_ = TOKEN_ENDING_PARENTHESIS));
			}

			void AddWord(const T& word) {
				// 'A A' is illegal
				// implicit: 'A and A' is legal.
				if (lastAddedTokenType_ == TOKEN_WORD)
				{
					if (implicitAnd_)
						AddAnd();
					else
						makeThrow(Throw_SysntaxErrorWord);
				}
				lastAddedTokenType_ = TOKEN_WORD;
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
					if (!insideVecForToken.empty())
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

				TokenVectorT insideVecForToken(it1, it2 + 1);
				TokenT parentToken(insideVecForToken);
				// now create new vector

				TokenVectorT newVec(tv.begin(), it1);
				newVec.push_back(parentToken);
				newVec.insert(newVec.end(), it2 + 1, tv.end());

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
				else if (token.IsOperator())
					return nullResult_;

				makeThrow(Throw_Unknow);
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
			void makeThrow(ThrowType) const {
				throw std::exception();
			}
			bool Evaluate(TokenVectorT& tv) const {
				if (tv.empty())
					return nullResult_;

				// make vector liner, or tree structure
				Unparen(tv);

				if (tv.empty())
				{
					// paren only
					return nullResult_;
				}


				// make vecotr Un-And, or make vector all connected with 'or'
				UnAnd(tv);

				assert((tv.size() % 2) == 1);

				// now, all vector is all connected with 'or' only or a single token.
				if (tv.size() == 1)
				{
					if (tv[0].IsWord())
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

						if (tokenVec[1].IsAnd())
							return EvaluateAnd(tokenVec[0], tokenVec[2]);
						else if (tokenVec[1].IsOr())
							return EvaluateOr(tokenVec[0], tokenVec[2]);

						makeThrow(Throw_Unknow);
					}
					if (tv[0].IsOperator())
						return nullResult_;

					assert(false);
					makeThrow(Throw_Unknow);
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
			

		private:
			// When no equations are provided like '', '()' and '(()())',
			// this value is returned.
			// Initialized at ctor.
			const bool nullResult_ = false;

			const bool implicitAnd_ = false;

			// The main tokens
			TokenVectorT tokens_;

			// User defined evaluator.
			std::function<bool(const T&)> evaluator_;

			// Last added token type, set by Add* mothed, for
			// check syntax.
			TokenType lastAddedTokenType_ = TokenType::TOKEN_NONE;
		};

	} // namespace OpParser
} // namespace Ambiesoft