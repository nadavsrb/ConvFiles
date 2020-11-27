#include <stdio.h>
#include <string.h>

//in utf-16 we have 2 bytes to represent char.
#define NUM_BYTES_CHAR 2

//in windows EOL is represent by 2 chars.
#define EOL_MAX_CHARS 2

#define CHARS_ARRAY_MAX_LENGTH 2

//we only read to one buffer
#define NUM_BUFFERS 1

#define NO_MORE_CHARS_TO_READ 0

#define BOM_FIRST_BYTE_BIG_ENDIAN 0xFE
#define BOM_SECOND_BYTE_BIG_ENDIAN 0xFF

//order according to big endian:
#define WIN_EOL_FIRST_CHAR_FIRST_BYTE  0x00
#define WIN_EOL_FIRST_CHAR_SECOND_BYTE  0X0d
#define WIN_EOL_SECOND_CHAR_FIRST_BYTE  0x00
#define WIN_EOL_SECOND_CHAR_SECOND_BYTE  0x0a
#define MAC_EOL_FIRST_BYTE  0x00
#define MAC_EOL_SECOND_BYTE  0x0d
#define UNIX_EOL_FIRST_BYTE  0x00
#define UNIX_EOL_SECOND_BYTE  0x0a

//length means how many chars.
#define WIN_EOL_LENGTH 2
#define MAC_EOL_LENGTH 1
#define UNIX_EOL_LENGTH 1

typedef enum{
    False,
    True
}Bool;

typedef enum{
    NON,
    WIN,
    MAC,
    UNIX
}Os;

typedef struct{
    Os srcFileOs;
    Os destFileOs;
    Bool isSwap;
}Commaned;

//in utf-16 we have 2 bytes to represent char.
typedef struct 
{
    Bool isEOF;
    unsigned char firstByte;
    unsigned char secondByte;
}FileChar;

typedef struct 
{
    FileChar fCArr[CHARS_ARRAY_MAX_LENGTH];
    int length;
}InFileCharArray;

typedef struct 
{
     //because every old char can become two new chars
    FileChar fCArr[2 * CHARS_ARRAY_MAX_LENGTH];
    int length;
}OutFileCharArray;

void switchBytesInChar(FileChar* fc){
    if(fc == NULL || fc->isEOF) {
        return;
    }

    unsigned char firstByte = fc->firstByte;

    fc->firstByte = fc->secondByte;
    fc->secondByte = firstByte;
};

FileChar readOneChar(FILE *in) {
    FileChar fc;
    fc.isEOF = False;

    unsigned char inCharBuffer[NUM_BYTES_CHAR];
    int buffersRead = fread(inCharBuffer, sizeof(inCharBuffer), NUM_BUFFERS, in);

    if(buffersRead != NUM_BUFFERS){
        fc.isEOF = True;
    }

    //we are taking the fist and second byte we read and
    //put them in FileChar in the sequence we have read them.
    fc.firstByte = inCharBuffer[0];
    fc.secondByte = inCharBuffer[1];

    return fc;
}

Bool isFileBigEndian(FileChar* endianChar) {
    if(endianChar == NULL || endianChar->isEOF) {
        return False;
    }
    
    return (endianChar->firstByte == BOM_FIRST_BYTE_BIG_ENDIAN) &&
            (endianChar->secondByte == BOM_SECOND_BYTE_BIG_ENDIAN);
}

Bool isStartCharOfWinLOF(const FileChar* c, Bool isSrcInBigEndian) {
    if(c == NULL || c->isEOF) {
        return False;
    }

    if(isSrcInBigEndian){
        if(c->firstByte == WIN_EOL_FIRST_CHAR_FIRST_BYTE &&
            c->secondByte == WIN_EOL_FIRST_CHAR_SECOND_BYTE) {
                return True;
        }

        return False;
    }

    if(c->secondByte == WIN_EOL_FIRST_CHAR_FIRST_BYTE &&
        c->firstByte == WIN_EOL_FIRST_CHAR_SECOND_BYTE) {
            return True;
    }

    return False;
}

