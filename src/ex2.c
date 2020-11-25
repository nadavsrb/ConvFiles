#include <stdio.h>

//utf-16 is using 2 bytes to store one char.
#define CHAR_SIZE_IN_FILE 2

void createFileFromFile(const char* inFile, const char* outFile,
     unsigned char* (*handleChar)(const char* outFile, unsigned char c[CHAR_SIZE_IN_FILE])){
         
    unsigned char charBuffer[CHAR_SIZE_IN_FILE];

    FILE *f = fopen("test.bin","rb");  // r for read, b for binary

    fread(charBuffer,sizeof(charBuffer),1,f);
}

int main(int argc, char const *argv[])
{
    printf("hi");
    return 0;
}