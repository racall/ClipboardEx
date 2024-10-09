{
    "targets": [
        {
            "target_name": "ClipboardEx",
            "sources": [ 
                "src/main.cpp"
            ],
            "include_dirs": [
                "include",
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            # "msvs_settings": {
            #     "VCCLCompilerTool": {
            #     "ExceptionHandling": 1,
            #     "AdditionalOptions": ["/std:c++11"]
            #     }
            # },
            # "cflags": ["-m", "-EHsc"],
            # "cflags_cc": ["-std=c++11"],
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
            # "cflags_cc": [ "-std=c++11" ],
            "conditions": [
                [
                    'OS=="win"',
                    {
                        'sources': [ 
                            # "src/windows_clipboard.cpp",
                            "src/windows/common.cpp",
                            "src/windows/file.cpp",
                            "src/windows/image.cpp",
                            "src/windows/text_read.cpp",
                            "src/windows/text_write.cpp"
                         ],
                        "libraries": [ "user32.lib", "shell32.lib", "gdi32.lib","Gdiplus.lib"],
                        "msvs_settings": {
                            "VCCLCompilerTool": {
                                "AdditionalOptions": [
                                    "/EHsc" #允许抛出异常
                                ]
                            }
                        }
                    }
                ],
                [
                    'OS=="mac"',
                    {
                        'sources': [ 
                            # "src/mac/common.cpp",
                            # "src/mac/file.cpp",
                            # "src/mac/image.cpp",
                            # "src/mac/text_read.cpp",
                            # "src/mac/text_write.cpp"
                        ],
                        'link_settings': {
                            'libraries': [
                                '-framework Cocoa',
                                '-framework CoreFoundation',
                            ],
                        },
                        "xcode_settings": {
                            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                            "CLANG_ENABLE_OBJC_ARC": "YES",
                        }
                    }
                ]
            ]
        }
    ],
    # 'variables' : {
    #     'openssl_fips': '',
    # },
}