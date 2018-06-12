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

#endif //TURINGSIMULATOR_INPUT_H
