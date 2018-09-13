#include "simulator.h"
#include <stdlib.h>

void tapesPrint(struct storage* inputStorage, struct transition* currentTrans, char readChar, int tape, int state) {

    printf("Transition: \nState number : %d\n", state);
    fprintf (inputStorage->outputFile, "Transition: \nState number : %d\n", state);
    printf("Read tape: %d\n Read symbol: %c; Write tape number: %d; Write symbol: %c; Next state: %d; ",
           tape, readChar, currentTrans->writeTape + 1, currentTrans->writeSymbol,
           currentTrans->writeState);
    fprintf (inputStorage->outputFile, "Read tape: %d\n Read symbol: %c; Write tape number: %d; Write symbol: %c; Next state: %d; ",
             tape, readChar, currentTrans->writeTape + 1, currentTrans->writeSymbol,
             currentTrans->writeState);
    switch(currentTrans->action) {
        case 0:
            printf("Move: L\n");
            fprintf (inputStorage->outputFile, "Move: L\n");
            break;
        case 1:
            printf("Move: R\n");
            fprintf (inputStorage->outputFile, "Move: R\n");
            break;
        case 2:
            printf("Move: H\n");
            fprintf (inputStorage->outputFile, "Move: H\n");
            break;
        case 3:
            printf("Move: S\n");
            fprintf (inputStorage->outputFile, "Move: S\n");
            break;
        default:
            break;
    }
    printf("First tape state: \n");
    fprintf (inputStorage->outputFile, "First tape state: \n");
    for(int i = 0; i < inputStorage->tapeSize[0]; i++) {
        if(i == inputStorage->headIndex[0]) {
            printf("v");
            fprintf(inputStorage->outputFile, "v");
        }
        else {
            printf("_");
            fprintf(inputStorage->outputFile, "_");
        }
    }
    printf("\n");
    fprintf(inputStorage->outputFile, "\n");
    struct tapeSymbol* currentSymbol = inputStorage->tapeStart[0];
    while(currentSymbol != NULL) {
        printf("%c", currentSymbol->symbol);
        fprintf(inputStorage->outputFile, "%c", currentSymbol->symbol);
        currentSymbol = currentSymbol->nextHead;
    }
    printf("\nSecond tape state: \n");
    fprintf(inputStorage->outputFile, "\nSecond tape state: \n");
    for(int i = 0; i < inputStorage->tapeSize[1]; i++) {
        if(i == inputStorage->headIndex[1]) {
            printf("v");
            fprintf(inputStorage->outputFile, "v");
        }
        else {
            printf("_");
            fprintf(inputStorage->outputFile, "_");
        }
    }
    printf("\n");
    fprintf(inputStorage->outputFile, "\n");
    currentSymbol = inputStorage->tapeStart[1];
    while(currentSymbol != NULL) {
        printf("%c", currentSymbol->symbol);
        fprintf(inputStorage->outputFile, "%c", currentSymbol->symbol);
        currentSymbol = currentSymbol->nextHead;
    }
    printf("\n");
    fprintf(inputStorage->outputFile, "\n");

}

void inputPrint(struct storage* inputStorage) {

    printf("Alphabet: %s\n", inputStorage->alphabet);
    printf("Initial first tape state: \n");
    for(int i = 0; i < inputStorage->tapeSize[0]; i++) {
        if(i == inputStorage->headIndex[0])
            printf("v");
        else
            printf("_");
    }
    printf("\n");
    struct tapeSymbol* currentSymbol = inputStorage->tapeStart[0];
    while(currentSymbol != NULL) {
        printf("%c", currentSymbol->symbol);
        currentSymbol = currentSymbol->nextHead;
    }
    printf("\n");
    printf("Initial second tape state: \n");
    for(int i = 0; i < inputStorage->tapeSize[1]; i++) {
        if(i == inputStorage->headIndex[1])
            printf("v");
        else
            printf("_");
    }
    printf("\n");
    currentSymbol = inputStorage->tapeStart[1];
    while(currentSymbol != NULL) {
        printf("%c", currentSymbol->symbol);
        currentSymbol = currentSymbol->nextHead;
    }
    printf("\nTransitions list:\n");
    for(int i = 0; i < inputStorage->statesNumber; i++) {
        printf("State %d:\nState tape number: %d\n", i, inputStorage->states[i]->tapeNumber + 1);
        for(int j = 0; j < inputStorage->alphabetSize; j++) {
            struct transition* currentTrans = inputStorage->states[i]->transitions[j];
            printf("Read symbol: %c; Write tape number: %d; Write symbol: %c; Next state: %d; ",
                   inputStorage->alphabet[j], currentTrans->writeTape + 1, currentTrans->writeSymbol,
                   currentTrans->writeState);
            switch(currentTrans->action) {
                case 0:
                    printf("Move: L\n");
                    break;
                case 1:
                    printf("Move: R\n");
                    break;
                case 2:
                    printf("Move: H\n");
                    break;
                case 3:
                    printf("Move: S\n");
                    break;
                default:
                    break;
            }
        }
    }

}

