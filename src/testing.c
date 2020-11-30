/*
 * C Program to Compare two Binary Files, Printing the First Byte 
 * Position where they Differ
 */
#include <stdio.h>
#include "ex2.c"

#define NUM_TESTS 54
#define MAX_ARGS 6

void compare_two_binary_files(const char *firstFile, const char *secondFile);
 
int main() {
    char const *argv[][MAX_ARGS]= {
    {"ex2.out", "testingFiles/win_input-utf-16.txt","result/win/win_input-utf-16_keep-copy.txt", "-win", "-win", "-keep"},
    {"ex2.out", "testingFiles/win_input-utf-16.txt","result/win/win_input-utf-16_swap-copy.txt", "-win", "-win", "-swap"},
    {"ex2.out", "testingFiles/win_input-utf-16.txt","result/win/mac_input-utf-16_keep-copy.txt", "-win", "-mac", "-keep"},
    {"ex2.out", "testingFiles/win_input-utf-16.txt","result/win/mac_input-utf-16_swap-copy.txt", "-win", "-mac", "-swap"},
    {"ex2.out", "testingFiles/win_input-utf-16.txt","result/win/unix_input-utf-16_keep-copy.txt", "-win", "-unix", "-keep"},
    {"ex2.out", "testingFiles/win_input-utf-16.txt","result/win/unix_input-utf-16_swap-copy.txt", "-win", "-unix", "-swap"},
    {"ex2.out", "testingFiles/mac_input-utf-16.txt","result/mac/win_input-utf-16_keep-copy.txt", "-mac", "-win", "-keep"},
    {"ex2.out", "testingFiles/mac_input-utf-16.txt","result/mac/win_input-utf-16_swap-copy.txt", "-mac", "-win", "-swap"},
    {"ex2.out", "testingFiles/mac_input-utf-16.txt","result/mac/mac_input-utf-16_keep-copy.txt", "-mac", "-mac", "-keep"},
    {"ex2.out", "testingFiles/mac_input-utf-16.txt","result/mac/mac_input-utf-16_swap-copy.txt", "-mac", "-mac", "-swap"},
    {"ex2.out", "testingFiles/mac_input-utf-16.txt","result/mac/unix_input-utf-16_keep-copy.txt", "-mac", "-unix", "-keep"},
    {"ex2.out", "testingFiles/mac_input-utf-16.txt","result/mac/unix_input-utf-16_swap-copy.txt", "-mac", "-unix", "-swap"},
    {"ex2.out", "testingFiles/unix_input-utf-16.txt","result/unix/win_input-utf-16_keep-copy.txt", "-unix", "-win", "-keep"},
    {"ex2.out", "testingFiles/unix_input-utf-16.txt","result/unix/win_input-utf-16_swap-copy.txt", "-unix", "-win", "-swap"},
    {"ex2.out", "testingFiles/unix_input-utf-16.txt","result/unix/mac_input-utf-16_keep-copy.txt", "-unix", "-mac", "-keep"},
    {"ex2.out", "testingFiles/unix_input-utf-16.txt","result/unix/mac_input-utf-16_swap-copy.txt", "-unix", "-mac", "-swap"},
    {"ex2.out", "testingFiles/unix_input-utf-16.txt","result/unix/unix_input-utf-16_keep-copy.txt", "-unix", "-unix", "-keep"},
    {"ex2.out", "testingFiles/unix_input-utf-16.txt","result/unix/unix_input-utf-16_swap-copy.txt", "-unix", "-unix", "-swap"},
    {"ex2.out", "myTests/win_t1_keep.txt", "result/t1/win/win_t1_keep-copy.txt", "-win", "-win", "-keep"},
    {"ex2.out", "myTests/win_t1_keep.txt", "result/t1/win/win_t1_swap-copy.txt", "-win", "-win", "-swap"},
    {"ex2.out", "myTests/win_t1_keep.txt", "result/t1/win/mac_t1_keep-copy.txt", "-win", "-mac", "-keep"},
    {"ex2.out", "myTests/win_t1_keep.txt", "result/t1/win/mac_t1_swap-copy.txt", "-win", "-mac", "-swap"},
    {"ex2.out", "myTests/win_t1_keep.txt", "result/t1/win/unix_t1_keep-copy.txt", "-win", "-unix", "-keep"},
    {"ex2.out", "myTests/win_t1_keep.txt", "result/t1/win/unix_t1_swap-copy.txt", "-win", "-unix", "-swap"},
    {"ex2.out", "myTests/mac_t1_keep.txt", "result/t1/mac/win_t1_keep-copy.txt", "-mac", "-win", "-keep"},
    {"ex2.out", "myTests/mac_t1_keep.txt", "result/t1/mac/win_t1_swap-copy.txt", "-mac", "-win", "-swap"},
    {"ex2.out", "myTests/mac_t1_keep.txt", "result/t1/mac/mac_t1_keep-copy.txt", "-mac", "-mac", "-keep"},
    {"ex2.out", "myTests/mac_t1_keep.txt", "result/t1/mac/mac_t1_swap-copy.txt", "-mac", "-mac", "-swap"},
    {"ex2.out", "myTests/mac_t1_keep.txt", "result/t1/mac/unix_t1_keep-copy.txt", "-mac", "-unix", "-keep"},
    {"ex2.out", "myTests/mac_t1_keep.txt", "result/t1/mac/unix_t1_swap-copy.txt", "-mac", "-unix", "-swap"},
    {"ex2.out", "myTests/unix_t1_keep.txt", "result/t1/unix/win_t1_keep-copy.txt", "-unix", "-win", "-keep"},
    {"ex2.out", "myTests/unix_t1_keep.txt", "result/t1/unix/win_t1_swap-copy.txt", "-unix", "-win", "-swap"},
    {"ex2.out", "myTests/unix_t1_keep.txt", "result/t1/unix/mac_t1_keep-copy.txt", "-unix", "-mac", "-keep"},
    {"ex2.out", "myTests/unix_t1_keep.txt", "result/t1/unix/mac_t1_swap-copy.txt", "-unix", "-mac", "-swap"},
    {"ex2.out", "myTests/unix_t1_keep.txt", "result/t1/unix/unix_t1_keep-copy.txt", "-unix", "-unix", "-keep"},
    {"ex2.out", "myTests/unix_t1_keep.txt", "result/t1/unix/unix_t1_swap-copy.txt", "-unix", "-unix", "-swap"},
    {"ex2.out", "myTests/win_t2_keep.txt", "result/t2/win/win_t2_keep-copy.txt", "-win", "-win", "-keep"},
    {"ex2.out", "myTests/win_t2_keep.txt", "result/t2/win/win_t2_swap-copy.txt", "-win", "-win", "-swap"},
    {"ex2.out", "myTests/win_t2_keep.txt", "result/t2/win/mac_t2_keep-copy.txt", "-win", "-mac", "-keep"},
    {"ex2.out", "myTests/win_t2_keep.txt", "result/t2/win/mac_t2_swap-copy.txt", "-win", "-mac", "-swap"},
    {"ex2.out", "myTests/win_t2_keep.txt", "result/t2/win/unix_t2_keep-copy.txt", "-win", "-unix", "-keep"},
    {"ex2.out", "myTests/win_t2_keep.txt", "result/t2/win/unix_t2_swap-copy.txt", "-win", "-unix", "-swap"},
    {"ex2.out", "myTests/mac_t2_keep.txt", "result/t2/mac/win_t2_keep-copy.txt", "-mac", "-win", "-keep"},
    {"ex2.out", "myTests/mac_t2_keep.txt", "result/t2/mac/win_t2_swap-copy.txt", "-mac", "-win", "-swap"},
    {"ex2.out", "myTests/mac_t2_keep.txt", "result/t2/mac/mac_t2_keep-copy.txt", "-mac", "-mac", "-keep"},
    {"ex2.out", "myTests/mac_t2_keep.txt", "result/t2/mac/mac_t2_swap-copy.txt", "-mac", "-mac", "-swap"},
    {"ex2.out", "myTests/mac_t2_keep.txt", "result/t2/mac/unix_t2_keep-copy.txt", "-mac", "-unix", "-keep"},
    {"ex2.out", "myTests/mac_t2_keep.txt", "result/t2/mac/unix_t2_swap-copy.txt", "-mac", "-unix", "-swap"},
    {"ex2.out", "myTests/unix_t2_keep.txt", "result/t2/unix/win_t2_keep-copy.txt", "-unix", "-win", "-keep"},
    {"ex2.out", "myTests/unix_t2_keep.txt", "result/t2/unix/win_t2_swap-copy.txt", "-unix", "-win", "-swap"},
    {"ex2.out", "myTests/unix_t2_keep.txt", "result/t2/unix/mac_t2_keep-copy.txt", "-unix", "-mac", "-keep"},
    {"ex2.out", "myTests/unix_t2_keep.txt", "result/t2/unix/mac_t2_swap-copy.txt", "-unix", "-mac", "-swap"},
    {"ex2.out", "myTests/unix_t2_keep.txt", "result/t2/unix/unix_t2_keep-copy.txt", "-unix", "-unix", "-keep"},
    {"ex2.out", "myTests/unix_t2_keep.txt", "result/t2/unix/unix_t2_swap-copy.txt", "-unix", "-unix", "-swap"}
    };

    int argc[] = {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};
    char const *result[] = {
    "testingFiles/win_input-utf-16_keep.txt",
    "testingFiles/win_input-utf-16_swap.txt",
    "testingFiles/mac_input-utf-16_keep.txt",
    "testingFiles/mac_input-utf-16_swap.txt",
    "testingFiles/unix_input-utf-16_keep.txt",
    "testingFiles/unix_input-utf-16_swap.txt",
    "testingFiles/win_input-utf-16_keep.txt",
    "testingFiles/win_input-utf-16_swap.txt",
    "testingFiles/mac_input-utf-16_keep.txt",
    "testingFiles/mac_input-utf-16_swap.txt",
    "testingFiles/unix_input-utf-16_keep.txt",
    "testingFiles/unix_input-utf-16_swap.txt",
    "testingFiles/win_input-utf-16_keep.txt",
    "testingFiles/win_input-utf-16_swap.txt",
    "testingFiles/mac_input-utf-16_keep.txt",
    "testingFiles/mac_input-utf-16_swap.txt",
    "testingFiles/unix_input-utf-16_keep.txt",
    "testingFiles/unix_input-utf-16_swap.txt",
    "myTests/win_t1_keep.txt",
    "myTests/win_t1_swap.txt",
    "myTests/mac_t1_keep.txt",
    "myTests/mac_t1_swap.txt",
    "myTests/unix_t1_keep.txt",
    "myTests/unix_t1_swap.txt",
    "myTests/win_t1_keep.txt",
    "myTests/win_t1_swap.txt",
    "myTests/mac_t1_keep.txt",
    "myTests/mac_t1_swap.txt",
    "myTests/unix_t1_keep.txt",
    "myTests/unix_t1_swap.txt",
    "myTests/win_t1_keep.txt",
    "myTests/win_t1_swap.txt",
    "myTests/mac_t1_keep.txt",
    "myTests/mac_t1_swap.txt",
    "myTests/unix_t1_keep.txt",
    "myTests/unix_t1_swap.txt",
    "myTests/win_t2_keep.txt",
    "myTests/win_t2_swap.txt",
    "myTests/mac_t2_keep.txt",
    "myTests/mac_t2_swap.txt",
    "myTests/unix_t2_keep.txt",
    "myTests/unix_t2_swap.txt",
    "myTests/win_t2_keep.txt",
    "myTests/win_t2_swap.txt",
    "myTests/mac_t2_keep.txt",
    "myTests/mac_t2_swap.txt",
    "myTests/unix_t2_keep.txt",
    "myTests/unix_t2_swap.txt",
    "myTests/win_t2_keep.txt",
    "myTests/win_t2_swap.txt",
    "myTests/mac_t2_keep.txt",
    "myTests/mac_t2_swap.txt",
    "myTests/unix_t2_keep.txt",
    "myTests/unix_t2_swap.txt"
    };

    for(int i = 0; i < NUM_TESTS; ++i){
        start(argc[i], argv[i]);
        compare_two_binary_files(argv[i][2],result[i]);
    }

    printf("done!\n");
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
        // printf("Files: %s , %s\n", firstFile, secondFile);
        // printf("files are identical and have %lu bytes\n\n", pos);
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