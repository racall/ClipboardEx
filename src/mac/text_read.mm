#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "../clipboard.h"

std::string ReadTextFromClipboard(){
    @autoreleasepool {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSString *string = [pasteboard stringForType:NSPasteboardTypeString];
        if (string) {
            return std::string([string UTF8String]);
        }
    }
    return "";
}

std::string ReadHTMLFromClipboard(){
    @autoreleasepool {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSString *string = [pasteboard stringForType:NSPasteboardTypeHTML];
        if (string) {
            return std::string([string UTF8String]);
        }
    }
    return "";
}