void simulation(struct storage* inputStorage, int printFlag, int executionMode) {

    if(printFlag == 1) { //если был флаг -p
        inputPrint(inputStorage); //печатаем входные штуки в консоль
    }
    printf("\n\nSimulation begins...\n");
    fprintf(inputStorage->outputFile, "Simulation begins...\n");
    int stateNumber = 0; //текущее состояние
    int endFlag = 0;
    while(endFlag == 0) {
        struct stateTransitions *state = inputStorage->states[stateNumber]; //берем указатель на структуру текущего состояния
        int currentTape = state->tapeNumber; //берем номер ленты, с которой считываем
        char currentSymbol = inputStorage->currentHead[currentTape]->symbol; //берем символ под головкой
        struct transition *currentTransition = state->transitions[getSymbolIndex(inputStorage->alphabet,
                                                                                 currentSymbol)]; //берем переход для этого состояния
        inputStorage->currentHead[currentTransition->writeTape]->symbol = currentTransition->writeSymbol; //переписываем символ
        stateNumber = currentTransition->writeState; //меняем состояние
        switch (currentTransition->action) {
            case 0: //сдвиг влево
                //если нет предыдущего (расширяем ленту влево)
                if (inputStorage->currentHead[currentTape]->previousHead == NULL) {
                    struct tapeSymbol *newSymbol = (struct tapeSymbol *) malloc(sizeof(struct tapeSymbol)); //добавляем память еще на один символ
                    if (newSymbol == NULL) {
                        printf("Memory allocation error\n");
                        exit(-1);
                    }
                    newSymbol->symbol = ' '; //пишем пустой символ
                    newSymbol->previousHead = NULL; //обнуляем предыдущий
                    newSymbol->nextHead = inputStorage->currentHead[currentTape]; //делаем текущий символ следующим для нового
                    inputStorage->currentHead[currentTape]->previousHead = newSymbol; //новый символ предыдущим для текущего
                    inputStorage->tapeStart[currentTape] = newSymbol; //новое начало ленты
                    inputStorage->tapeSize[currentTape]++;
                    inputStorage->headIndex[currentTape]++;
                }
                //сдвигаем на предыдущий
                inputStorage->currentHead[currentTape] = inputStorage->currentHead[currentTape]->previousHead;
                inputStorage->headIndex[currentTape]--;
                break;
            case 1: //сдвиг вправо
                //если нет следующего (расщширяем ленту вправо)
                if (inputStorage->currentHead[currentTape]->nextHead == NULL) {
                    struct tapeSymbol *newSymbol = (struct tapeSymbol *) malloc(sizeof(struct tapeSymbol));
                    if (newSymbol == NULL) {
                        printf("Memory allocation error\n");
                        exit(-1);
                    }
                    newSymbol->symbol = ' ';
                    newSymbol->previousHead = inputStorage->currentHead[currentTape]; //делаем предыдущий текущим
                    newSymbol->nextHead = NULL; //следующий для нового - нулл
                    inputStorage->currentHead[currentTape]->nextHead = newSymbol; //следующий для текущего - новый
                    inputStorage->tapeSize[currentTape]++;
                }
                //сдвигаем на следующий
                inputStorage->currentHead[currentTape] = inputStorage->currentHead[currentTape]->nextHead;
                inputStorage->headIndex[currentTape]++;
                break;

            //case 2 не трогаем

            case 3:
                endFlag = 1;
                break;
            default:
                break;
        }

        //режим отладки
        if(executionMode == 1) {
            char buf = getchar(); //читаем символ
            if(buf == 'f')
                executionMode = 0; //отключение отладки
            else if(buf == 'b') {
                printf("Program was terminated.\n"); //остановка программы
                exit(0);
            }
            else if(buf != 's') {
                printf("Error: invalid input symbol!\n"); //проверка на дичь
                exit(-1);
            }
            getchar(); //для \n
        }
        tapesPrint(inputStorage, currentTransition, currentSymbol, currentTape, stateNumber); //печать текущего состояния лент
        if (endFlag != 0) {
            printf("Program ended successfully. \n");
            fprintf(inputStorage->outputFile, "Program ended successfully. \n");
        }
    }

}


