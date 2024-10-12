const clipboard = require('./index');
const sharp = require('sharp');
const fs = require('fs');

console.log("获取类型：",clipboard.getContentType())
// console.log("清理剪切板",clipboard.clear())
// console.log("clipboard",clipboard);
// function extractFragment(html) {
//     const regex = /<!--StartFragment-->([\s\S]*?)<!--EndFragment-->/;
//     const match = regex.exec(html);

//     return match ? match[1].trim() : '';
// }
// console.log("读取剪切板文本:");
// console.log(clipboard.readText());

// console.log("\nWriting text to clipboard...");
// clipboard.writeText("Hello from Node.js!");
// console.log("Text written to clipboard. Reading it back:");
// console.log(clipboard.readText());

// console.log("\nReading files from clipboard:");
// const files = clipboard.readFiles();
// console.log(files);

// console.log("\nWriting files to clipboard...");
// clipboard.writeFiles(['C:\\Users\\brian\\Desktop\\winhole.ahk']);
// console.log("从剪切板里读取文件:");
// console.log(clipboard.readFiles());
// // console.log(clipboard.getFileIcon("C:\\Users\\brian\\Desktop\\winhole.ahk"))
// const { buffer, width, height } = clipboard.getFileIcon("D:\\Program Files\\BaiduNetdisk\\BaiduNetdisk.exe");
// // fs.writeFileSync("./icon.ico", buffer);
// console.log(`First 16 bytes of the buffer: ${buffer.slice(0, 16).toString('hex')}`);
// console.log("buffer",buffer);
// const base64Data = buffer.toString('base64');

// // 创建完整的 data URL
// const dataUrl = `data:image/png;base64,${base64Data}`;
// console.log(dataUrl);
// 使用 Sharp 将 ICO 格式转换为 PNG
// sharp(buffer, {
//     density: 300  // 这有助于保持图标的清晰度
// }).png().toBuffer().then(pngBuffer => {
//     // 将 PNG Buffer 转换为 base64 字符串
//     const base64Data = pngBuffer.toString('base64');

//     // 创建完整的 data URL
//     const dataUrl = `data:image/png;base64,${base64Data}`;
//     console.log(dataUrl);
// });

// function uint8ArrayToBase64(uint8Array) {
//     // 创建一个 Buffer 对象
//     const buffer = Buffer.from(uint8Array);
//     // 将 Buffer 转换为 base64 字符串
//     return buffer.toString('base64');
// }
// try {
//     console.log("\n读取剪贴板中的图片:");
//     const image = clipboard.readImage();
//     console.log("img: ", image);
//     if (image.width > 0 && image.height > 0) {
//         console.log(`图片尺寸: ${image.width}x${image.height}`);
//         console.log(`图片数据长度: ${image.data.length} 字节`);
//         fs.writeFileSync('clipboard_image_verify.png', image.data);
//         console.log('Image data saved to clipboard_image_verify.png');
//         clipboard.writeText("Hello from Node.js!");
//         let a = fs.readFileSync('clipboard_image_verify.png');
//         console.log("读取图片数据:", a);
//         // const dataUrl = `data:image/png;base64,${image.data.toString("base64")}`;
//         // console.log(dataUrl,"\n",Buffer.from(image.data.toString("base64"),"base64"));
//         setTimeout(() => {
//             const a = clipboard.writeImage(image.data);
//             console.log("写入剪贴板",a);
//         }, 15000);
//     } else {
//         console.log("剪贴板中没有图片");
//     }
// } catch (error) {
//     console.error("Error:", error);
// }


// const savePath = "./image.png";
// const success = clipboard.saveImageToFile(savePath);

// if (success) {
//     console.log("Image saved successfully!");
// } else {
//     console.log("Failed to save the image.");
// }
// console.log("\n写入图片到剪贴板...");