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
			virtual ~OpParserError() {}

			//            virtual const char* what() const {
			//                return std::runtime_error::what();
			//            }

		};


		enum class OpParserTokenType
		{
			TOKEN_NONE,

			TOKEN_BEGINNING_PARENTHESIS,
			TOKEN_ENDING_PARENTHESIS,
			TOKEN_OR,
			TOKEN_AND,


			TOKEN_PREDICATOR,

			TOKEN_PARENT,
		};


		template<typename T>
		class Token
		{
			using TokenT = Token<T>;
			using TokenVectorT = OpParserContainerType<TokenT>; //std::vector<TokenT>;


		public:
			explicit Token(OpParserTokenType tt) :
				tt_(tt) {}
			explicit Token(const T& predicator) :
				tt_(OpParserTokenType::TOKEN_PREDICATOR),
				predicator_(new T(predicator))
			{}

			// Ctor for parent token which contains sub tokens
			explicit Token(const TokenVectorT& v) :
				tt_(OpParserTokenType::TOKEN_PARENT),
				subtokens_(v)
			{}

			// copy
			explicit Token(const TokenT& that) :
				tt_(that.tt_),
				subtokens_(that.subtokens_)
			{
				if (that.predicator_)
					predicator_ = new T(*that.predicator_);
			}

			// rval copy
			explicit Token(TokenT&& that) :
				tt_(std::move(that.tt_)),
				subtokens_(std::move(that.subtokens_))
			{
				predicator_ = that.predicator_;
				that.predicator_ = nullptr;
			}

			// dtor
			~Token()
			{
				delete predicator_;
			}
			// Equal Operator
			TokenT& operator=(const TokenT& that) {
				if (this != &that)
				{
					this->tt_ = that.tt_;
					if (that.predicator_)
						this->predicator_ = new T(*that.predicator_);
					this->subtokens_ = that.subtokens_;
				}
				return *this;
			}
			// rval Equal Operator
			TokenT& operator=(TokenT&& that) {
				if (this != &that)
				{
					this->tt_ = std::move(that.tt_);
					this->predicator_ = that.predicator_;
					that.predicator_ = nullptr;
					this->subtokens_ = std::move(that.subtokens_);
				}
				return *this;
			}

			bool IsOperator() const {
				return tt_ == OpParserTokenType::TOKEN_AND || tt_ == OpParserTokenType::TOKEN_OR;
			}
			bool IsAnd() const {
				return tt_ == OpParserTokenType::TOKEN_AND;
			}
			bool IsOr() const {
				return tt_ == OpParserTokenType::TOKEN_OR;
			}
			bool IsBeginParen() const {
				return tt_ == OpParserTokenType::TOKEN_BEGINNING_PARENTHESIS;
			}
			bool IsEndParen() const {
				return tt_ == OpParserTokenType::TOKEN_ENDING_PARENTHESIS;
			}
			bool IsPredicator() const {
				return tt_ == OpParserTokenType::TOKEN_PREDICATOR;
			}
			bool IsParent() const {
				return tt_ == OpParserTokenType::TOKEN_PARENT;
			}

			const T& predicator() const {
				assert(IsPredicator());
				assert(predicator_ != nullptr);
				return *predicator_;
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

			bool hasResult() const {
				return result_ != RESULT_UNKNOWN;
			}
			bool result() const {
				assert(hasResult());
				return result_ == RESULT_TRUE ? true : false;
			}
			void setResult(bool b) const {
				result_ = b ? RESULT_TRUE : RESULT_FALSE;
			}
			void clearResult() const {
				result_ = RESULT_UNKNOWN;
				if (IsParent()) {
					for (const TokenT& token : subtokens_)
						token.clearResult();
				}
			}
		private:
			// Constructed by ctor from user, or
			// while evaluating, it will become Parent.
			OpParserTokenType tt_;

			// User's predicator.
			// Much of copy will be performed on T.
			T* predicator_ = nullptr;

			// When token type is Parent, this is set.
			// All '()' and 'A AND B' will be subtoken.
			TokenVectorT subtokens_;

			// Result of calling evaluate
			mutable enum {
				RESULT_UNKNOWN,
				RESULT_TRUE,
				RESULT_FALSE,
			} result_ = RESULT_UNKNOWN;
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
				Throw_SysntaxErrorPredicator,

				Throw_SysntaxErrorMismatchedParenthesis,
			};

			void overMe(const OpParserT& that) {
				*(const_cast<bool*>(&nullResult_)) = that.nullResult_;
				*(const_cast<bool*>(&implicitAnd_)) = that.implicitAnd_;
				evaluator_ = that.evaluator_;
				tokens_ = that.tokens_;
				parsedTokens_ = that.parsedTokens_;
				lastAddedTokenType_ = that.lastAddedTokenType_;
			}
		public:
			explicit OpParser() {}
			explicit OpParser(EvaluatorT evaluator,
				bool nullResult = false,
				bool implicitAnd = false) :
				evaluator_(evaluator),
				nullResult_(nullResult),
				implicitAnd_(implicitAnd) {}

			OpParser(const OpParserT& that) {
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
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_PREDICATOR)
				{
					if (implicitAnd_)
						AddAnd();
					else
						makeThrow(Throw_SysntaxErrorBeginningParenthesis);
				}
				// ') (' is illegal.
				// implicit: ') and (' is legal.
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_ENDING_PARENTHESIS)
				{
					if (implicitAnd_)
						AddAnd();
					else
						makeThrow(Throw_SysntaxErrorBeginningParenthesis);
				}

				tokens_.push_back(TokenT(lastAddedTokenType_ = OpParserTokenType::TOKEN_BEGINNING_PARENTHESIS));
			}
			void AddAnd() {
				dirty_ = true;
				// 'AND ...' with 'AND' is illegal.
				// '( AND ...' is illegal.
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_NONE ||
					lastAddedTokenType_ == OpParserTokenType::TOKEN_BEGINNING_PARENTHESIS)
				{
					makeThrow(Throw_SysntaxErrorAnd);
				}

				tokens_.push_back(TokenT(lastAddedTokenType_ = OpParserTokenType::TOKEN_AND));
			}
			void AddOr() {
				dirty_ = true;
				// 'OR ...' is illegal.
				// implicit: 'AND OR ...' is still illegal.
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_NONE)
				{
					makeThrow(Throw_SysntaxErrorOr);
				}

				// '( OR ...' is illegal.
				// '( AND OR ...' is still illegal.
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_BEGINNING_PARENTHESIS)
				{
					makeThrow(Throw_SysntaxErrorOr);
				}
				tokens_.push_back(TokenT(lastAddedTokenType_ = OpParserTokenType::TOKEN_OR));
			}
			void AddEndingParenthesis() {
				dirty_ = true;
				// ') ...' is illegal.
				// implicat: 'AND ) ...' is still illegal.
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_NONE)
					makeThrow(Throw_SysntaxErrorEndingParenthesis);

				// 'AND )' is illegal.
				// implicit: 'AND AND )' is illegal.
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_AND)
					makeThrow(Throw_SysntaxErrorEndingParenthesis);

				tokens_.push_back(TokenT(lastAddedTokenType_ = OpParserTokenType::TOKEN_ENDING_PARENTHESIS));
			}

			// TODO: Add Not
			void AddNot() {
				dirty_ = true;
			}

			void AddPredicator(const T& predicator) {
				dirty_ = true;
				PreAddPredicator();
				lastAddedTokenType_ = OpParserTokenType::TOKEN_PREDICATOR;
				tokens_.push_back(TokenT(predicator));
			}
			// rval
			void AddPredicator(T&& predicator) {
				dirty_ = true;
				PreAddPredicator();
				lastAddedTokenType_ = OpParserTokenType::TOKEN_PREDICATOR;
				tokens_.push_back(TokenT(std::move(predicator)));
			}

			bool Evaluate(Args... args) const {
				clearResults();
				if (dirty_)
				{
					parsedTokens_ = tokens_;
					Parse(parsedTokens_);
					dirty_ = false;
				}
				return EvaluateInner(parsedTokens_, false, args...);
			}
			void TryEvaluate(Args...args) const {
				clearResults();
				if (dirty_)
				{
					parsedTokens_ = tokens_;
					Parse(parsedTokens_);
					dirty_ = false;
				}
				EvaluateInner(parsedTokens_, true, args...);
			}
		private:
			void clearResults() const {
				for (const TokenT& token : parsedTokens_)
					token.clearResult();
				for (const TokenT& token : tokens_)
					token.clearResult();
			}
			void PreAddPredicator() {
				dirty_ = true;
				// 'A A' is illegal
				// implicit: 'A and A' is legal.
				if (lastAddedTokenType_ == OpParserTokenType::TOKEN_PREDICATOR)
				{
					if (implicitAnd_)
						AddAnd();
					else
						makeThrow(Throw_SysntaxErrorPredicator);
				}
			}

			// remove paren
			void UnParenthesis(TokenVectorT& tv) const
			{
				typename TokenVectorT::iterator it = tv.begin();
				typename TokenVectorT::iterator itLastOpeningParen = tv.end();
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
					typename TokenVectorT::iterator insideStart = itLastOpeningParen;
					++insideStart;

					typename TokenVectorT::iterator insideEnd = it;
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
				TokenVectorT& tv) const
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

				typename TokenVectorT::iterator it1, itOp, it2;
				if (!FindAndTripet(it1, itOp, it2, tv))
				{
					// no more 'and'
					return;
				}

				typename TokenVectorT::iterator itTmp = it2;
				++itTmp;
				TokenVectorT insideVecForToken(it1, itTmp);
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
				return EvaluateToken(left, dryRun, args...) && EvaluateToken(right, dryRun, args...);
			}
			bool EvaluateOr(const TokenT& left, const TokenT& right, const bool dryRun, Args...args) const
			{
				return EvaluateToken(left, dryRun, args...) || EvaluateToken(right, dryRun, args...);
			}
			void makeThrow(ThrowType) const {
				throw OpParserError("");
			}
			bool CallEvaluator(const T& predicator, const bool dryRun, Args...args) const {
				if (dryRun)
					return nullResult_;
				return evaluator_(predicator, args...);
			}
			bool EvaluateToken(const TokenT& token, const bool dryRun, Args...args) const
			{
				if (token.IsPredicator())
				{
					// cache result
					if (token.hasResult())
						return token.result();
					bool ret = CallEvaluator(token.predicator(), dryRun, args...);
					token.setResult(ret);
					return ret;
				}
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
				}
				else if (tv.size() == 3)
				{
					typename TokenVectorT::const_iterator it = tv.begin();

					typename TokenVectorT::const_iterator it1 = it++;
					typename TokenVectorT::const_iterator itOp = it++;
					typename TokenVectorT::const_iterator it2 = it++;

					assert(itOp->IsOperator());

					if (itOp->IsAnd())
						return EvaluateAnd(*it1, *it2, dryRun, args...);
					else if (itOp->IsOr())
						return EvaluateOr(*it1, *it2, dryRun, args...);

					makeThrow(Throw_Unknow);
				}

				// Now all are connected by 'or'.
				for (typename TokenVectorT::const_iterator it = tv.begin();;)
				{
					typename TokenVectorT::const_iterator it1 = it;
					assert(!it1->IsOr());

					++it;
					if (it == tv.end())
						break;

					assert(it->IsOr());

					++it;
					typename TokenVectorT::const_iterator it2 = it;
					assert(!it2->IsOr());

					if (EvaluateOr(*it1, *it2, dryRun, args...))
					{
						// true means no more evaluation needs
						return true;
					}
				}
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
			// User defined evaluator.
			EvaluatorT evaluator_;

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


			// Last added token type, set by Add* mothed, for
			// check syntax.
			OpParserTokenType lastAddedTokenType_ = OpParserTokenType::TOKEN_NONE;
		};

	} // namespace OpParser
} // namespace Ambiesoft