OutFileCharArray convertBigEndianOs(Os src, Os dest, const InFileCharArray* oldChars){
    OutFileCharArray newChars;
    newChars.length = 0;

    if(oldChars == NULL) {
        return newChars;
    }

    FileChar eolSrcChars[EOL_MAX_CHARS];
    eolSrcChars[0].isEOF = False;
    eolSrcChars[1].isEOF = False;
    int eolSrcLength = 0;

    switch (src) {
    case WIN:
        eolSrcLength = WIN_EOL_LENGTH;

        eolSrcChars[0].firstByte = WIN_EOL_FIRST_CHAR_FIRST_BYTE;
        eolSrcChars[0].secondByte = WIN_EOL_FIRST_CHAR_SECOND_BYTE;
        eolSrcChars[1].firstByte = WIN_EOL_SECOND_CHAR_FIRST_BYTE;
        eolSrcChars[1].secondByte = WIN_EOL_SECOND_CHAR_SECOND_BYTE;
        break;

    case MAC:
        eolSrcLength = MAC_EOL_LENGTH;

        eolSrcChars[0].firstByte = MAC_EOL_FIRST_BYTE;
        eolSrcChars[0].secondByte = MAC_EOL_SECOND_BYTE;
        break;

    case UNIX:
        eolSrcLength = UNIX_EOL_LENGTH;

        eolSrcChars[0].firstByte = UNIX_EOL_FIRST_BYTE;
        eolSrcChars[0].secondByte = UNIX_EOL_SECOND_BYTE;
        break;

    default: //if NON
        break;
    }

    FileChar eolDestChars[EOL_MAX_CHARS];
    eolDestChars[0].isEOF = False;
    eolDestChars[1].isEOF = False;
    int eolDestLength = 0;

    switch (dest) {
    case WIN:
        eolDestLength = WIN_EOL_LENGTH;

        eolDestChars[0].firstByte = WIN_EOL_FIRST_CHAR_FIRST_BYTE;
        eolDestChars[0].secondByte = WIN_EOL_FIRST_CHAR_SECOND_BYTE;
        eolDestChars[1].firstByte = WIN_EOL_SECOND_CHAR_FIRST_BYTE;
        eolDestChars[1].secondByte = WIN_EOL_SECOND_CHAR_SECOND_BYTE;
        break;

    case MAC:
        eolDestLength = MAC_EOL_LENGTH;

        eolDestChars[0].firstByte = MAC_EOL_FIRST_BYTE;
        eolDestChars[0].secondByte = MAC_EOL_SECOND_BYTE;
        break;

    case UNIX:
        eolDestLength = UNIX_EOL_LENGTH;

        eolDestChars[0].firstByte = UNIX_EOL_FIRST_BYTE;
        eolDestChars[0].secondByte = UNIX_EOL_SECOND_BYTE;
        break;

    default: //if NON
        break;
    }

    int startCheck = 0;

    int endCheck = startCheck + eolSrcLength - 1;
    if(eolSrcLength == 0) {
         endCheck = startCheck;
    }

    while(endCheck < oldChars->length) {
        Bool isNeedConv = True;

        if(eolSrcLength == 0) {
            isNeedConv = False;
        }

        for(int index = 0; index < eolSrcLength; ++index) {

            if((oldChars->fCArr[startCheck + index].isEOF) ||
                (oldChars->fCArr[startCheck + index].firstByte
                 != eolSrcChars[index].firstByte) ||
                (oldChars->fCArr[startCheck + index].secondByte
                 != eolSrcChars[index].secondByte)) {

                    isNeedConv = False;
                    break;
                }
        }

        if(isNeedConv){

            for(int index = 0; index < eolDestLength; ++index) {
                newChars.fCArr[newChars.length] = eolDestChars[index];
                ++newChars.length;
            }

            startCheck += eolSrcLength;
            endCheck += eolSrcLength;
            continue;
        }

        newChars.fCArr[newChars.length] = oldChars->fCArr[startCheck];
        ++newChars.length;

        ++startCheck;
        ++endCheck;
    }

    for (int index = startCheck; index < oldChars->length; ++index)
    {
        newChars.fCArr[newChars.length] = oldChars->fCArr[startCheck];
        ++newChars.length;
    }

    return newChars;
}

