const {
    getContentType,
    readText,
    readHTML,
    writeText,
    readImage,
    writeImage,
    readFiles,
    writeFiles,
    saveImageToFile,
    getFileIcon
} = require('node-gyp-build')(__dirname)

module.exports = {
    getContentType,
    readText,
    readHTML,
    writeText,
    readImage,
    writeImage,
    readFiles,
    writeFiles,
    saveImageToFile,
    getFileIcon
}
