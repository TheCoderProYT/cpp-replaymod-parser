#pragma once

#pragma once

#include <string>
#include <cstdint>
#include <cstdint>

/*
Intended to print file sizes
Examples: (input numbers are bytes)
    144243 -> 144.23 KB
    1 -> 001.00 B
    1500111222 -> 001.50 GB

Inputs:
    size_t num : file size in bytes
Returns char* : Formatted file size
*/
char * printFileSize(size_t fileSize);

/*
Intended to print big numbers.
Example: 123456 -> 123.45K

Inputs:
    size_t num : input number
Returns char* : formatted output
*/
char * printBigNum(size_t num);