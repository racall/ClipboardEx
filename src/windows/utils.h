#include <windows.h>
#include <string>

std::wstring Utf8ToWstring(const std::string& str);
std::string WstringToUtf8(const std::wstring& wstr);