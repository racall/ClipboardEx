#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "../clipboard.h"

ClipboardContentType GetClipboardContentType(){
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSArray *types = [pasteboard types];
    if ([types count] == 0) {
        return ClipboardContentType::Empty;
    }
    if ([types containsObject:NSPasteboardTypeHTML]) {
        return ClipboardContentType::Html;
    } else if ([types containsObject:NSPasteboardTypeFileURL]) {
        return ClipboardContentType::File;
    }
    if ([types containsObject:NSPasteboardTypeString]) {
        return ClipboardContentType::Text;
    } else if ([types containsObject:NSPasteboardTypeTIFF] || [types containsObject:NSPasteboardTypePNG]) {
        return ClipboardContentType::Image;
    }
    return ClipboardContentType::Unkonwn;
}

bool clearClipboard(){
    return [NSPasteboard generalPasteboard].clearContents;
}