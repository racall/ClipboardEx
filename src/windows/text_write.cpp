#define UNICODE
#include "../clipboard.h"

/** 
 * 往剪切板写入文本
 * @param text 文本
 * @return 是否写入成功
 */
bool WriteTextToClipboard(const std::wstring& text) {
    if (!OpenClipboard(nullptr)) return false;
    
    EmptyClipboard();
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * sizeof(wchar_t));
    if (hMem == nullptr) {
        CloseClipboard();
        return false;
    }
    
    wchar_t* pszData = static_cast<wchar_t*>(GlobalLock(hMem));
    wcscpy_s(pszData, text.length() + 1, text.c_str());
    GlobalUnlock(hMem);
    
    if (SetClipboardData(CF_UNICODETEXT, hMem) == nullptr) {
        CloseClipboard();
        return false;
    }
    
    CloseClipboard();
    return true;
}