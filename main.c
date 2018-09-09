#include <stdio.h>
#include "input.h"

int main (int argc, char *argv[]){
    FILE *inputOne;
    FILE *inputTwo;
    FILE *outFile;

    if ((argc == 5) && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0) ||
        ((argc == 6) && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0) && (strcmp(argv[5], "-p") == 0))) {

        inputOne = fopen(argv[1], "rt");

        if (inputOne == NULL) {
            printf("error 100: Can not find first input file\n");
            exit(100);
        }

        inputTwo = fopen(argv[2], "rt");
        if (inputTwo == NULL) {
            printf("error 109: Can not find second input file\n");
            exit(109);
        }

        outFile = fopen(argv[3], "wr");
        if (outFile == NULL) {
            printf("Can not find output file\n");
        }
    }

    if (argc != 5) {
        if (argc != 6 || (argc == 6 && (strcmp(argv[5], "-p") != 0))) {
            printf("error 101: Wrong number of arguments\n");
            exit(101);
        }
    }

    input(inputOne, inputTwo, outFile, argc, argv);

    exit(0);
}

