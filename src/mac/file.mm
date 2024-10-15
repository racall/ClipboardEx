#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "../clipboard.h"

std::vector<FileInfox> ReadFilesFromClipboard() {
    std::vector<FileInfox> result;

    @autoreleasepool {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        NSArray *classes = @[[NSURL class]];
        NSDictionary *options = @{NSPasteboardURLReadingFileURLsOnlyKey : @YES};

        NSArray *fileURLs = [pasteboard readObjectsForClasses:classes options:options];

        if (fileURLs) {
            for (NSURL *fileURL in fileURLs) {
                NSError *error = nil;
                NSDictionary *attributes = [[NSFileManager defaultManager] attributesOfItemAtPath:[fileURL path] error:&error];

                if (!error) {
                    FileInfox fileInfo;
                    fileInfo.path = std::string([[fileURL path] UTF8String]);
                    fileInfo.name = std::string([[fileURL lastPathComponent] UTF8String]);
                    fileInfo.size = [attributes fileSize];

                    result.push_back(fileInfo);
                } else {
                    std::cerr << "Error reading file attributes: " << [[error localizedDescription] UTF8String] << std::endl;
                }
            }
        }
    }

    return result;
}

bool WriteFilesToClipboard(const std::vector<std::string>& files) {
    @autoreleasepool {
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        [pasteboard clearContents];

        NSMutableArray *fileURLs = [NSMutableArray array];
        for (const auto& filePath : files) {
            NSURL *fileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:filePath.c_str()]];
            if (fileURL) {
                [fileURLs addObject:fileURL];
            } else {
                std::cerr << "Invalid file path: " << filePath << std::endl;
                return false;
            }
        }

        BOOL success = [pasteboard writeObjects:fileURLs];
        if (!success) {
            std::cerr << "Failed to write files to clipboard." << std::endl;
        }

        return success;
    }
}
