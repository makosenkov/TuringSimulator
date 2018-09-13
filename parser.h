#ifndef TURINGSIMULATOR_PARSER_H
#define TURINGSIMULATOR_PARSER_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>

//символ на ленте
struct tapeSymbol {

    struct tapeSymbol* previousHead; //указатель на предыдущий символ
    struct tapeSymbol* nextHead; //указатель на следующий символ
    char symbol; //сам символ

};

//один переход
struct transition {

    char writeSymbol; //символ, который мы пишем
    int writeTape; //лента, на которую пишем
    int writeState; //номер состояния, в которое переходим
    int action; //L/R/H/S
    int flag; //для инициализации состояния

};

struct stateTransitions {

    int tapeNumber; //номер ленты
    struct transition** transitions; //массив переходов

};

struct storage {

    char* alphabet; //алфавит
    int alphabetSize; //размер алфавита
    struct tapeSymbol* currentHead[2]; //элементы под головками
    struct tapeSymbol* tapeStart[2]; //начало лент
    int tapeSize[2]; // текущие размеры лент
    int headIndex[2]; //положение головок
    int statesNumber; //количество состояний
    struct stateTransitions** states; //массив состояний
    FILE* outputFile; //указатель на файл вывода

};

void parsing(struct storage*, int, char* [], int*, int*);
int getSymbolIndex(char *, char);

#endif

