#import <Cocoa/Cocoa.h>
#include "clipboard.h"
#include <vector>
#include <string>
#include <filesystem>

std::string WstringToUtf8(const std::wstring& wstr) {
    std::string utf8;
    try {
        utf8 = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
    } catch (const std::exception& e) {
        NSLog(@"Error converting wstring to UTF-8: %s", e.what());
    }
    return utf8;
}

std::wstring Utf8ToWstring(const std::string& str) {
    std::wstring wide;
    try {
        wide = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
    } catch (const std::exception& e) {
        NSLog(@"Error converting UTF-8 to wstring: %s", e.what());
    }
    return wide;
}

std::wstring ReadTextFromClipboard() {
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSString *string = [pasteboard stringForType:NSPasteboardTypeString];
    return Utf8ToWstring([string UTF8String]);
}

std::wstring ReadHTMLFromClipboard() {
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSString *html = [pasteboard stringForType:NSPasteboardTypeHTML];
    return Utf8ToWstring([html UTF8String]);
}

bool WriteTextToClipboard(const std::wstring& text) {
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    NSString *nsString = [NSString stringWithUTF8String:WstringToUtf8(text).c_str()];
    return [pasteboard setString:nsString forType:NSPasteboardTypeString];
}

std::vector<FileInfo> ReadFilesFromClipboard() {
    std::vector<FileInfo> files;
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSArray *classes = @[[NSURL class]];
    NSDictionary *options = @{NSPasteboardURLReadingFileURLsOnlyKey : @YES};
    NSArray *urls = [pasteboard readObjectsForClasses:classes options:options];
    
    for (NSURL *url in urls) {
        if ([url isFileURL]) {
            NSString *path = [url path];
            NSString *name = [path lastPathComponent];
            NSError *error = nil;
            NSDictionary *attributes = [[NSFileManager defaultManager] attributesOfItemAtPath:path error:&error];
            
            if (!error) {
                unsigned long long fileSize = [attributes fileSize];
                files.push_back({
                    Utf8ToWstring([path UTF8String]),
                    Utf8ToWstring([name UTF8String]),
                    static_cast<uintmax_t>(fileSize)
                });
            }
        }
    }
    
    return files;
}

bool WriteFilesToClipboard(const std::vector<std::wstring>& files) {
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    
    NSMutableArray *urls = [NSMutableArray array];
    for (const auto& file : files) {
        NSString *path = [NSString stringWithUTF8String:WstringToUtf8(file).c_str()];
        NSURL *url = [NSURL fileURLWithPath:path];
        [urls addObject:url];
    }
    
    return [pasteboard writeObjects:urls];
}

BitmapData ReadImageFromClipboard() {
    BitmapData result = {nullptr, 0, 0, {}};
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSData *imageData = [pasteboard dataForType:NSPasteboardTypeTIFF];
    
    if (imageData) {
        NSBitmapImageRep *bitmap = [NSBitmapImageRep imageRepWithData:imageData];
        result.width = [bitmap pixelsWide];
        result.height = [bitmap pixelsHigh];
        
        NSInteger bytesPerRow = [bitmap bytesPerRow];
        NSInteger bitsPerPixel = [bitmap bitsPerPixel];
        NSInteger samplesPerPixel = [bitmap samplesPerPixel];
        
        result.pixels.resize(bytesPerRow * result.height);
        memcpy(result.pixels.data(), [bitmap bitmapData], result.pixels.size());
        
        // Convert to 32-bit RGBA if necessary
        if (bitsPerPixel != 32 || samplesPerPixel != 4) {
            std::vector<unsigned char> newPixels(4 * result.width * result.height);
            for (int y = 0; y < result.height; ++y) {
                for (int x = 0; x < result.width; ++x) {
                    NSColor *color = [bitmap colorAtX:x y:y];
                    CGFloat r, g, b, a;
                    [color getRed:&r green:&g blue:&b alpha:&a];
                    int index = (y * result.width + x) * 4;
                    newPixels[index] = static_cast<unsigned char>(r * 255);
                    newPixels[index + 1] = static_cast<unsigned char>(g * 255);
                    newPixels[index + 2] = static_cast<unsigned char>(b * 255);
                    newPixels[index + 3] = static_cast<unsigned char>(a * 255);
                }
            }
            result.pixels = std::move(newPixels);
        }
    }
    
    return result;
}

bool WriteImageToClipboard(const BitmapData& bmpData) {
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:NULL
        pixelsWide:bmpData.width
        pixelsHigh:bmpData.height
        bitsPerSample:8
        samplesPerPixel:4
        hasAlpha:YES
        isPlanar:NO
        colorSpaceName:NSCalibratedRGBColorSpace
        bytesPerRow:bmpData.width * 4
        bitsPerPixel:32];
    
    memcpy([bitmap bitmapData], bmpData.pixels.data(), bmpData.pixels.size());
    
    NSData *tiffData = [bitmap TIFFRepresentation];
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    return [pasteboard setData:tiffData forType:NSPasteboardTypeTIFF];
}

bool SaveBitmapToFile(const BitmapData& bmpData, const std::wstring& filePath) {
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:NULL
        pixelsWide:bmpData.width
        pixelsHigh:bmpData.height
        bitsPerSample:8
        samplesPerPixel:4
        hasAlpha:YES
        isPlanar:NO
        colorSpaceName:NSCalibratedRGBColorSpace
        bytesPerRow:bmpData.width * 4
        bitsPerPixel:32];
    
    memcpy([bitmap bitmapData], bmpData.pixels.data(), bmpData.pixels.size());
    
    NSData *pngData = [bitmap representationUsingType:NSBitmapImageFileTypePNG properties:@{}];
    NSString *path = [NSString stringWithUTF8String:WstringToUtf8(filePath).c_str()];
    return [pngData writeToFile:path atomically:YES];
}

bool SaveClipboardImageToFile(const std::wstring& filePath) {
    BitmapData bmpData = ReadImageFromClipboard();
    if (bmpData.pixels.empty()) return false;
    return SaveBitmapToFile(bmpData, filePath);
}
