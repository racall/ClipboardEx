declare module "clipboardex" {
    /**
     * 清除剪贴板
     */
    export function clear(): boolean;

    /**
     * 保存剪贴板图片到文件
     * @param filePath 文件路径
     */
    export function saveImageToFile(filePath: string): boolean;

    /**
     * 读取剪贴板文本
     */
    export function readText(): string;

    /**
     * 读取剪贴板HTML
     */
    export function readHTML(): string;

    /**
     * 写入剪贴板文本
     * @param text 文本
     */
    export function writeText(text: string): boolean;

    interface FileInfo {
        path: string;
        name: string;
        size: number;
    }

    /**
     * 读取剪贴板文件
     * @return 文件信息数组
     */
    export function readFiles(): FileInfo[];

    /**
     * 写入剪贴板文件
     * @param files 文件路径数组
     */
    export function writeFiles(files: string[]): boolean;

    interface BitmapData {
        width: number;
        height: number;
        pixels: Uint8Array;
    }

    /**
     * 读取剪贴板图片
     * @return 包含宽度、高度和像素数据的对象
     */
    export function readImage(): BitmapData;

    /**
     * 写入剪贴板图片
     * @param image 包含宽度、高度和像素数据的对象
     */
    export function writeImage(image: BitmapData): boolean;

    /**
     * Represents the result of getting a file icon.
     */
    interface FileIconResult {
        /**
         * The raw buffer containing the icon data in RGBA format.
         */
        buffer: Buffer;

        /**
         * The width of the icon in pixels.
         */
        width: number;

        /**
         * The height of the icon in pixels.
         */
        height: number;
    }

    /**
     * Retrieves the icon for the specified file.
     * @param filePath The full path to the file.
     * @throws Will throw an error if the file icon cannot be retrieved or if there's an issue with the file path.
     */
    function getFileIcon(filePath: string): FileIconResult;

    export { getFileIcon, FileIconResult };
}
