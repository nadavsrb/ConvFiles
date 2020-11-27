/*
 * C Program to Compare two Binary Files, Printing the First Byte 
 * Position where they Differ
 */
#include <stdio.h>
#include "ex2.c"

#define NUM_TESTS 1
#define MAX_ARGS 6

void compare_two_binary_files(const char *firstFile, const char *secondFile);
 
int main() {
    char const *argv[][MAX_ARGS]= {{"ex2.out", "testingFiles/win_input-utf-16.txt", "result/win_input-utf-16-copy.txt"}};
    int argc[] = {3};
    char const *result[] = {"testingFiles/win_input-utf-16.txt"};

    for(int i = 0; i < NUM_TESTS; ++i){
        start(argc[i], argv[i]);
        compare_two_binary_files(argv[i][2],result[i]);
    }

    return 0;
}
 
/*
 * compare two binary files character by character
 */
void compare_two_binary_files(const char *firstFile, const char *secondFile)
{   

    FILE *fp1, *fp2;

    fp1 = fopen(firstFile,  "r");
    if (fp1 == NULL)
    {
        printf("\nError in opening file %s", firstFile);
        return;
    }
 
    fp2 = fopen(secondFile, "r");
 
    if (fp2 == NULL)
    {
        printf("\nError in opening file %s", secondFile);
        return;
    }

    unsigned long pos;
    int c1, c2;
    for (pos = 0;; pos++) {
        c1 = getc(fp1);
        c2 = getc(fp2);
        if (c1 != c2 || c1 == EOF)
            break;
    }
    if (c1 == c2) {
        printf("Files: %s , %s\n", firstFile, secondFile);
        printf("files are identical and have %lu bytes\n\n", pos);
    } else
    if (c1 == EOF) {
        printf("Files: %s , %s\n", firstFile, secondFile);
        printf("file1 is included in file2, the first %lu bytes are identical\n\n", pos);

    } else
    if (c2 == EOF) {
        printf("Files: %s , %s\n", firstFile, secondFile);
        printf("file2 is included in file1, the first %lu bytes are identical\n\n", pos);

    } else {
        printf("Files: %s , %s\n", firstFile, secondFile);
        printf("file1 and file2 differ at position %lu: 0x%02X <> 0x%02X\n\n", pos, c1, c2);

    }

}