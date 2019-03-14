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

#include <Windows.h>

#include <map>
#include <tuple>

namespace Ambiesoft {

	inline bool SetUserAgent(LPCSTR pUserAgent)
	{
		return SUCCEEDED(
			UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)pUserAgent, (DWORD)strlen(pUserAgent), 0)
			);
	}

	class CUAEntry
	{
		std::string os_;
		int addrwidth_;
		std::string browser_;
	public:
		CUAEntry(const std::string& os, int addrwidth, const std::string& browser) :
			os_(os),
			addrwidth_(addrwidth),
			browser_(browser){}
		CUAEntry(const CUAEntry& that)
		{
			this->os_ = that.os_;
			this->addrwidth_ = that.addrwidth_;
			this->browser_ = that.browser_;
		}

		const std::string& os() const {
			return os_;
		}
		int addrwidth() const {
			return addrwidth_;
		}
		const std::string& browser() const {
			return browser_;
		}
		bool operator<(const CUAEntry& that) const
		{
			return std::tie(os_, addrwidth_, browser_) < std::tie(that.os_, that.addrwidth_, that.browser_);
		}
	};
	class CUserAgentMap
	{
		std::map<CUAEntry, std::string> themap_;
		CUserAgentMap() {
			themap_[CUAEntry(std::string(), 32, "IE7")] = "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)";
			themap_[CUAEntry(std::string(), 32, "IE8")] = "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0)";
		}
	public:
		static CUserAgentMap& GetInstance() {
			static CUserAgentMap theInstance;
			return theInstance;
		}

		std::string GetUA(const std::string& browser) {
			for (auto&& uaEntry : themap_)
			{
				if (uaEntry.first.browser() == browser)
					return uaEntry.second;
			}
			return std::string();
		}
	};
} // namespace