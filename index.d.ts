declare module "clipboardex" {
    /**
     * 清除剪贴板
     */
    export function clear(): boolean;


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

    interface ImageData {
        width: number;
        height: number;
        size: number;
        data: Uint8Array;
    }
    /**
     * 读取剪贴板图片
     * @return ImageData 包含宽度、高度、大小、像素数据的对象
     */
    export function readImage(): ImageData;

    /**
     * 写入剪贴板图片
     * @param image 图片数据
     */
    export function writeImage(image: Buffer): boolean;

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
