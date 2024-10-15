#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "../clipboard.h"

bool WriteTextToClipboard(const std::string& text){
    @autoreleasepool {
        NSPasteboard *pboard = [NSPasteboard generalPasteboard];
        [pboard clearContents];
        return [pboard setString:[NSString stringWithUTF8String:text.c_str()] forType:NSPasteboardTypeString];
    }
    return false;
}