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
#include <list>
#include <functional>
#include <cassert>

namespace Ambiesoft {
    namespace Logic {

		// TODO: Can I remove this?
#define OpParserContainerType std::list

		class OpParserError : public std::runtime_error
		{
		public:
			OpParserError(char const* const message) :
				std::runtime_error(message) {}
		
		};

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
			using TokenVectorT = OpParserContainerType<TokenT>; //std::vector<TokenT>;


		public:
			explicit Token(TokenType tt) :
				tt_(tt) {}
			explicit Token(const T& word) :
				tt_(TOKEN_WORD),
				word_(new T(word))
			{}
			// rval
			//explicit Token(T&& word) :
			//	tt_(std::move(TOKEN_WORD)),
			//	word_(std::move(word)) 
			//{}

			// Ctor for parent token which contains sub tokens
			explicit Token(const TokenVectorT& v) :
				tt_(TOKEN_PARENT),
				subtokens_(v) 
			{}

			// copy
			explicit Token(const TokenT& that) :
				tt_(that.tt_),
				//word_(that.word_),
				subtokens_(that.subtokens_)
			{
				if (that.word_)
					word_ = new T(*that.word_);
			}
			
			// rval copy
			explicit Token(TokenT&& that) :
				tt_(std::move(that.tt_)),
				// word_(std::move(that.word_)),
				subtokens_(std::move(that.subtokens_))
			{
				word_ = that.word_;
				that.word_ = nullptr;
			}

			// dtor
			~Token()
			{
				delete word_;
			}
			// Equal Operator
			TokenT& operator=(const TokenT& that) {
				if (this != &that)
				{
					this->tt_ = that.tt_;
					if(that.word_)
						this->word_ = new T(*that.word_);
					this->subtokens_ = that.subtokens_;
				}
				return *this;
			}
			// rval Equal Operator
			TokenT& operator=(TokenT&& that) {
				if (this != &that)
				{
					this->tt_ = std::move(that.tt_);
					this->word_ = that.word_;
					that.word_ = nullptr;
					this->subtokens_ = std::move(that.subtokens_);
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
				assert(IsWord());
				assert(word_ != nullptr);
				return *word_;
			}

			const TokenVectorT& subTokens() const {
				assert(IsParent());
				return subtokens_;
			}

			// TODO: Remove this in the future.
			//TokenVectorT subTokensCopy() const {
			//	assert(IsParent());
			//	return subtokens_;
			//}

		private:
			// Constructed by ctor from user, or
			// while evaluating, it will become Parent.
			TokenType tt_;

			// User's operand.
			// Much of copy will be performed on T.
			T* word_ = nullptr;

			// When token type is Parent, this is set.
			// All '()' and 'A AND B' will be subtoken.
			TokenVectorT subtokens_;
		};

		template<typename T, typename... Args>
		class OpParser
		{
			using TokenT = Token<T>;
			using OpParserT = OpParser<T, Args...>;
			using TokenVectorT = OpParserContainerType<TokenT>;
			using EvaluatorT = std::function<bool(const T&, Args...)>;

			enum ThrowType {
				Throw_Unknow,
				Throw_SysntaxErrorBeginningParenthesis,
				Throw_SysntaxErrorAnd,
				Throw_SysntaxErrorOr,
				Throw_SysntaxErrorEndingParenthesis,
				Throw_SysntaxErrorWord,

				Throw_SysntaxErrorMismatchedParenthesis,
			};

			void overMe(const OpParserT& that) {
				*(const_cast<bool*>(&nullResult_)) = that.nullResult_;
				*(const_cast<bool*>(&implicitAnd_)) = that.implicitAnd_;
				evaluator_ = that.evaluator_;
				tokens_ = that.tokens_;
				lastAddedTokenType_ = that.lastAddedTokenType_;
			}
		public:
			explicit OpParser() {}
			explicit OpParser(EvaluatorT evaluator,
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
				return tokens_.empty();
			}
			void AddBeginningParenthesis() {
				dirty_ = true;
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
				dirty_ = true;
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
				dirty_ = true;
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
				dirty_ = true;
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
				dirty_ = true;
				PreAddWord();
				lastAddedTokenType_ = TOKEN_WORD;
				tokens_.push_back(TokenT(word));
			}
			// rval
			void AddWord(T&& word) {
				dirty_ = true;
				PreAddWord();
				lastAddedTokenType_ = TOKEN_WORD;
				tokens_.push_back(TokenT(std::move(word)));
			}

