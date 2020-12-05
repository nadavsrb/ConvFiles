// 213056153 Nadav Sharabi

#include <stdio.h>
#include <string.h>

// in utf-16 we have 2 bytes to represent char.
#define NUM_BYTES_CHAR 2

// in windows EOL is represent by 2 chars.
#define EOL_MAX_CHARS 2

// we would use a out buffer that get 2 chars each time.
#define CHARS_ARRAY_MAX_LENGTH 2

// we only read to one buffer
#define NUM_BUFFERS 1

// the length of the out buffer if got to EOF:
#define NO_MORE_CHARS_TO_READ 0

// the BOM bytes in big endian:
#define BOM_FIRST_BYTE_BIG_ENDIAN 0xFE
#define BOM_SECOND_BYTE_BIG_ENDIAN 0xFF

// order according to big endian:
#define WIN_EOL_FIRST_CHAR_FIRST_BYTE 0x00
#define WIN_EOL_FIRST_CHAR_SECOND_BYTE 0X0d
#define WIN_EOL_SECOND_CHAR_FIRST_BYTE 0x00
#define WIN_EOL_SECOND_CHAR_SECOND_BYTE 0x0a
#define MAC_EOL_FIRST_BYTE 0x00
#define MAC_EOL_SECOND_BYTE 0x0d
#define UNIX_EOL_FIRST_BYTE 0x00
#define UNIX_EOL_SECOND_BYTE 0x0a

// length means how many chars.
#define WIN_EOL_LENGTH 2
#define MAC_EOL_LENGTH 1
#define UNIX_EOL_LENGTH 1

/**
 * @brief for convenient:
 * enum for boolean vars.
 */
typedef enum { False, True } Bool;

/**
 * @brief enum to represent os,
 * NON - would use us as default,
 * if the os wasn't given.
 */
typedef enum { NON, WIN, MAC, UNIX } Os;

/**
 * @brief represent a commaned
 * from the terminal.
 */
typedef struct {
  Os srcFileOs;
  Os destFileOs;
  Bool isSwap;
} Commaned;

/**
 * @brief represent a utf-16 char.
 * (in utf-16 we have 2 bytes to represent char).
 */
typedef struct {
  Bool isEOF;
  unsigned char firstByte;
  unsigned char secondByte;
} FileChar;

/**
 * @brief is used to represent the in buffer.
 */
typedef struct {
  FileChar fCArr[CHARS_ARRAY_MAX_LENGTH];
  int length;
} InFileCharArray;

/**
 * @brief is used to represent the out buffer.
 */
typedef struct {
  // because every old char can become two new chars:
  FileChar fCArr[2 * CHARS_ARRAY_MAX_LENGTH];
  int length;
} OutFileCharArray;

/**
 * @brief this func gets a FileChar
 * that represents a char in utf-16
 * and swap his bytes.
 *
 * @param fc the char to swap his bytes.
 */
void switchBytesInChar(FileChar *fc) {
  // if it's not a char we should swap:
  if (fc == NULL || fc->isEOF) {
    return;
  }

  // saves the first byte.
  unsigned char firstByte = fc->firstByte;

  // swap the chars.
  fc->firstByte = fc->secondByte;
  fc->secondByte = firstByte;
};

/**
 * @brief this func read one char
 * from the file stream.
 *
 * @param in the file stream.
 * @return FileChar the char that was read.
 */
FileChar readOneChar(FILE *in) {
  // initialize the char:
  FileChar fc;
  fc.isEOF = False;

  // reads the char into a buffer.
  unsigned char inCharBuffer[NUM_BYTES_CHAR];
  int buffersRead = fread(inCharBuffer, sizeof(inCharBuffer), NUM_BUFFERS, in);

  // check if its EOF:
  if (buffersRead != NUM_BUFFERS) {
    fc.isEOF = True;
  }

  // we are taking the first and second byte we read and
  // put them in FileChar in the sequence we have read them.
  fc.firstByte = inCharBuffer[0];
  fc.secondByte = inCharBuffer[1];

  // returns the char we read.
  return fc;
}

