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

struct ImageData {
    int width;
    int height;
    size_t size;
    std::vector<unsigned char> data;
};


struct FileInfox {
    std::string path;
    std::string name;
    uintmax_t size;
};


struct IconData {
    char* buffer;
    int width;
    int height;
    size_t dataSize;
};

enum class ClipboardContentType {
    Text,
    Html,
    Image,
    File,
    Unkonwn,
    Empty
};

ClipboardContentType GetClipboardContentType();
bool clearClipboard();
std::string ReadTextFromClipboard();
std::string ReadHTMLFromClipboard();
bool WriteTextToClipboard(const std::string& text);
IconData GetFileIconData(const std::string& filePath);
// void FreeIconData(IconData& iconData);

// std::wstring Utf8ToWstring(const std::string& str);
// std::string WstringToUtf8(const std::wstring& wstr);

std::vector<FileInfox> ReadFilesFromClipboard();
bool WriteFilesToClipboard(const std::vector<std::string>& files);

ImageData ReadImageFromClipboard();
bool WriteImageToClipboard(const uint8_t* buffer, size_t bufferSize);