			bool Evaluate(Args... args) const {
				if (dirty_)
				{
					parsedTokens_ = tokens_;
					Parse(parsedTokens_);
					dirty_ = false;
				}
				return EvaluateInner(parsedTokens_, false, args...);
			}
			void TryEvaluate(Args...args) const {
				if (dirty_)
				{
					parsedTokens_ = tokens_;
					Parse(parsedTokens_);
					dirty_ = false;
				}
				EvaluateInner(parsedTokens_, true, args...);
			}
		private:
			void PreAddWord() {
				dirty_ = true;
				// 'A A' is illegal
				// implicit: 'A and A' is legal.
				if (lastAddedTokenType_ == TOKEN_WORD)
				{
					if (implicitAnd_)
						AddAnd();
					else
						makeThrow(Throw_SysntaxErrorWord);
				}
			}

			// remove paren
			void UnParenthesis(TokenVectorT& tv) const
			{
				TokenVectorT::iterator it = tv.begin();
				TokenVectorT::iterator itLastOpeningParen = tv.end();
				bool found = false;

				// Find inner-most paren pair
				for (; it != tv.end(); ++it)
				{
					if (it->IsBeginParen())
					{
						itLastOpeningParen = it;
					}
					else if (it->IsEndParen())
					{
						if (itLastOpeningParen == tv.end())
						{
							// not found start paren
							makeThrow(Throw_SysntaxErrorMismatchedParenthesis);
						}
						found = true;
						it++;
						break;
					}
				}

				if (!found)
				{
					// no ending paren found

					if (itLastOpeningParen != tv.end())
					{
						// but starting paren found
						makeThrow(Throw_SysntaxErrorMismatchedParenthesis);
					}
					
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
					TokenVectorT::iterator insideStart = itLastOpeningParen;
					++insideStart;

					TokenVectorT::iterator insideEnd = it;
					std::advance(insideEnd, -1);


					TokenVectorT insideVecForToken(insideStart, insideEnd);
					TokenT parentToken(insideVecForToken);

					// now create new vector
					std::advance(insideStart, -1);
					std::advance(insideEnd, 1);

					TokenVectorT newVec(tv.begin(), insideStart);
					if (!insideVecForToken.empty())
						newVec.push_back(parentToken);
					newVec.insert(newVec.end(), insideEnd, tv.end());

					// reset our vector
					tv = std::move(newVec);

					// dont know whether it's liner
					UnParenthesis(tv);
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

				typename TokenVectorT::iterator it = tv.begin();
				++it;
				for (; it != tv.end(); std::advance(it, 2))
				{
					if (it->IsAnd())
					{
						it1 = it;
						--it1;

						itOp = it;
						
						it2 = it;
						++it2;
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

				TokenVectorT::iterator itTmp = it2;
				++itTmp;
				TokenVectorT insideVecForToken(it1,  itTmp);
				TokenT parentToken(insideVecForToken);
				// now create new vector

				TokenVectorT newVec(tv.begin(), it1);
				newVec.push_back(parentToken);
				itTmp = it2;
				++itTmp;
				newVec.insert(newVec.end(), itTmp, tv.end());

				// reset our vector
				tv = std::move(newVec);

				// dont know whether it's liner
				UnAnd(tv);
			}
		
			bool EvaluateAnd(const TokenT& left, const TokenT& right, const bool dryRun, Args...args) const
			{
				return EvaluateToken(left,dryRun, args...) && EvaluateToken(right,dryRun, args...);
			}
			bool EvaluateOr(const TokenT& left, const TokenT& right, const bool dryRun, Args...args) const
			{
				return EvaluateToken(left,dryRun, args...) || EvaluateToken(right,dryRun, args...);
			}
			void makeThrow(ThrowType) const {
				throw OpParserError("");
			}
			bool CallEvaluator(const T& word, const bool dryRun, Args...args) const {
				if (dryRun)
					return nullResult_;
				return evaluator_(word, args...);
			}
			bool EvaluateToken(const TokenT& token, const bool dryRun, Args...args) const
			{
				if (token.IsWord())
					return CallEvaluator(token.word(), dryRun, args...);
				else if (token.IsParent())
					return EvaluateInner(token.subTokens(), dryRun, args...);
				else if (token.IsOperator())
					return nullResult_;

				makeThrow(Throw_Unknow);
				return false;
			}
			void Parse(TokenVectorT& tv) const {
				if (tv.empty())
					return;

				// make vector liner, or tree structure
				UnParenthesis(tv);

				if (tv.empty())
				{
					// paren only
					return;
				}

				// make vecotr Un-And, or make vector all connected with 'or'
				UnAnd(tv);

				assert((tv.size() % 2) == 1);
			}
			bool EvaluateInner(const TokenVectorT& tv, const bool dryRun, Args...args) const {
				if (tv.empty())
					return nullResult_;

				// now, all vector is a single, triplet, or, connected only by 'or'.
				if (tv.size() == 1)
				{
					return EvaluateToken(*tv.begin(), dryRun, args...);
					//TokenT& firstTv = *tv.begin();
					//if (firstTv.IsWord())
					//{
					//	return CallEvaluator(firstTv.word(), dryRun, args...);
					//}
					//if (firstTv.IsParent())
					//{
					//	// Todo: remove Copy by add const everywhere
					//	TokenVectorT tokenVec = firstTv.subTokensCopy();
					//	TokenVectorT::iterator it = tokenVec.begin();
					//	TokenT& t0 = *it;

					//	if (tokenVec.size() == 1)
					//		return EvaluateToken(t0, dryRun, args...);

					//	TokenT& t1 = *(++it);


					//	assert(tokenVec.size() == 3);
					//	assert(t1.IsOperator());

					//	TokenT& t2 = *(++it);
					//	if (t1.IsAnd())
					//		return EvaluateAnd(t0, t2, dryRun, args...);
					//	else if (t1.IsOr())
					//		return EvaluateOr(t0, t2, dryRun, args...);

					//	makeThrow(Throw_Unknow);
					//}
					//if (firstTv.IsOperator())
					//	return nullResult_;

					//assert(false);
					//makeThrow(Throw_Unknow);
				}
				else if (tv.size()==3)
				{
					TokenVectorT::const_iterator it = tv.begin();

					TokenVectorT::const_iterator it1 = it++;
					TokenVectorT::const_iterator itOp = it++;
					TokenVectorT::const_iterator it2 = it++;

					assert(itOp->IsOperator());

					if (itOp->IsAnd())
						return EvaluateAnd(*it1, *it2, dryRun, args...);
					else if (itOp->IsOr())
						return EvaluateOr(*it1, *it2, dryRun, args...);

					makeThrow(Throw_Unknow);
				}

				// Now all are connected by 'or'.
				TokenVectorT::const_iterator it = tv.begin();
				do
				{
					//it1 = it - 1;
					//it2 = it + 1;
					TokenVectorT::const_iterator it1 = it;
					++it;
					
					assert(it->IsOr());

					++it;
					TokenVectorT::const_iterator it2 = it;
					
					if (EvaluateOr(*it1, *it2, dryRun, args...))
					{
						// true means no more evaluation needs
						return true;
					}

					++it;
				} while (it != tv.end());
				return false;
			}

		public:
			EvaluatorT ResetEvaluator(EvaluatorT ev) {
				EvaluatorT tmp = evaluator_;
				evaluator_ = ev;
				return tmp;
			}
			bool HasEvaluator() const {
				return !!evaluator_;
			}
		private:
			// Hold dirty state,
			// Optimize Consecutive Evaluate call.
			mutable bool dirty_ = true;
			
			// Hold parsed tokens, culculated by Evaluate call, if dirty_.
			mutable TokenVectorT parsedTokens_;

			// When no equations are provided like '', '()' and '(()())',
			// this value is returned.
			// Initialized at ctor.
			const bool nullResult_ = false;

			const bool implicitAnd_ = false;

			// The main tokens
			TokenVectorT tokens_;

			// User defined evaluator.
			EvaluatorT evaluator_;

			// Last added token type, set by Add* mothed, for
			// check syntax.
			TokenType lastAddedTokenType_ = TokenType::TOKEN_NONE;
		};

	} // namespace OpParser
} // namespace Ambiesoft
