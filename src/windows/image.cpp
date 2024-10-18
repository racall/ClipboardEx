#define UNICODE
#include "../clipboard.h"
#include "utils.h"
#include <shlobj.h>
#include <gdiplus.h>
#include <gdiplusinit.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

/**
 * Write image to clipboard
 * @param bmpData BitmapData
 * @return bool
 */
std::vector<uint8_t> convertToDIB(const uint8_t* imageData, int width, int height, int channels) {
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height; // top-down DIB
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    size_t dibSize = sizeof(BITMAPINFOHEADER) + (width * height * 4);
    std::vector<uint8_t> dibData(dibSize);

    memcpy(dibData.data(), &bi, sizeof(BITMAPINFOHEADER));
    
    // Convert to BGRA
    for (int i = 0; i < width * height; ++i) {
        dibData[sizeof(BITMAPINFOHEADER) + i * 4 + 0] = imageData[i * channels + 2];
        dibData[sizeof(BITMAPINFOHEADER) + i * 4 + 1] = imageData[i * channels + 1];
        dibData[sizeof(BITMAPINFOHEADER) + i * 4 + 2] = imageData[i * channels + 0];
        dibData[sizeof(BITMAPINFOHEADER) + i * 4 + 3] = channels == 4 ? imageData[i * channels + 3] : 255;
    }

    return dibData;
}

HGLOBAL createGlobalMemory(const std::vector<uint8_t>& data) {
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, data.size());
    if (hMem) {
        void* pMem = GlobalLock(hMem);
        memcpy(pMem, data.data(), data.size());
        GlobalUnlock(hMem);
    }
    return hMem;
}

bool WriteImageToClipboard(const uint8_t* buffer, size_t bufferSize) {
    int width, height, channels;
    unsigned char* img = stbi_load_from_memory(buffer, bufferSize, &width, &height, &channels, 0);
    
    if (!img) {
        return false;
    }

    if (!OpenClipboard(NULL)) {
        stbi_image_free(img);
        return false;
    }

    EmptyClipboard();

    // Set DIB format
    std::vector<uint8_t> dibData = convertToDIB(img, width, height, channels);
    HGLOBAL hDib = createGlobalMemory(dibData);
    if (hDib && SetClipboardData(CF_DIB, hDib) == NULL) {
        GlobalFree(hDib);
    }

    // Set PNG format
    UINT pngFormat = RegisterClipboardFormat(L"PNG");
    if (pngFormat) {
        HGLOBAL hPng = createGlobalMemory(std::vector<uint8_t>(buffer, buffer + bufferSize));
        if (hPng && SetClipboardData(pngFormat, hPng) == NULL) {
            GlobalFree(hPng);
        }
    }

    stbi_image_free(img);
    CloseClipboard();

    return true;
}


/**
 * Convert icon to icon data
 * @param hIcon HICON
 * @return IconData
 */
IconData IconToIconData(HICON hIcon)
{
    // 初始化 GDI+
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 将 HICON 转换为 Bitmap
    Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromHICON(hIcon);

    // 获取宽度和高度
    int width = bmp->GetWidth();
    int height = bmp->GetHeight();

    // 创建 IStream
    IStream *istream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &istream);

    // 将 Bitmap 保存到 IStream 中（PNG 格式）
    CLSID pngClsid;
    CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
    bmp->Save(istream, &pngClsid, NULL);

    // 获取 IStream 的大小
    STATSTG statstg;
    istream->Stat(&statstg, STATFLAG_DEFAULT);
    ULONG size = statstg.cbSize.LowPart;

    // 读取 IStream 数据到缓冲区
    HGLOBAL hGlobal = NULL;
    GetHGlobalFromStream(istream, &hGlobal);
    void *buffer = GlobalLock(hGlobal);

    // 为图标数据分配内存
    char *data = new char[size];
    memcpy(data, buffer, size);

    // 解锁和释放资源
    GlobalUnlock(hGlobal);
    istream->Release();
    delete bmp;
    DestroyIcon(hIcon);
    Gdiplus::GdiplusShutdown(gdiplusToken);

    // 创建并返回 IconData 结构体
    IconData iconData;
    iconData.buffer = data;
    iconData.width = width;
    iconData.height = height;
    iconData.dataSize = size;

    return iconData;
}

/**
 * Get file icon data
 * @param filePath std::string
 * @return IconData
 */
IconData GetFileIconData(const std::string& filePath)
{
    IconData result = {nullptr, 0, 0, 0};
    SHFILEINFOW shFileInfo;
    std::wstring wfilePath = Utf8ToWstring(filePath);
    if (!SHGetFileInfoW(wfilePath.data(), 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_LARGEICON))
    {
        std::cout << "Failed to get file icon data" << std::endl;
        return result;
    }

    result = IconToIconData(shFileInfo.hIcon);
    return result;
}

/**
 * Get encoder clsid
 * @param format const WCHAR*
 * @param pClsid CLSID*
 * @return bool
 */
bool GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT  num = 0;          // Number of image encoders
    UINT  size = 0;         // Size of the image encoder array in bytes

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0) {
        return false;  // Failure
    }

    std::unique_ptr<BYTE[]> p(new BYTE[size]);
    Gdiplus::ImageCodecInfo* pImageCodecInfo = reinterpret_cast<Gdiplus::ImageCodecInfo*>(p.get());

    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            return true;
        }
    }

    return false;  // Failure
}

/**
 * Read image from clipboard
 * @return ImageData
 */
ImageData ReadImageFromClipboard() {
    // 初始化 GDI+
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    ImageData result;
    if (!OpenClipboard(nullptr)) {
        return result;
    }  
    // if (GetClipboardData(CF_DIB)) {
    //     std::cout << "get CF_DIB !!!" << std::endl;
    // }
    HANDLE hData = GetClipboardData(CF_BITMAP);
    if (hData == NULL)
    {
        CloseClipboard();
        return result;
    }
    HBITMAP hBitmap = (HBITMAP)hData;
    // 使用GDI+将HBITMAP转换为Image对象
    Gdiplus::Bitmap *bitmap = Gdiplus::Bitmap::FromHBITMAP(hBitmap, NULL);
    if (bitmap == NULL)
    {
        std::cout << "Failed to create GDI+ bitmap" << std::endl;
        CloseClipboard();
        return result;
    }
    UINT width = bitmap->GetWidth();
    UINT height = bitmap->GetHeight();

    // 创建 IStream
    IStream *istream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &istream);

    // 将 Bitmap 保存到 IStream 中（PNG 格式）
    CLSID pngClsid;
    CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
    bitmap->Save(istream, &pngClsid, NULL);

    // 获取 IStream 的大小
    STATSTG statstg;
    istream->Stat(&statstg, STATFLAG_DEFAULT);
    ULONG size = statstg.cbSize.LowPart;

    // 读取 IStream 数据到缓冲区
    HGLOBAL hGlobal = NULL;
    GetHGlobalFromStream(istream, &hGlobal);
    void *buffer = GlobalLock(hGlobal);

    // 为图标数据分配内存
    char *data = new char[size];
    memcpy(data, buffer, size);

    // 解锁和释放资源
    GlobalUnlock(hGlobal);
    istream->Release();
    delete bitmap;
    Gdiplus::GdiplusShutdown(gdiplusToken);

    result.size = size;
    result.width = width;
    result.height = height;
    result.data.assign(data, data + size);
    return result;
}

void FreeIconData(IconData &iconData)
{
    if (iconData.buffer)
    {
        delete[] iconData.buffer;
        iconData.buffer = nullptr;
    }
    iconData.width = 0;
    iconData.height = 0;
    iconData.dataSize = 0;
}