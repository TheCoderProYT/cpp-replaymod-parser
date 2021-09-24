#include "PrintBigNumbers.hpp"

std::string fileSizeSuffixes[5] = {
    "B",
    "KB",
    "MB",
    "GB",
    "TB"
};
std::string bigNumSuffixes[5] = {
    "",
    "K",
    "M",
    "G",
    "T"
};

char * printFileSize(size_t fileSize) {
    float fsD = (float)fileSize;
    char* buf = new char[256];
    long size = 1;
    for(int i = 0; i < 5; i++) {
        if((fileSize < size*1000) || (i == 4)) {
            sprintf(buf,"%3.2f %s",1000+(fsD/size),fileSizeSuffixes[i].c_str());
            break;
        }
        size*=1000;
    }
    return buf+1;
}
char * printBigNum(size_t num) {
    float nD = (float)num;
    char* buf = new char[256];
    long size = 1;
    for(int i = 0; i < 5; i++) {
        if((num < size*1000) || (i == 4)) {
            sprintf(buf,"%3.2f%s",1000+(nD/size),bigNumSuffixes[i].c_str());
            break;
        }
        size*=1000;
    }
    return buf+1;
}