#include <stdio.h>

//in utf-16 we have 2 bytes to represent char.
#define NUM_BYTES_CHAR 2

//we only read to one buffer
#define NUM_BUFFERS 1

//in utf-16 we have 2 bytes to represent char.
typedef struct 
{
    unsigned char firstByte;
    unsigned char secondByte;
}FileChar;


void createFileFromFile(const char* inFile, const char* outFile,
        FileChar (**handleFileChar)(FileChar fc), int numFuncs){

    FILE *in = fopen(inFile,"rb");  // r for read, b for binary
    FILE *out = fopen(outFile,"wb");  // w for write, b for binary

    for(;;){
        unsigned char inCharBuffer[NUM_BYTES_CHAR];
        int buffersRead = fread(inCharBuffer, sizeof(inCharBuffer), NUM_BUFFERS, in);

        if(buffersRead != NUM_BUFFERS){
            break;
        }

        //we are taking the fist and second byte we read and
        //put them in FileChar in the sequence we have read them.
        FileChar fc = {inCharBuffer[0], inCharBuffer[1]};

        for(int seq = 0; seq <= numFuncs; ++seq) {
            fc = handleFileChar[seq](fc);
        }

        unsigned char outCharBuffer[] = {fc.firstByte, fc.secondByte};
        fwrite(outCharBuffer, sizeof(outCharBuffer), NUM_BUFFERS, out);
    }

    fclose(in);
    fclose(out);
}

int main(int argc, char const *argv[])
{
    printf("hi");
    return 0;
}