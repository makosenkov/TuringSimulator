#include "parser.h"

int repeatCheck(char* alphabet, char symbol, int alphabetSize) {

    for(int i = 0; i < alphabetSize; i++) {
        if(alphabet[i] == symbol)
            return 1;
    }
    return 0;

}

char* getLine(FILE* inputFile) {

    char* buffer = (char*)malloc(sizeof(char));
    int bufferSize = 0;
    buffer[bufferSize] = '\0';
    char symbol = getc(inputFile);
    if(feof(inputFile)) {
        printf("Error: invalid input file format (something missed)\n");
        exit(-1);
    }
    while(symbol != EOF && symbol != '\n') {
        if(symbol != '\r') {
            buffer[bufferSize] = symbol;
            bufferSize++;
            buffer = (char *) realloc(buffer, sizeof(char) * (bufferSize + 1));
            buffer[bufferSize] = '\0';
        }
        symbol = getc(inputFile);
    }
    return buffer;

}


int getSymbolIndex(char *alphabet, char symbol) {

    int i = 0;
    while(alphabet[i] != '\0') {
        if(alphabet[i] == symbol)
            return i;
        i++;
    }
    return -1;

}

void getAlphabet(struct storage* inputStorage, FILE* inputFile) {

    char* buffer = getLine(inputFile);
    int i = 0;
    while((buffer[i] != '\0') && (buffer[i] != '\r')) {
        if(buffer[i] <= ' ') {
            printf("Error: invalid alphabet symbol\n");
            exit(-1);
        }
        if(!repeatCheck(inputStorage->alphabet, buffer[i], inputStorage->alphabetSize)) {
            inputStorage->alphabetSize++;
            inputStorage->alphabet = (char*)realloc(inputStorage->alphabet, sizeof(char) * inputStorage->alphabetSize);
            inputStorage->alphabet[inputStorage->alphabetSize - 1] = buffer[i];
        }
        else {
            printf("Error: invalid alphabet format (repeating character)\n");
            exit(-1);
        }
        i++;
    }
    inputStorage->alphabetSize++;
    inputStorage->alphabet = (char*)realloc(inputStorage->alphabet, sizeof(char) * (inputStorage->alphabetSize + 1));
    inputStorage->alphabet[inputStorage->alphabetSize - 1] = ' ';
    inputStorage->alphabet[inputStorage->alphabetSize] = '\0';

}

void getTape(struct storage* inputStorage, FILE* inputFile, int tapeNumber) {

    char* buffer = getLine(inputFile);
    int headIndex = 0;
    int tapeSize = 0;
    while((buffer[tapeSize] != '\0') && (buffer[tapeSize] != '\r')) {
        if(buffer[tapeSize] == 'v')
            headIndex = tapeSize;
        else if(buffer[tapeSize] != '_') {
            printf("Error: invalid input file format (tape %d)\n", tapeNumber + 1);
            exit(-1);
        }
        tapeSize++;
    }
    buffer = getLine(inputFile);
    struct tapeSymbol* symbol = NULL;
    for(int i = 0; i < tapeSize; i++) {
        if(!repeatCheck(inputStorage->alphabet, buffer[i], inputStorage->alphabetSize)) {
            printf("Error: invalid symbol on the tape (tape %d)\n", tapeNumber + 1);
            exit(-1);
        }
        struct tapeSymbol* bufferTape = (struct tapeSymbol*)malloc(sizeof(struct tapeSymbol));
        bufferTape->previousHead = symbol;
        bufferTape->nextHead = NULL;
        bufferTape->symbol = buffer[i];
        if(symbol != NULL)
            symbol->nextHead = bufferTape;
        symbol = bufferTape;
        if(i == 0)
            inputStorage->tapeStart[tapeNumber] = symbol;
        if(i == headIndex) {
            inputStorage->currentHead[tapeNumber] = symbol;
            inputStorage->headIndex[tapeNumber] = headIndex;
            inputStorage->tapeSize[tapeNumber] = tapeSize;
        }
    }

}

