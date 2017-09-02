#pragma once


#ifdef __cplusplus_cli



System::String^ toCLR(const std::wstring& s);
std::wstring toWstring(System::String^ s);


#define toLPCW(s) (toWstring(s).c_str())


System::String^ doubleQuote(System::String^ s);





#endif