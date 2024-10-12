#pragma once

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <objc/objc-runtime.h>
#endif
#include <string>
#include <vector>
#include <memory>
#include <iostream>

// struct ImageData {
//     int width;
//     int height;
//     size_t size;
//     std::vector<unsigned char> data;
// };


// struct FileInfo {
//     std::wstring path;
//     std::wstring name;
//     uintmax_t size;
// };


// struct IconData {
//     char* buffer;
//     int width;
//     int height;
//     int dataSize;
// };

enum class ClipboardContentType {
    Text,
    Html,
    Image,
    File,
    Unkonwn,
    Empty
};

ClipboardContentType GetClipboardContentType();

// IconData GetFileIconData(const std::wstring& filePath);
// void FreeIconData(IconData& iconData);

// std::wstring Utf8ToWstring(const std::string& str);
// std::string WstringToUtf8(const std::wstring& wstr);
bool clearClipboard();
// std::wstring ReadTextFromClipboard();
// std::wstring ReadHTMLFromClipboard();
// bool WriteTextToClipboard(const std::wstring& text);

// std::vector<FileInfo> ReadFilesFromClipboard();
// bool WriteFilesToClipboard(const std::vector<std::wstring>& files);

// ImageData ReadImageFromClipboard();
// bool WriteImageToClipboard(const uint8_t* buffer, size_t bufferSize);

