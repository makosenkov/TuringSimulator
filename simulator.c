#include "simulator.h"
#include <stdlib.h>

void tapesPrint(struct storage* inputStorage, struct transition* currentTrans, char readChar, int tape) {

    printf("Transition: \n");
    printf("Read tape: %d\n Read symbol: %c; Write tape number: %d; Write symbol: %c; Next state: %d; ",
           tape, readChar, currentTrans->writeTape + 1, currentTrans->writeSymbol,
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
    printf("First tape state: \n");
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
    printf("Second tape state: \n");
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
    printf("\n");

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

    if(printFlag == 1) {
        inputPrint(inputStorage);
    }
    printf("\n\nSimulation begins...\n");
    int stateNumber = 0;
    while(stateNumber != -1) {
        struct stateTransitions *state = inputStorage->states[stateNumber];
        int currentTape = state->tapeNumber;
        char currentSymbol = inputStorage->currentHead[currentTape]->symbol;
        struct transition *currentTransition = state->transitions[getSymbolIndex(inputStorage->alphabet,
                                                                                 currentSymbol)];
        inputStorage->currentHead[currentTransition->writeTape]->symbol = currentTransition->writeSymbol;
        stateNumber = currentTransition->writeState;
        switch (currentTransition->action) {
            case 0:
                if (inputStorage->currentHead[currentTape]->previousHead == NULL) {
                    struct tapeSymbol *newSymbol = (struct tapeSymbol *) malloc(sizeof(struct tapeSymbol));
                    newSymbol->symbol = ' ';
                    newSymbol->previousHead = NULL;
                    newSymbol->nextHead = inputStorage->currentHead[currentTape];
                    inputStorage->currentHead[currentTape]->previousHead = newSymbol;
                    inputStorage->tapeSize[currentTape]++;
                    inputStorage->headIndex[currentTape]++;
                }
                inputStorage->currentHead[currentTape] = inputStorage->currentHead[currentTape]->previousHead;
                inputStorage->tapeStart[currentTape] = inputStorage->currentHead[currentTape];
                inputStorage->headIndex[currentTape]--;
                break;
            case 1:
                if (inputStorage->currentHead[currentTape]->nextHead == NULL) {
                    struct tapeSymbol *newSymbol = (struct tapeSymbol *) malloc(sizeof(struct tapeSymbol));
                    newSymbol->symbol = ' ';
                    newSymbol->previousHead = inputStorage->currentHead[currentTape];
                    newSymbol->nextHead = NULL;
                    inputStorage->currentHead[currentTape]->nextHead = newSymbol;
                    inputStorage->tapeSize[currentTape]++;
                }
                inputStorage->currentHead[currentTape] = inputStorage->currentHead[currentTape]->nextHead;
                inputStorage->headIndex[currentTape]++;
                break;
            case 3:
                printf("Program ended successfully. \n");
                stateNumber = -1;
                break;
            default:
                break;
        }
        if(executionMode == 1) {
            char buf = getchar();
            if(buf == 'f')
                executionMode = 0;
            else if(buf == 'b') {
                printf("Program was terminated.\n");
                exit(0);
            }
            else if(buf != 's') {
                printf("Error: invalid input symbol!\n");
                exit(-1);
            }
            getchar(); //для \n?
        }
        tapesPrint(inputStorage, currentTransition, currentSymbol, currentTape);
    }

}


