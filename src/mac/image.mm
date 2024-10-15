#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "../clipboard.h"

ImageData ReadImageFromClipboard() {
    ImageData result = {
        .data = {},
        .size = 0,
        .width = 0,
        .height = 0
    };
    @autoreleasepool {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSArray *classArray = @[ [NSImage class] ];
        NSDictionary *options = @{};
        if (![pasteboard canReadObjectForClasses:classArray options:options]) {
            return result;
        }
        
        NSArray *objectsToPaste = [pasteboard readObjectsForClasses:classArray options:options];
        if (objectsToPaste.count == 0) {
            return result;
        }
        
        NSImage *image = objectsToPaste.firstObject;
        if (!image) {
            return result;
        }
        
        NSBitmapImageRep *bitmapRep = nil;
        for (NSImageRep *rep in image.representations) {
            if ([rep isKindOfClass:[NSBitmapImageRep class]]) {
                bitmapRep = (NSBitmapImageRep *)rep;
                break;
            }
        }
        
        if (!bitmapRep) {
            return result;
        }
        
        // Getting width and height
        result.width = (int)bitmapRep.pixelsWide;
        result.height = (int)bitmapRep.pixelsHigh;
        
        // Convert the image to PNG data format
        NSData *pngData = [bitmapRep representationUsingType:NSBitmapImageFileTypePNG properties:@{}];
        if (!pngData) {
            return result;
        }
        
        result.size = pngData.length;
        result.data.assign((unsigned char *)pngData.bytes, (unsigned char *)pngData.bytes + pngData.length);
    }
    return result;
}