/**
 * @brief this func gets the BOM char,
 * and tells if the file is in big endian.
 *
 * @param endianChar the BOM char.
 * @return Bool true if file is in big endian,
 * if file is in little endian false.
 */
Bool isFileBigEndian(FileChar *endianChar) {
  // checks if it's a real char:
  if (endianChar == NULL || endianChar->isEOF) {
    return False;
  }

  // checks if it's the big endian BOM char.
  if ((endianChar->firstByte == BOM_FIRST_BYTE_BIG_ENDIAN) &&
      (endianChar->secondByte == BOM_SECOND_BYTE_BIG_ENDIAN)) {
    return True;
  }

  // else, it's the little endian BOM char.
  return False;
}

/**
 * @brief this func checks if the char c
 * is the \r char.
 *
 * @param c the char to check.
 * @param isSrcInBigEndian is the char is in big endian.
 * @return Bool true if the char is \r, else false.
 */
Bool isStartCharOfWinLOF(const FileChar *c, Bool isSrcInBigEndian) {
  // checks this is really a char:
  if (c == NULL || c->isEOF) {
    return False;
  }

  // check in big endian if the char is \r:
  if (isSrcInBigEndian) {
    if (c->firstByte == WIN_EOL_FIRST_CHAR_FIRST_BYTE &&
        c->secondByte == WIN_EOL_FIRST_CHAR_SECOND_BYTE) {
      return True;
    }

    return False;
  }

  // check in little endian if the char is \r:
  if (c->secondByte == WIN_EOL_FIRST_CHAR_FIRST_BYTE &&
      c->firstByte == WIN_EOL_FIRST_CHAR_SECOND_BYTE) {
    return True;
  }

  return False;
}

/**
 * @brief this func get InFileCharArray
 * witch contains some chars that were read
 * from the src file, and converts the EOL chars
 * from the src os to the dest os.
 *
 * @param src the src file's os.
 * @param dest the dest file's os.
 * @param oldChars the chars from the src file
 * to convert (assuming that if a part of
 * EOL is in this buffer then all the
 * EOL is in this buffer).
 * @return OutFileCharArray returns the
 * converted chars.
 */
