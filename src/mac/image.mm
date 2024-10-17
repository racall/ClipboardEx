#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "../clipboard.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
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

bool WriteImageToClipboard(const uint8_t* buffer, size_t bufferSize) {
    if (bufferSize > INT_MAX) {
        // 如果 bufferSize 超过 int 的最大值，返回失败
        return false;
    }
    // 使用 stb_image 加载图像
    int width, height, channels;
    unsigned char* img = stbi_load_from_memory(buffer, static_cast<int>(bufferSize), &width, &height, &channels, 0);
    
    if (!img) {
        return false; // 图像加载失败
    }

    // 创建 NSImage 对象
    NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(uint8_t **)&img
                                                                      pixelsWide:width
                                                                      pixelsHigh:height
                                                                       bitsPerSample:8
                                                                     samplesPerPixel:channels
                                                                            hasAlpha:(channels == 4)
                                                                            isPlanar:NO
                                                                      colorSpaceName:NSCalibratedRGBColorSpace
                                                                        bytesPerRow:width * channels
                                                                       bitsPerPixel:channels * 8];

    NSImage *image = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
    [image addRepresentation:bitmapRep];

    // 获取剪贴板
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    
    // 清空剪贴板
    [pasteboard clearContents];
    
    // 将图像写入剪贴板
    BOOL success = [pasteboard setData:[bitmapRep TIFFRepresentation] forType:NSPasteboardTypeTIFF];
    
    // 释放资源
    stbi_image_free(img);

    return success;
}

IconData IconToIconData(NSImage *image) {
    IconData iconData = {nullptr, 0, 0, 0};

    // 获取图像的大小
    NSSize size = [image size];
    iconData.width = size.width;
    iconData.height = size.height;

    // 创建一个 bitmap context
    NSBitmapImageRep *bitmapRep = [NSBitmapImageRep alloc];
    bitmapRep = [bitmapRep initWithBitmapDataPlanes:NULL
                                            pixelsWide:size.width
                                            pixelsHigh:size.height
                                         bitsPerSample:8
                                       samplesPerPixel:4
                                          hasAlpha:YES
                                          isPlanar:NO
                                        colorSpaceName:NSCalibratedRGBColorSpace
                                           bytesPerRow:0
                                          bitsPerPixel:0];

    [NSGraphicsContext saveGraphicsState];
    NSGraphicsContext *context = [NSGraphicsContext graphicsContextWithBitmapImageRep:bitmapRep];
    [NSGraphicsContext setCurrentContext:context];

    // 绘制图标到上下文
    [image drawInRect:NSMakeRect(0, 0, size.width, size.height)];

    [NSGraphicsContext restoreGraphicsState];

    // 获取图像数据
    NSData *data = [bitmapRep representationUsingType:NSBitmapImageFileTypePNG properties:@{}];
    iconData.dataSize = [data length];
    iconData.buffer = new char[iconData.dataSize];
    memcpy(iconData.buffer, [data bytes], iconData.dataSize);

    return iconData;
}

IconData GetFileIconData(const std::string &filePath) {
    IconData result = {nullptr, 0, 0, 0};
    
    // 将 std::string 转换为 NSString
    NSString *nsFilePath = [NSString stringWithUTF8String:filePath.c_str()];
    NSURL *fileURL = [NSURL fileURLWithPath:nsFilePath];

    // 获取文件的图标
    NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
    NSImage *icon = [workspace iconForFile:fileURL.path];

    if (icon) {
        result = IconToIconData(icon);
    } else {
        std::cout << "Failed to get file icon data" << std::endl;
    }

    return result;
}

