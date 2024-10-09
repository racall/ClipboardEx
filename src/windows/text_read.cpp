#define UNICODE
#include "../clipboard.h"



/**
 * 从剪切板中读取文本
 * @return text
 */
std::wstring ReadTextFromClipboard() {
    if (!OpenClipboard(nullptr)) return L"";
    
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return L"";
    }
    
    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
    std::wstring text(pszText);
    
    GlobalUnlock(hData);
    CloseClipboard();
    return text;
}

/**
 * 从剪切板中读取 HTML
 * @return html
 */
std::wstring ReadHTMLFromClipboard() {
    if (!OpenClipboard(nullptr)) return L"";
    unsigned int CF_HTML = RegisterClipboardFormatW(L"HTML Format");
    HANDLE hData = GetClipboardData(CF_HTML);
    if (hData == nullptr) {
        CloseClipboard();
        return L"";
    }
    DWORD dataSize = GlobalSize(hData);
    if (dataSize == 0) {
        CloseClipboard();
        return L"";
    }

    char* pszText = static_cast<char*>(GlobalLock(hData));
    std::wstring text;

    if (pszText) {
        text = Utf8ToWstring(std::string(pszText, dataSize));
        GlobalUnlock(hData);
    }

    CloseClipboard();
    return text;
}