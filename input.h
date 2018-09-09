#ifndef TURINGSIMULATOR_INPUT_H
#define TURINGSIMULATOR_INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct Pair{
    char first;
    char second;
};

struct ArrayPair{
    char *first;
    char *second;
};


struct Command {
    struct Pair newChar;
    int newStateNum;
    struct Pair move;
};

int arrayContainsPair(struct Pair symbols, struct Pair *arr, int maxSize);

int arrayContainsInt(int symbol, int *arr, int maxSize);

int input (FILE *inputOne, FILE *inputTwo, FILE *outFile, int argc, char *argv[]);

int * step(int firstHeadState, int secondHeadState, int lastStateNum, struct ArrayPair tape, struct Pair *symbols, int *states, struct Command **arr, int maxArraySize, int maxTapeSize,
           int quit, FILE *output, _Bool flag);

void printTape(char *tape, int maxTapeSize, FILE *output);

void printHead(int headState, int maxTapeSize, FILE *output);

void printCommand(struct Command command, FILE *output);

void printCommands(struct Command **arr, int maxSize, FILE *output);

#endif //TURINGSIMULATOR_INPUT_H