OutFileCharArray convFileOsEOL(Os src, Os dest, FILE *in){
    static Bool isFileEndianWasCalculated = False;
    static Bool isSrcInBigEndian = False;
    static FileChar lastChar = {False, 0, 0};//default

    InFileCharArray inChars;
    inChars.length = 0;
    
    if(inChars.length != CHARS_ARRAY_MAX_LENGTH &&
        isStartCharOfWinLOF(&lastChar, isSrcInBigEndian)) {
            inChars.fCArr[inChars.length] = lastChar;
            ++inChars.length;
    }

    do{
        lastChar = readOneChar(in);

        if(lastChar.isEOF) {
            break;
        }

        if(inChars.length == CHARS_ARRAY_MAX_LENGTH - 1 &&
            isStartCharOfWinLOF(&lastChar, isSrcInBigEndian)) {
                break;
        }

        inChars.fCArr[inChars.length] = lastChar;
        ++inChars.length;
    }while (inChars.length != CHARS_ARRAY_MAX_LENGTH &&
        isStartCharOfWinLOF(&lastChar, isSrcInBigEndian));

    OutFileCharArray outChars;
    outChars.length = 0;

    if(!isFileEndianWasCalculated) {
        isSrcInBigEndian = isFileBigEndian(&lastChar);
        isFileEndianWasCalculated = True;
    }

    if(!isSrcInBigEndian) {
        for(int index = 0; index < inChars.length; ++index) {
            switchBytesInChar(&inChars.fCArr[index]);
        }
    }

    outChars = convertBigEndianOs(src, dest, &inChars);

    if(!isSrcInBigEndian) {
        for(int index = 0; index < outChars.length; ++index) {
            switchBytesInChar(&outChars.fCArr[index]);
        }
    }

    return outChars;
}

OutFileCharArray convFileChars(FILE *in, Commaned* com) {
    OutFileCharArray chars = convFileOsEOL(com->srcFileOs, com->destFileOs, in);

    if(!com->isSwap){
        return chars;
    }

    for(int index = 0; index < chars.length; ++index) {
        switchBytesInChar(&chars.fCArr[index]);
    }

    return chars;
}

void createFileFromFile(const char* inFile, const char* outFile,
        OutFileCharArray (*handleFileChars)(FILE *in, Commaned* com),Commaned* com){

    FILE *in = fopen(inFile,"rb");  // r for read, b for binary
    if(in == NULL) { //if src file doesn't exists.
        return;
    }

    FILE *out = fopen(outFile,"wb");  // w for write, b for binary

    for(;;){
        OutFileCharArray chars = handleFileChars(in, com);
        if(chars.length == NO_MORE_CHARS_TO_READ) {
            break;
        }

        for(int index = 0; index < chars.length; ++index){
            unsigned char outCharBuffer[] = {chars.fCArr[index].firstByte,
                                         chars.fCArr[index].secondByte};

            fwrite(outCharBuffer, sizeof(outCharBuffer), NUM_BUFFERS, out);
        }
    }

    fclose(in);
    fclose(out);
}

int main(int argc, char const *argv[])
{   
    int indexArg = 1;
    --argc;

    if(argc != 2 || argc != 4 || argc != 5) {
        return 1;
    }

    Commaned com = {NON, NON, False};

    const char* srcPath = argv[indexArg];
    ++indexArg;

    const char* destPath = argv[indexArg];
    ++indexArg;

    if(argc >= 4){
        if(strcmp(argv[indexArg], "-unix") == 0) {
            com.srcFileOs = UNIX;
        }else if(strcmp(argv[indexArg], "-mac") == 0){
            com.srcFileOs = MAC;
        }else if(strcmp(argv[indexArg], "-win") == 0) {
            com.srcFileOs = WIN;
        } else{
            return 1;
        }

        ++indexArg;

        if(strcmp(argv[indexArg], "-unix") == 0) {
            com.destFileOs = UNIX;
        }else if(strcmp(argv[indexArg], "-mac") == 0){
            com.destFileOs = MAC;
        }else if(strcmp(argv[indexArg], "-win") == 0) {
            com.destFileOs = WIN;
        } else{
            return 1;
        }

        ++indexArg;
    }
    
    if(argc == 5){
        //in default the -keep flag is used.
        if(strcmp(argv[indexArg], "-swap") == 0){
            com.isSwap = True;
        } else if(strcmp(argv[indexArg], "-keep") != 0){
            return 1;
        }
    }

    //creates the needed file
    createFileFromFile(srcPath, destPath, &convFileChars, &com);

    return 0;
}