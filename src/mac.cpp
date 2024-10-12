#include <napi.h>
#include "clipboard.h"
#include <iostream>

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
Napi::Boolean clearClipboardWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, clearClipboard());
}
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getContentType", Napi::Function::New(env, GetClipboardContentTypeWrapped));
    exports.Set("clear", Napi::Function::New(env, clearClipboardWrapped));
    // exports.Set("readText", Napi::Function::New(env, ReadTextFromClipboardWrapped));
    // exports.Set("readHTML",Napi::Function::New(env, ReadHTMLFromClipboardWrapped));
    // exports.Set("writeText", Napi::Function::New(env, WriteTextToClipboardWrapped));
    // exports.Set("readFiles", Napi::Function::New(env, ReadFilesFromClipboardWrapped));
    // exports.Set("writeFiles", Napi::Function::New(env, WriteFilesToClipboardWrapped));
    // exports.Set("readImage", Napi::Function::New(env, ReadClipboardImageWrapped));
    // exports.Set("writeImage", Napi::Function::New(env, WriteImageToClipboardWrapped));
    // exports.Set("saveImageToFile", Napi::Function::New(env, SaveClipboardImageToFileWrapped));
    // exports.Set("getFileIcon", Napi::Function::New(env, GetFileIcon));
    return exports;
}

NODE_API_MODULE(clipboard, Init)