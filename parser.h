#ifndef TURINGSIMULATOR_PARSER_H
#define TURINGSIMULATOR_PARSER_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>

//положение головки
struct tapeSymbol {

    struct tapeSymbol* previousHead;
    struct tapeSymbol* nextHead;
    char symbol;

};

//один переход
struct transition {

    char writeSymbol;
    int writeTape;
    int writeState; //номер состояния
    int action; //L/R/H/S
    int flag;

};

struct stateTransitions {

    int tapeNumber;
    struct transition** transitions;

};

struct storage {

    char* alphabet;
    int alphabetSize;
    struct tapeSymbol* currentHead[2];
    struct tapeSymbol* tapeStart[2];
    int tapeSize[2];
    int headIndex[2];
    int statesNumber;
    struct stateTransitions** states; //двумерный массив указателей. количество переходов x алфавит
    FILE* outputFile;

};

void parsing(struct storage*, int, char* [], int*, int*);
int getSymbolIndex(char *, char);

#endif

