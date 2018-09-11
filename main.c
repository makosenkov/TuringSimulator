#include "main.h"

void initStorage(struct storage* inputStorage) {

    inputStorage->alphabet = (char*)malloc(sizeof(char));
    inputStorage->alphabet[0] = '\0';
    inputStorage->alphabetSize = 0;
    inputStorage->currentHead[0] = NULL;
    inputStorage->currentHead[1] = NULL;
    inputStorage->tapeStart[0] = NULL;
    inputStorage->tapeStart[1] = NULL;
    inputStorage->tapeSize[0] = 0;
    inputStorage->headIndex[0] = 0;
    inputStorage->headIndex[1] = 0;
    inputStorage->statesNumber = 0;
    inputStorage->states = NULL;
    inputStorage->outputFile = NULL;

}

int main(int argc, char* argv[]) {

    struct storage* inputStorage = (struct storage*)malloc(sizeof(struct storage));
    initStorage(inputStorage);
    int printFlag = 0;
    int executionMode;
    parsing(inputStorage, argc, argv, &printFlag, &executionMode);
    simulation(inputStorage, printFlag, executionMode);
    if(inputStorage != NULL)
        free(inputStorage); //todo
    return 0;

}

