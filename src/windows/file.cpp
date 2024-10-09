#define UNICODE
#include "../clipboard.h"
#include <shlobj.h>
#include <filesystem>
#include <fstream>
#include <iostream>


/**
 * 从剪切板中读取文件信息
 * @return vector of FileInfo
 */
std::vector<FileInfo> ReadFilesFromClipboard() {
    std::vector<FileInfo> files;
    if (!OpenClipboard(nullptr)) return files;
    
    HDROP hDrop = (HDROP)GetClipboardData(CF_HDROP);
    if (hDrop == nullptr) {
        CloseClipboard();
        return files;
    }
    
    UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);
    for (UINT i = 0; i < fileCount; ++i) {
        UINT size = DragQueryFileW(hDrop, i, nullptr, 0) + 1;
        std::vector<wchar_t> filePath(size);
        DragQueryFileW(hDrop, i, filePath.data(), size);

        std::wstring fullPath = filePath.data();
        std::wstring fileName = std::filesystem::path(fullPath).filename().wstring();
        uintmax_t fileSize = std::filesystem::file_size(fullPath);

        files.push_back({ fullPath, fileName, fileSize });
    }
    
    CloseClipboard();
    return files;
}

/**
 * 将文件写入剪切板
 * @param files vector of file paths
 * @return true if success
 */
bool WriteFilesToClipboard(const std::vector<std::wstring>& files) {
    if (!OpenClipboard(nullptr)) return false;
    
    EmptyClipboard();
    
    size_t totalSize = sizeof(DROPFILES) + 2;
    for (const auto& file : files) {
        totalSize += (file.length() + 1) * sizeof(wchar_t);
    }
    
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, totalSize);
    if (hMem == nullptr) {
        CloseClipboard();
        return false;
    }
    
    DROPFILES* pDropFiles = static_cast<DROPFILES*>(GlobalLock(hMem));
    pDropFiles->pFiles = sizeof(DROPFILES);
    pDropFiles->fWide = TRUE;
    
    wchar_t* pFilenames = reinterpret_cast<wchar_t*>(reinterpret_cast<char*>(pDropFiles) + sizeof(DROPFILES));
    for (const auto& file : files) {
        wcscpy_s(pFilenames, file.length() + 1, file.c_str());
        pFilenames += file.length() + 1;
    }
    *pFilenames = L'\0';  // 额外的null终止符
    
    GlobalUnlock(hMem);
    
    if (SetClipboardData(CF_HDROP, hMem) == nullptr) {
        CloseClipboard();
        GlobalFree(hMem);
        return false;
    }
    
    CloseClipboard();
    return true;
}


