#define UNICODE
#include "../clipboard.h"

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
