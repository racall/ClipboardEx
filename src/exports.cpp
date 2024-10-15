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

    for (size_t i = 0; i < files.size(); i++) {
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

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getContentType", Napi::Function::New(env, GetClipboardContentTypeWrapped));
    exports.Set("clear", Napi::Function::New(env, clearClipboardWrapped));
    exports.Set("readText", Napi::Function::New(env, ReadTextFromClipboardWrapped));
    exports.Set("readHTML",Napi::Function::New(env, ReadHTMLFromClipboardWrapped));
    exports.Set("writeText", Napi::Function::New(env, WriteTextToClipboardWrapped));
    exports.Set("readFiles", Napi::Function::New(env, ReadFilesFromClipboardWrapped));
    exports.Set("writeFiles", Napi::Function::New(env, WriteFilesToClipboardWrapped));
    exports.Set("readImage", Napi::Function::New(env, ReadClipboardImageWrapped));
    // exports.Set("writeImage", Napi::Function::New(env, WriteImageToClipboardWrapped));
    // exports.Set("saveImageToFile", Napi::Function::New(env, SaveClipboardImageToFileWrapped));
    // exports.Set("getFileIcon", Napi::Function::New(env, GetFileIcon));
    return exports;
}

NODE_API_MODULE(clipboard, Init)