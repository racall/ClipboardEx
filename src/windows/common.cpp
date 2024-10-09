#define UNICODE
#include "../clipboard.h"

/**
 * Convert UTF-8 string to wide string
 * @param str UTF-8 string
 * @return wide string
 */
std::wstring Utf8ToWstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

/**
 * Convert wide string to UTF-8 string
 * @param wstr wide string
 * @return UTF-8 string
 */
std::string WstringToUtf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

/**
 * 清除剪贴板
 * @return true if clipboard was cleared
 */
bool clearClipboard() {
    if (!OpenClipboard(nullptr)) return false;
    BOOL isEmpty = EmptyClipboard();
    CloseClipboard();
    return isEmpty;
}
UINT RegisterClipboardFormatHTML() {
    static UINT cf_HTML = 0;
    if (cf_HTML == 0) {
        cf_HTML = RegisterClipboardFormatA("HTML Format");
    }
    return cf_HTML;
}

/*
 * 获取剪贴板内容类型
 * @return ClipboardContentType
*/
ClipboardContentType GetClipboardContentType() {
    if (!OpenClipboard(nullptr)) return ClipboardContentType::Empty;
    UINT cf_HTML = RegisterClipboardFormatHTML();
    if(cf_HTML && IsClipboardFormatAvailable(cf_HTML)) {
        CloseClipboard();
        return ClipboardContentType::Html;
    } else if (IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_UNICODETEXT)){
        CloseClipboard();
        return ClipboardContentType::Text;
    }else if (IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB)){
        CloseClipboard();
        return ClipboardContentType::Image;
    } else if (IsClipboardFormatAvailable(CF_HDROP)){
        CloseClipboard();
        return ClipboardContentType::File;
    }
    CloseClipboard();
    return ClipboardContentType::Unkonwn;
}