OutFileCharArray convertBigEndianOs(Os src, Os dest,
                                    const InFileCharArray *oldChars) {

  // initialize the out buffer.
  OutFileCharArray newChars;
  newChars.length = 0;

  // if there isn't in chars returns
  // an empty out buffer.
  if (oldChars == NULL) {
    return newChars;
  }

  // here we create and initialize the eolSrcChars
  // which will contains the src os's EOL.
  FileChar eolSrcChars[EOL_MAX_CHARS];
  eolSrcChars[0].isEOF = False;
  eolSrcChars[1].isEOF = False;
  int eolSrcLength = 0;

  // here we put the src os's EOL chars in eolSrcChars.
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

  default: // if NON, the src os's EOL doesn't matter.
    break;
  }

  // here we create and initialize the eolDestChars
  // which will contains the dest os's EOL.
  FileChar eolDestChars[EOL_MAX_CHARS];
  eolDestChars[0].isEOF = False;
  eolDestChars[1].isEOF = False;
  int eolDestLength = 0;

  // here we put the dest os's EOL chars in eolDestChars.
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

  default: // if NON, the dest os's EOL doesn't matter.
    break;
  }

  // here we initialize the pointers we will
  // go over the in buffer by. we will go over
  // the buffer with these pointers and check if the chars
  // between them are equals to the EOL src,
  // and will convert them if needed.
  int startCheck = 0;
  int endCheck = startCheck + eolSrcLength - 1;
  if (eolSrcLength == 0) { // if the src os's is NON (doesn't matter).
    endCheck = startCheck;
  }

  // we will go over the buffer with the pointers
  // and check if the chars between them are equals
  // to the EOL src, and will convert them if needed.
  while (endCheck < oldChars->length) {
    // first we assume we will need to convert.
    Bool isNeedConv = True;

    // if the src os's is NON than we don't need to convert.
    if (eolSrcLength == 0) {
      isNeedConv = False;
    }

    // we check if we really need to convert the chars between the pointers.
    for (int index = 0; index < eolSrcLength; ++index) {

      // here we check for each char between the pointers if
      // it equals to the char in the EOL src, if not
      // then we shouldN't convert those chars and we will do:
      // isNeedConv = False;
      if ((oldChars->fCArr[startCheck + index].isEOF) ||
          (oldChars->fCArr[startCheck + index].firstByte !=
           eolSrcChars[index].firstByte) ||
          (oldChars->fCArr[startCheck + index].secondByte !=
           eolSrcChars[index].secondByte)) {

        isNeedConv = False;
        break;
      }
    }

    // if we need to convert the chars between the pointers:
    if (isNeedConv) {
      // we will write to the output buffer the EOL of the dest os.
      for (int index = 0; index < eolDestLength; ++index) {
        newChars.fCArr[newChars.length] = eolDestChars[index];
        ++newChars.length;
      }

      // and continue to check the next chars.
      startCheck += eolSrcLength;
      endCheck += eolSrcLength;
      continue;
    }

    // if we don't need to convert these chars,
    // so we only can know that the first char should be
    // in the out buffer we will put him:
    newChars.fCArr[newChars.length] = oldChars->fCArr[startCheck];
    ++newChars.length;

    // we would check the next group of chars:
    ++startCheck;
    ++endCheck;
  }

  // here we add the last chars that can't be EOL, but
  // we didn't manage to put them into the out buffer:
  for (int index = startCheck; index < oldChars->length; ++index) {
    newChars.fCArr[newChars.length] = oldChars->fCArr[startCheck];
    ++newChars.length;
  }

  // we return the out buffer:
  return newChars;
}

/**
 * @brief this func returns an OutFileCharArray
 * witch constains some chars that were read from the src file, and
 * the EOL (=end of line) chars were already converted.
 * (we get some chars from the src files and return some
 * chars, so the EOL (=end of line chars) are converted
 * from the src Os to the dest OS)
 *
 * @param src the src OS.
 * @param dest the dest OS.
 * @param in the input file stream.
 * @return OutFileCharArray return some
 * chars (from src file but,), so the EOL
 * (=end of line chars) are converted
 * from the src Os to the dest OS.
 */
