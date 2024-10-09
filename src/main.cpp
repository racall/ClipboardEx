#ifdef _WIN32
#define UNICODE
#endif
#include <napi.h>
#include "clipboard.h"
#include <iostream>

/**
 * 清除剪贴板
 */
Napi::Boolean clearClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, clearClipboard());
}

/**
 * 保存剪贴板图片到文件
 * @param filePath 文件路径
 */
// Napi::Boolean SaveClipboardImageToFileWrapped(const Napi::CallbackInfo& info) {
//     Napi::Env env = info.Env();
//     if (info.Length() < 1 || !info[0].IsString()) {
//         Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
//         return Napi::Boolean::New(env, false);
//     }

//     std::string filePath = info[0].As<Napi::String>().Utf8Value();
//     std::wstring wFilePath = Utf8ToWstring(filePath);

//     return Napi::Boolean::New(env, SaveClipboardImageToFile(wFilePath));
// }

/*
 * 读取剪贴板文本
*/
Napi::String ReadTextFromClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::wstring text = ReadTextFromClipboard();
    return Napi::String::New(env, WstringToUtf8(text));
}

/**
 * 读取剪贴板HTML
*/
Napi::String ReadHTMLFromClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::wstring html = ReadHTMLFromClipboard();
    return Napi::String::New(env, WstringToUtf8(html));
}

/**
 * 写入剪贴板文本
 * @param text 文本
 */
Napi::Boolean WriteTextToClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    std::string text = info[0].As<Napi::String>().Utf8Value();
    std::wstring wtext = Utf8ToWstring(text);
    return Napi::Boolean::New(env, WriteTextToClipboard(wtext));
}

/**
 * 读取剪贴板文件
 * @return Array of file info
*/
Napi::Array ReadFilesFromClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::vector<FileInfo> files = ReadFilesFromClipboard();
    Napi::Array result = Napi::Array::New(env, files.size());

    for (size_t i = 0; i < files.size(); i++) {
        Napi::Object fileObject = Napi::Object::New(env);
        fileObject.Set("path", Napi::String::New(env, WstringToUtf8(files[i].path)));
        fileObject.Set("name", Napi::String::New(env, WstringToUtf8(files[i].name)));
        fileObject.Set("size", Napi::Number::New(env, static_cast<double>(files[i].size)));

        result.Set(i, fileObject);
    }

    return result;
}

/**
 * 写入剪贴板文件
 * @param files Array of file paths
*/
Napi::Boolean WriteFilesToClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Array of strings expected").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    Napi::Array jsFiles = info[0].As<Napi::Array>();
    std::vector<std::wstring> files;
    // std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    for (uint32_t i = 0; i < jsFiles.Length(); i++) {
        Napi::Value value = jsFiles.Get(i);
        if (!value.IsString()) {
            Napi::TypeError::New(env, "Array elements must be strings").ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }
        std::string file = value.As<Napi::String>().Utf8Value();
        files.push_back(Utf8ToWstring(file));
    }
    return Napi::Boolean::New(env, WriteFilesToClipboard(files));
}

/**
 * 读取剪贴板图片
 * @return Object containing width, height, and pixels
*/
// Napi::Object ReadImageFromClipboardWrapped(const Napi::CallbackInfo& info) {
//     Napi::Env env = info.Env();
//     Napi::Object result = Napi::Object::New(env);

//     try {
//         BitmapData bmpData = ReadImageFromClipboard();
        
//         result.Set("width", Napi::Number::New(env, bmpData.width));
//         result.Set("height", Napi::Number::New(env, bmpData.height));
        
//         // 创建一个持久的副本
//         uint8_t* pixelsCopy = new uint8_t[bmpData.pixels.size()];
//         std::memcpy(pixelsCopy, bmpData.pixels.data(), bmpData.pixels.size());

//         // 使用 ArrayBuffer::New 的另一个重载,它接受一个析构函数
//         auto buffer = Napi::ArrayBuffer::New(
//             env, 
//             pixelsCopy, 
//             bmpData.pixels.size(),
//             [](Napi::Env env, void* data) {
//                 delete[] static_cast<uint8_t*>(data);
//             }
//         );
//         result.Set("pixels", Napi::Uint8Array::New(env, bmpData.pixels.size(), buffer, 0));
//     }
//     catch (const std::exception& e) {
//         Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
//     }
//     catch (...) {
//         Napi::Error::New(env, "Unknown error occurred").ThrowAsJavaScriptException();
//     }

