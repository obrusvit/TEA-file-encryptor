# TEA-file-encryptor
> [!WARNING]  
> This program is not intended for serious usage. It was done for educational purposes. Use at own risk.

This program encrypts or decrypts a file with [TEA (Tiny Encryption Algorithm)](https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm) with a user defined key entered upon prompt.
It allows hiding sensitive text files on your PC or flashdrive.
The algorithm implementation is taken from a book: *Programming: Principles and Practice Using C++, Second Edition* by Bjarne Stroustup.
This repo adds a command line interface (CLI) to facilitate inteded usage.

## Example usage.
To encrypt a file:
```
./TEA-file-encryptor -a encrypt -s file.txt -t file_encrypted.txt
```
To decrypt a file:
```
./TEA-file-encryptor -a decrypt -s file_encrypted.txt -t file_decrypted.txt
```

## Build
Needed:
- reasonably new C++ compiler (tested with `gcc` version `11.4.0`)
- CMake (`>3.16`)
- Unix system (hiding terminal echo when entering key works only on Unix)

The project uses [CPM](https://github.com/cpm-cmake/CPM.cmake) to handle dependencies. The script is included in `cmake` directory.
```
mkdir build && cd build
cmake ../
make
```

## Test
Simple end-to-end tests implemented with Catch2. Build and run:
```
./build/tests
```