OutFileCharArray convFileOsEOL(Os src, Os dest, FILE *in) {
  // we would like to calculate the endianess of the file
  // only once by the BOM, witch should be the first char
  // we would read.
  static Bool isFileEndianWasCalculated = False;
  static Bool isSrcInBigEndian = False;

  // Some time as you would see we would read chars,
  // but the last char we wouldn't know what to do with
  // because we would need the next char to decide so what
  // we will do is to save the last char we read from src
  // in the lastChar and if it's a char we didn't know what to do
  // with it (so we didn't put it into the in buffer) we would
  // remember to add it to the next time we would read from
  // the src file. we start with default initialize that
  // don't means much.
  // (Notice! the only char that depends on the next char is \r
  // because if \n is after it then they both represent EOL together
  // and should be change accourding to the dest OS, so we would use
  // the lastChar only if it saves \r char if it was read to the end of
  // the buffer, and we would like to know what the next char is).
  static FileChar lastChar = {False, 0, 0};

  // we initialize the InFileCharArray (in buffer).
  InFileCharArray inChars;
  inChars.length = 0;

  // if there is a lastChar (\r) and there is enough place for
  // puting it in the buffer so it won't be the last char,
  // than we would add this char into the buffer.
  if (inChars.length < CHARS_ARRAY_MAX_LENGTH - 1 &&
      isStartCharOfWinLOF(&lastChar, isSrcInBigEndian)) {
    inChars.fCArr[inChars.length] = lastChar;
    ++inChars.length;
  }

  // here we would fill the in buffer withe src chars.
  do {
    // first we read a char from the src to the lastChar.
    lastChar = readOneChar(in);

    // if the endianess of the file wasn't calculated
    // yet so this is the first char - BOM, and we would,
    // calculate the endianess of the file.
    if (!isFileEndianWasCalculated) {
      isSrcInBigEndian = isFileBigEndian(&lastChar);
      isFileEndianWasCalculated = True;
    }

    // if the last char is EOF we don't have more char to read.
    if (lastChar.isEOF) {
      break;
    }

    // if we read the \r char but we don't have place to put
    // him and also the char after him so we won't put it
    // in the in buffer and stop reading.
    if (inChars.length >= CHARS_ARRAY_MAX_LENGTH - 1 &&
        isStartCharOfWinLOF(&lastChar, isSrcInBigEndian)) {
      break;
    }

    // else we would put the char into the in buffer.
    inChars.fCArr[inChars.length] = lastChar;
    ++inChars.length;

    // this loop should fill the in buffer or break by one of the condition in
    // it.
  } while (inChars.length != CHARS_ARRAY_MAX_LENGTH);

  // we initialize the OutFileCharArray (out buffer).
  OutFileCharArray outChars;
  outChars.length = 0;

  // if we are in little endian we would
  // switch the byte order to big indian for the func
  // convertBigEndianOs.
  if (!isSrcInBigEndian) {
    for (int index = 0; index < inChars.length; ++index) {
      switchBytesInChar(&inChars.fCArr[index]);
    }
  }

  // we convert the EOL chars according the os.
  outChars = convertBigEndianOs(src, dest, &inChars);

  // if we are in little endian we switched the byte order to
  // big indian for the func convertBigEndianOs.
  // so we would switch again the bytes order to return into
  // little endian.
  if (!isSrcInBigEndian) {
    for (int index = 0; index < outChars.length; ++index) {
      switchBytesInChar(&outChars.fCArr[index]);
    }
  }

  // we returns the out buffer.
  return outChars;
}

/**
 * @brief this func is used for
 * handleFileChars func to put in the
 * createFileFromFile func. this func
 * gets the src file's stream, and the
 * commaned to handle chars by,
 * every time this func is called it's
 * return an OutFileCharArray, so we read
 * some chars and handle them.
 *
 * @param in the input file stream.
 * @param com the commaned to handle chars by.
 * @return OutFileCharArray the out chars,
 * if there are no more chars returns an empty
 * OutFileCharArray (it's length is
 * NO_MORE_CHARS_TO_READ)
 */
OutFileCharArray convFileChars(FILE *in, Commaned *com) {
  // first we use the convFileOsEOLto to get an OutFileCharArray
  // witch constains some chars that were read from the src file, and
  // the EOL (=end of line) chars were already converted.
  OutFileCharArray chars = convFileOsEOL(com->srcFileOs, com->destFileOs, in);

  // if the commaned didn't had the -swap flag we done.
  if (!com->isSwap) {
    return chars;
  }

  // if the commaned had the -swap flag we will swap
  // each chars bytes in the OutFileCharArray:
  for (int index = 0; index < chars.length; ++index) {
    switchBytesInChar(&chars.fCArr[index]);
  }

  return chars;
}

/**
 * @brief Create a File From File.
 * this func use the handleFileChars
 * func (who knows what to do according
 * to the com) to get the chars for the
 * dest file. then writes the new chars
 * to the dest file (creates or overrides
 * the dest file).
 *
 * @param inFile the input file.
 * @param outFile the dest file.
 * @param handleFileChars the func to handle the bytes.
 * @param com the commaned to handle the bytes.
 */
