#include <napi.h>
#include "clipboard.h"
#include <iostream>

/**
 * Get the content type of the clipboard
 * @returns {String}
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

/**
 * Clear the clipboard
 * @returns {Boolean}
 */
Napi::Boolean clearClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, clearClipboard());
}

/**
 * Read text from clipboard
 * @returns {String}
 */
Napi::String ReadTextFromClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::string text = ReadTextFromClipboard();
    return Napi::String::New(env, text);
}

/**
 * Read HTML from clipboard
 * @returns {String}
 */
Napi::String ReadHTMLFromClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::string html = ReadHTMLFromClipboard();
    return Napi::String::New(env, html);
}

/**
 * Write text to clipboard
 * @returns {Boolean}
 */
Napi::Boolean WriteTextToClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    std::string text = info[0].As<Napi::String>().Utf8Value();
    return Napi::Boolean::New(env, WriteTextToClipboard(text));
}

/**
 * Read files from clipboard
 * @returns {Array}
 */
Napi::Array ReadFilesFromClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::vector<FileInfox> files = ReadFilesFromClipboard();
    Napi::Array result = Napi::Array::New(env, files.size());

    for (uint32_t i = 0; i < files.size(); i++) {
        Napi::Object fileObject = Napi::Object::New(env);
        fileObject.Set("path", Napi::String::New(env, files[i].path));
        fileObject.Set("name", Napi::String::New(env, files[i].name));
        fileObject.Set("size", Napi::Number::New(env, static_cast<double>(files[i].size)));

        result.Set(i, fileObject);
    }

    return result;
}

/**
 * Write files to clipboard
 * @returns {Boolean}
 */
Napi::Boolean WriteFilesToClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Array of strings expected").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    Napi::Array jsFiles = info[0].As<Napi::Array>();
    std::vector<std::string> files;
    // std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    for (uint32_t i = 0; i < jsFiles.Length(); i++) {
        Napi::Value value = jsFiles.Get(i);
        if (!value.IsString()) {
            Napi::TypeError::New(env, "Array elements must be strings").ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }
        std::string file = value.As<Napi::String>().Utf8Value();
        files.push_back(file);
    }
    return Napi::Boolean::New(env, WriteFilesToClipboard(files));
}

/**
* Read image from clipboard
* @return {width,height,size}
*/
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
 * Write image to clipboard
 * @returns {Boolean}
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
    size_t length = buffer.Length();
    
    // 调用功能函数
    bool result = WriteImageToClipboard(buffer.Data(), length);

    return Napi::Boolean::New(env, result);
}

/**
* Read icon from file
* @return {width,height,buffer}
*/
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


    IconData iconData = GetFileIconData(filePath);

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
    exports.Set("getFileIcon", Napi::Function::New(env, GetFileIcon));
    return exports;
}

NODE_API_MODULE(clipboard, Init)
