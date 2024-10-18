#define UNICODE
#include "../clipboard.h"
#include "utils.h"


/**
 * 从剪切板中读取文本
 * @return text
 */
std::string ReadTextFromClipboard() {
    if (!OpenClipboard(nullptr)) return "";
    
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }
    
    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
    std::wstring text(pszText);
    
    GlobalUnlock(hData);
    CloseClipboard();
    return WstringToUtf8(text);
}

/**
 * 从剪切板中读取 HTML
 * @return html
 */
std::string ReadHTMLFromClipboard() {
    if (!OpenClipboard(nullptr)) return "";
    unsigned int CF_HTML = RegisterClipboardFormatW(L"HTML Format");
    HANDLE hData = GetClipboardData(CF_HTML);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }
    DWORD dataSize = GlobalSize(hData);
    if (dataSize == 0) {
        CloseClipboard();
        return "";
    }

    char* pszText = static_cast<char*>(GlobalLock(hData));
    std::string text = "";

    if (pszText) {
        text = std::string(pszText, dataSize);
        GlobalUnlock(hData);
    }

    CloseClipboard();
    return text;
}