void createFileFromFile(const char *inFile, const char *outFile,
                        OutFileCharArray (*handleFileChars)(FILE *in,
                                                            Commaned *com),
                        Commaned *com) {

  // first we open the src file & check it exists.
  FILE *in = fopen(inFile, "rb"); // r for read, b for binary.
  if (in == NULL) {               // if src file doesn't exists.
    return;
  }

  // open / creates the dest file.
  FILE *out = fopen(outFile, "wb"); // w for write, b for binary.

  // we would keep handle chars
  // until we won't have any left and we would
  // break from the func.
  for (;;) {

    // gets the next chars to write to the output file.
    OutFileCharArray chars = handleFileChars(in, com);

    // if there aren't more chars to write we should get
    // NO_MORE_CHARS_TO_READ, so we would need to break
    // from this loop.
    if (chars.length == NO_MORE_CHARS_TO_READ) {
      break;
    }

    // here we go char char get his bytes and write it
    // in to the dest file.
    for (int index = 0; index < chars.length; ++index) {

      // gets the bytes of the char.
      unsigned char outCharBuffer[] = {chars.fCArr[index].firstByte,
                                       chars.fCArr[index].secondByte};

      // writes the bytes of the char to the dest file.
      fwrite(outCharBuffer, sizeof(outCharBuffer), NUM_BUFFERS, out);
    }
  }

  // closing the files.
  fclose(in);
  fclose(out);
}

/**
 * @brief The main func, gets args
 * as required in ex2, and creates file as needed.
 *
 * @param argc num of args in input.
 * @param argv the input's args.
 * @return int if inputs valid return 0, else return 1.
 */
int start(int argc, char const *argv[]) {
  // Notice! the first arg from the terminal is
  // the .out file this info doesn't metter
  // to us so we initialize the args to not include
  // the first one.
  int indexArg = 1;
  --argc;

  // The num of args the program can get are:
  // 2: <source-file-name> <new-file-name>
  // 4: <source-file-name> <new-file-name>
  // <source-file-os-flag> <new-file-os-flag>
  // 5: <source-file-name> <new-file-name>
  // <source-file-os-flag> <new-file-os-flag>
  // <byte-order-flag>
  if (argc != 2 && argc != 4 && argc != 5) {
    return 1;
  }

  // We would refer all commaned as 6 input
  // and the optional inputs we would initialize
  // to the default.
  Commaned com = {NON, NON, False};

  // The first arg must be the src path
  // we would get it:
  const char *srcPath = argv[indexArg];
  ++indexArg;

  // The second arg must be the dest path
  // we would get it:
  const char *destPath = argv[indexArg];
  ++indexArg;

  // if we got more than 4 args the the 3rd
  // and 4th args must be:
  // <source-file-os-flag> <new-file-os-flag>
  if (argc >= 4) {

    // gets the 3rd arg: <source-file-os-flag>
    if (strcmp(argv[indexArg], "-unix") == 0) {
      com.srcFileOs = UNIX;
    } else if (strcmp(argv[indexArg], "-mac") == 0) {
      com.srcFileOs = MAC;
    } else if (strcmp(argv[indexArg], "-win") == 0) {
      com.srcFileOs = WIN;
    } else { // not valid input
      return 1;
    }

    ++indexArg;

    // gets the 4th arg: <new-file-os-flag>
    if (strcmp(argv[indexArg], "-unix") == 0) {
      com.destFileOs = UNIX;
    } else if (strcmp(argv[indexArg], "-mac") == 0) {
      com.destFileOs = MAC;
    } else if (strcmp(argv[indexArg], "-win") == 0) {
      com.destFileOs = WIN;
    } else { // not valid input
      return 1;
    }

    ++indexArg;
  }

  // if we got 5 args the 5th arg must be: <byte-order-flag>
  if (argc == 5) {
    // in default the -keep flag is used.
    if (strcmp(argv[indexArg], "-swap") == 0) {
      com.isSwap = True;
    } else if (strcmp(argv[indexArg], "-keep") != 0) { // not valid input
      return 1;
    }
  }

  // creates the needed file
  createFileFromFile(srcPath, destPath, &convFileChars, &com);

  return 0;
}