//     return result;
// }

/**
 * 写入剪贴板图片
 * @param image Object containing width, height, and pixels
*/
Napi::Boolean WriteImageToClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // 检查参数数量和类型
    if (info.Length() < 1 || !info[0].IsBuffer()) {
        // Napi::TypeError::New(env,"参数错误：需要一个Buffer类型的参数").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    // 获取Buffer数据
    Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();
    // void* data = buffer.Data();
    size_t length = buffer.Length();

    // 调用功能函数
    bool result = WriteImageToClipboard(buffer.Data(), length);

    if (!result) {
        // Napi::Error::New(env,"无法将图像设置到剪贴板").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    return Napi::Boolean::New(env, result);
}
Napi::Value GetFileIcon(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string filePath = info[0].As<Napi::String>().Utf8Value();
    std::wstring wFilePath = Utf8ToWstring(filePath);

    IconData iconData = GetFileIconData(wFilePath);

    if (iconData.buffer == nullptr) {
        Napi::Error::New(env, "Failed to get file icon").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Buffer<char> buffer = Napi::Buffer<char>::New(env, iconData.buffer, iconData.dataSize, [](Napi::Env, char* data) {
        delete[] data;
    });

    Napi::Object result = Napi::Object::New(env);
    result.Set(Napi::String::New(env, "buffer"), buffer);
    result.Set(Napi::String::New(env, "width"), Napi::Number::New(env, iconData.width));
    result.Set(Napi::String::New(env, "height"), Napi::Number::New(env, iconData.height));

    return result;
}

Napi::Object ReadClipboardImageWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Object result = Napi::Object::New(env);
    ImageData imgData = ReadImageFromClipboard();
    result.Set("width", Napi::Number::New(env, imgData.width));
    result.Set("height", Napi::Number::New(env, imgData.height));
    result.Set("size", Napi::Number::New(env, imgData.size));
    if (imgData.data.empty())
    {
        return result;
    }
    Napi::Buffer<unsigned char> buffer = Napi::Buffer<unsigned char>::Copy(
        env, 
        imgData.data.data(), 
        imgData.size
    );
    result.Set("data", buffer);

    return result;
}

/**
 * 获取剪切板内容类型
 * @return string
*/
Napi::String GetClipboardContentTypeWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    
    ClipboardContentType type = GetClipboardContentType();
    
    switch (type) {
        case ClipboardContentType::Html:
            return Napi::String::New(env, "html");
        case ClipboardContentType::Text:
            return Napi::String::New(env, "text");
        case ClipboardContentType::Image:
            return Napi::String::New(env, "image");
        case ClipboardContentType::File:
            return Napi::String::New(env, "file");
        case ClipboardContentType::Empty:
            return Napi::String::New(env, "empty");
        default:
            return Napi::String::New(env, "unknown");
    }
}
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getContentType", Napi::Function::New(env, GetClipboardContentTypeWrapped));
    exports.Set("clear", Napi::Function::New(env, clearClipboardWrapped));
    exports.Set("readText", Napi::Function::New(env, ReadTextFromClipboardWrapped));
    exports.Set("readHTML",Napi::Function::New(env, ReadHTMLFromClipboardWrapped));
    exports.Set("writeText", Napi::Function::New(env, WriteTextToClipboardWrapped));
    exports.Set("readFiles", Napi::Function::New(env, ReadFilesFromClipboardWrapped));
    exports.Set("writeFiles", Napi::Function::New(env, WriteFilesToClipboardWrapped));
    exports.Set("readImage", Napi::Function::New(env, ReadClipboardImageWrapped));
    exports.Set("writeImage", Napi::Function::New(env, WriteImageToClipboardWrapped));
    // exports.Set("saveImageToFile", Napi::Function::New(env, SaveClipboardImageToFileWrapped));
    exports.Set("getFileIcon", Napi::Function::New(env, GetFileIcon));
    return exports;
}

NODE_API_MODULE(clipboard, Init)