void transitionsListParsing(struct storage* inputStorage, FILE* inputFile) {

    inputStorage->states = (struct stateTransitions **)malloc(inputStorage->statesNumber * sizeof(struct stateTransitions*));
    for(int i = 0; i < inputStorage->statesNumber; i++) {
        inputStorage->states[i] = (struct stateTransitions *)malloc(sizeof(struct stateTransitions));
        inputStorage->states[i]->tapeNumber = -1;
        inputStorage->states[i]->transitions = (struct transition **)malloc(inputStorage->alphabetSize * sizeof(struct transition*));
        for(int j = 0; j < inputStorage->alphabetSize; j++) {
            inputStorage->states[i]->transitions[j] = (struct transition *)malloc(inputStorage->alphabetSize * sizeof(struct transition));
            inputStorage->states[i]->transitions[j]->action = -1;
            inputStorage->states[i]->transitions[j]->flag = 0;
            inputStorage->states[i]->transitions[j]->writeState = -1;
            inputStorage->states[i]->transitions[j]->writeSymbol = '\0';
            inputStorage->states[i]->transitions[j]->writeTape = -1;
        }
    }
    int readState = 0, writeState = 0;
    char readSymbol, writeSymbol, action, readTape, writeTape;
    int scanfRet = 0;
    for(int i = 0; i < inputStorage->statesNumber * inputStorage->alphabetSize; i++) {
        scanfRet = fscanf(inputFile, "-%c-%c%d-%c-%c%d-%c\r\n", &readTape,
                          &readSymbol, &readState, &writeTape, &writeSymbol, &writeState, &action);
        if(scanfRet != 7) {
            printf("Error: invalid transitions list format (too few transitions)\n");
            exit(-1);
        }
        if(readState > inputStorage->statesNumber) {
            printf("Error: read state number out of bounds on %d line\n", i);
            exit(-1);
        }
        int readSymbolIndex = getSymbolIndex(inputStorage->alphabet, readSymbol);
        if(readSymbolIndex == -1) {
            printf("Error: invalid read symbol on %d line\n", i);
            exit(-1);
        }
        if(inputStorage->states[readState]->transitions[readSymbolIndex]->flag == 1) {
            printf("Error: state redefinition on %d line\n", i);
            exit(-1);
        }
        inputStorage->states[readState]->transitions[readSymbolIndex]->flag = 1;
        if(writeState > inputStorage->statesNumber) {
            printf("Error: write state number out of bounds on %d line\n", i);
            exit(-1);
        }
        inputStorage->states[readState]->transitions[readSymbolIndex]->writeState = writeState;
        if(!repeatCheck(inputStorage->alphabet, writeSymbol, inputStorage->alphabetSize)) {
            printf("Error: invalid write symbol on %d line\n", i);
            exit(-1);
        }
        inputStorage->states[readState]->transitions[readSymbolIndex]->writeSymbol = writeSymbol;
        if(readTape == 'O') {
            if(inputStorage->states[readState]->tapeNumber == -1)
                inputStorage->states[readState]->tapeNumber = 0;
            else if(inputStorage->states[readState]->tapeNumber != 0) {
                printf("Error: invalid read tape number on %d line\n", i);
                exit(-1);
            }
        }
        else if(readTape == 'T') {
            if(inputStorage->states[readState]->tapeNumber == -1)
                inputStorage->states[readState]->tapeNumber = 1;
            else if(inputStorage->states[readState]->tapeNumber != 1) {
                printf("Error: invalid read tape number on %d line\n", i);
                exit(-1);
            }
        }
        else {
            printf("Error: invalid read tape identifier on %d line\n", i);
            exit(-1);
        }
        if(writeTape == 'O')
            inputStorage->states[readState]->transitions[readSymbolIndex]->writeTape = 0;
        else if(writeTape == 'T')
            inputStorage->states[readState]->transitions[readSymbolIndex]->writeTape = 1;
        else {
            printf("Error: invalid write tape identifier on %d line\n", i);
            exit(-1);
        }
        if(action == 'L')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 0;
        else if(action == 'R')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 1;
        else if(action == 'H')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 2;
        else if(action == 'S')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 3;
        else {
            printf("Error: invalid action identifier on %d line\n", i);
            exit(-1);
        }
    }

}

void inputFileParsing(char* fileName, struct storage* inputStorage) {

    FILE* inputFile = fopen(fileName, "rb");
    if(inputFile == NULL) {
        printf("Error: can't open input file\n");
        exit(-1);
    }
    getAlphabet(inputStorage, inputFile);
    getTape(inputStorage, inputFile, 0);
    getTape(inputStorage, inputFile, 1);
    if(fscanf(inputFile, "%d\r\n", &(inputStorage->statesNumber)) != 1) {
        printf("Error: can't file number of states\n");
        exit(-1);
    }
    transitionsListParsing(inputStorage, inputFile);

}

void argsParsing(int argc, char* argv[], int* printFlag, int* executionMode) {

    if(argc == 5) {
        if(strcmp(argv[4], "-p")) {
            printf("Error: invalid fourth command line argument (must be \"-p\")\n");
            exit(-1);
        }
        *printFlag = 1;
    }
    if(!strcmp(argv[3], "-a"))
        *executionMode = 0;
    else if(!strcmp(argv[3], "-o"))
        *executionMode = 1;
    else {
        printf("Error: invalid third command line argument (must be \"-a\" or \"-o\")\n");
        exit(-1);
    }

}

void parsing(struct storage* inputStorage, int argc, char* argv[], int* printFlag, int* executionMode) {

    //[1] - input.txt, [2] - output.txt, [3] - -a/-o, [4] - [-p]
    char *inputFileName;
    char *outputFileName;
    argsParsing(argc, argv, printFlag, executionMode);
    inputFileName = argv[1];
    outputFileName = argv[2];
    inputStorage->outputFile = fopen(outputFileName, "wt");
    if (inputStorage->outputFile == NULL) {
        printf("Error: can't open output file\n");
        exit(-1);
    }
    inputFileParsing(inputFileName, inputStorage);
}
