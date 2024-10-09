const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');
const colors = require('colors');
// 删除build文件夹的函数
function deleteBuildFolder() {
  const buildPath = path.join(__dirname, 'build');
  if (fs.existsSync(buildPath)) {
    console.log('Deleting build folder...'.bold.red);
    fs.rmSync(buildPath, { recursive: true, force: true });
    console.log('Build folder deleted.\n'.bold.green);
  } else {
    console.log('Build folder does not exist. Skipping deletion.'.bold.red);
  }
}

// 执行命令的函数
function runCommand(command) {
  console.log(`Running command: ${command}\n`.bold.green);
  try {
    execSync(command, { stdio: 'inherit' });
    console.log(`Command completed successfully: ${command}\n`.bold.green);
  } catch (error) {
    console.error(`Error executing command: ${command}\n`.bold.red);
    console.error(error);
    process.exit(1);
  }
}

// 主函数
function main() {
  // 删除build文件夹
  deleteBuildFolder();

  // 执行 npx node-gyp configure
  runCommand('npx node-gyp configure');

  // 执行 npx node-gyp build
  runCommand('npx node-gyp build');

  console.log('\nAll tasks completed successfully.'.brightGreen.bold);
}

// 运行主函数
main();