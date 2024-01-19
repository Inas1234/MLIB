# Build tool for C++

## Overview
This build tool is a C++ based utility designed to automate and simplify the build process for C++ projects. It includes features like hot reloading, command execution, and logging, making it an essential tool for C++ developers who seek to streamline their build and development process.

## Features
* **Command Execution:** Execute predefined commands with ease.
* **Hot Reloading:** Automatically detect changes in specified files and trigger build commands.
* **Logging:** Log messages with different severity levels (INFO, WARNING, ERROR).
* **Cross-Platform Compatibility:** Designed to work on multiple operating systems.
* **Dynamic File Handling:** Read from and write to files with dynamic memory management.
* **OS-Specific** Command Handling: Execute OS-specific system commands.

## Usage
### Basic Setup
To use this tool, first create a file `mlib.cpp` and then include `mlib.h` afterwards you need to create an instance of the `Cmd` class. Configure the tool with the commands you wish to execute and the files you want to watch.
### Executing Commands
Use the `mlib_append` method to add commands and mlib_execute to run them. You can chain multiple commands as needed.
### File Watching
You first need to enable the reabuild with the `mlib_rebuild()` and then after you append the command use `mlib_watch()`
### Reading and Writing Files
Utilize `mlib_read_file` and `mlib_write_to_file` for file operations. These functions handle file reading and writing with dynamic sizing.
### Logging
Use `mlib_log` to log messages throughout your application. This function supports different log levels for better message categorization.

## Example
```cpp
#include "Mlib.h"   

int main(){
    Cmd cmd;
    cmd.mlib_rebuild();
    cmd.mlib_append("g++", "main.cpp", "-o", "main");
    cmd.mlib_watch();
    return 0;
}
```
Then you can bootstrap it by using your c++ compiler for example `g++ Mlib.cpp -o ml.exe`  and then run the `ml.exe`

