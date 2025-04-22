#### 关于这个项目

这个项目是我在使用node.js过程中发现没有好用的剪切板操作库，于是就自己写了一个

#### Node.js调用剪切板扩展

支持windows和macOS的剪切板相关操作，可以自由使用

#### 编译
```bash
pnpm prebuild
```

#### 使用

使用案例可以参考test.js文件

参考：

[windows相关函数：GetClipboardData](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboarddata)

[macOS相关函数：NSPasteboard](https://developer.apple.com/documentation/appkit/nspasteboard?language=objc)

[electron-clipboard-ex](https://github.com/kenan2002/electron-clipboard-ex/tree/master)

