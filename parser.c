#include "parser.h"
#include <stdlib.h>

int repeatCheck(char *alphabet, char symbol, int alphabetSize) {

    //Проверяем, есть ли уже такой символ в алфавите
    for (int i = 0; i < alphabetSize; i++) {
        if (alphabet[i] == symbol)
            return 1;
    }
    return 0;

}

char *getLine(FILE *inputFile, struct storage* inputStorage) {

    char *buffer = (char *) malloc(sizeof(char)); //сюда считываем строку из файла
    if (buffer == NULL) {
        printf("Memory allocation error\n");
        freeStorage(inputStorage);
        exit(-1);
    }
    int bufferSize = 0; //размер считанной строки
    buffer[bufferSize] = '\0'; //избавляемся от чуши в значении
    char symbol = getc(inputFile); //читаем символ
    if (feof(inputFile)) {
        printf("Error: invalid input file format (something missed)\n"); //если прочитанный символ - конец файла, то вылетаем
        freeStorage(inputStorage);
        exit(-1);
    }
    while (symbol != EOF && symbol != '\n') { //до конца строки или до конца файла
        if (symbol != '\r') { //пропускаем возврат каретки
            buffer[bufferSize] = symbol; //записываем символ
            bufferSize++;
            buffer = (char *) realloc(buffer, sizeof(char) * (bufferSize + 1)); //расширяем память еще на один символ
            buffer[bufferSize] = '\0'; //избавляемся от чуши в новом символе
        }
        symbol = getc(inputFile); //читаем следующий
    }
    if (bufferSize == 0){
        printf("Error: invalid input file format (something missed)\n");
        freeStorage(inputStorage);
        exit(-1);
    } else
        return buffer;

}

void freeStorage(struct storage* inputStorage) {

    if(inputStorage != NULL) {
        if(inputStorage->alphabet != NULL)
            free(inputStorage->alphabet);
        for(int i = 0; i < 2; i++) {
            struct tapeSymbol* symb = inputStorage->tapeStart[i];
            struct tapeSymbol* buffSymb;
            while(symb != NULL) {
                buffSymb = symb->nextHead;
                free(symb);
                symb = buffSymb;
            }
        }
        if(inputStorage->states != NULL) {
            for(int i = 0; i < inputStorage->statesNumber; i++) {
                if(inputStorage->states[i] != NULL) {
                    for(int j = 0; j < inputStorage->alphabetSize; j++) {
                        if(inputStorage->states[i]->transitions[j] != NULL)
                            free(inputStorage->states[i]->transitions[j]);
                    }
                    free(inputStorage->states[i]);
                }
            }
            free(inputStorage->states);
        }
        free(inputStorage);
    }

}

int getSymbolIndex(char *alphabet, char symbol) {

    int i = 0;
    while (alphabet[i] != '\0') {
        if (alphabet[i] == symbol)
            return i;
        i++;
    }
    return -1;

}

void getAlphabet(struct storage *inputStorage, FILE *inputFile) {

    char *buffer = getLine(inputFile, inputStorage); //читаем первую строку
    int i = 0;
    while (buffer[i] != '\0') { //пока не конец строки
        if (buffer[i] <= ' ') { //запрещаем служебные символы и пробел
            printf("Error: invalid alphabet symbol\n");
            freeStorage(inputStorage);
            exit(-1);
        }
        if (!repeatCheck(inputStorage->alphabet, buffer[i], inputStorage->alphabetSize)) { //если такого символа нет
            inputStorage->alphabetSize++; //увеличиваем размер
            //расширяем память
            inputStorage->alphabet = (char *) realloc(inputStorage->alphabet,
                                                      sizeof(char) * inputStorage->alphabetSize);
            inputStorage->alphabet[inputStorage->alphabetSize - 1] = buffer[i]; //добавляем символ в алфавит
        } else {
            printf("Error: invalid alphabet format (repeating character)\n"); //если такой символ уже есть, вылетаем
            freeStorage(inputStorage);
            exit(-1);
        }
        i++;
    }
    free(buffer);
    inputStorage->alphabetSize++;
    inputStorage->alphabet = (char *) realloc(inputStorage->alphabet, sizeof(char) * (inputStorage->alphabetSize + 1));
    inputStorage->alphabet[inputStorage->alphabetSize - 1] = ' '; //добавляем пробел
    inputStorage->alphabet[inputStorage->alphabetSize] = '\0'; //обозначаем конец алфавита

}

void getTape(struct storage *inputStorage, FILE *inputFile, int tapeNumber) {

    char *buffer = getLine(inputFile, inputStorage); //читаем строку (головка)
    int headIndex = -1; //положение головки
    int tapeSize = 0; //размер ленты
    while (buffer[tapeSize] != '\0') { //пока не конец строки
        if (buffer[tapeSize] == 'v') { //ищем головку
            if (headIndex == -1) //проверка на несколько головок
                headIndex = tapeSize;
            else { //вылетаем, если несколько головок
                printf("Error: invalid input file format, multiple heads (tape %d)\n", tapeNumber + 1);
                freeStorage(inputStorage);
                exit(-1);
            }
        } else if (buffer[tapeSize] != '_') { //проверяем на левые символы
            printf("Error: invalid input file format (tape %d)\n", tapeNumber + 1);
            freeStorage(inputStorage);
            exit(-1);
        }
        tapeSize++;
    }
    buffer = getLine(inputFile, inputStorage); //читаем следующую строку (сама лента)
    struct tapeSymbol *symbol = NULL; //создаем пустой символ (чтобы быть предыдущим для первого элемента)
    inputStorage->tapeSize[tapeNumber] = tapeSize;
    for (int i = 0; i < tapeSize; i++) {
        if (buffer[i] == '\0') {
            printf("Error: invalid tape size (tape %d)\n", tapeNumber + 1); //проверяем размерность
            freeStorage(inputStorage);
            exit(-1);
        }
        if (!repeatCheck(inputStorage->alphabet, buffer[i], inputStorage->alphabetSize)) {
            printf("Error: invalid symbol on the tape (tape %d)\n", tapeNumber + 1); //проверяем, есть ли символ в алфавите
            freeStorage(inputStorage);
            exit(-1);
        }
        struct tapeSymbol *bufferTape = (struct tapeSymbol *) malloc(sizeof(struct tapeSymbol)); //создаем новый символ
        if (bufferTape == NULL) {
            printf("Memory allocation error\n");
            freeStorage(inputStorage);
            exit(-1);
        }
        bufferTape->previousHead = symbol; //предыдущий элемент
        bufferTape->nextHead = NULL;
        bufferTape->symbol = buffer[i]; //записываем символ
        if (symbol != NULL)
            symbol->nextHead = bufferTape; //финты со следующим элементом
        symbol = bufferTape; //symbol = последний добавленный элемент
        if (i == 0)
            inputStorage->tapeStart[tapeNumber] = symbol; //начала лент в storage
        if (i == headIndex) {
            inputStorage->currentHead[tapeNumber] = symbol; //элементы под головками в storage
            inputStorage->headIndex[tapeNumber] = headIndex; //положения головок в storage
        }
    }
    free(buffer);

}

void transitionsListParsing(struct storage *inputStorage, FILE *inputFile) {

    inputStorage->states = (struct stateTransitions **) malloc(
            inputStorage->statesNumber * sizeof(struct stateTransitions *)); //выделение памяти на массив состояний
            if (inputStorage->states == NULL) {
                printf("Memory allocation error\n");
                freeStorage(inputStorage);
                exit(-1);
            }
    for (int i = 0; i < inputStorage->statesNumber; i++) {
        inputStorage->states[i] = (struct stateTransitions *) malloc(sizeof(struct stateTransitions)); //память для содержимого
        if (inputStorage->states[i] == NULL) {
            printf("Memory allocation error\n");
            freeStorage(inputStorage);
            exit(-1);
        }
        inputStorage->states[i]->tapeNumber = -1; //инициализация номера ленты
        inputStorage->states[i]->transitions = (struct transition **) malloc(
                inputStorage->alphabetSize * sizeof(struct transition *)); //память для массива переходов
        if (inputStorage->states[i]->transitions == NULL) {
            printf("Memory allocation error\n");
            freeStorage(inputStorage);
            exit(-1);
        }
        for (int j = 0; j < inputStorage->alphabetSize; j++) {
            inputStorage->states[i]->transitions[j] = (struct transition *) malloc(sizeof(struct transition)); //память для одного перехода
            if (inputStorage->states[i]->transitions[j] == NULL) {
                printf("Memory allocation error\n");
                freeStorage(inputStorage);
                exit(-1);
            }
            //инициализация полей
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
    for (int i = 0; i < inputStorage->statesNumber * inputStorage->alphabetSize; i++) {
        scanfRet = fscanf(inputFile, "-%c-%c%d-%c-%c%d-%c\r\n", &readTape,
                          &readSymbol, &readState, &writeTape, &writeSymbol, &writeState, &action);
        if (scanfRet != 7) {
            printf("Error: invalid transitions list format (too few transitions)\n");
            freeStorage(inputStorage);
            exit(-1);
        }
        if (readState >= inputStorage->statesNumber) {
            printf("Error: read state number out of bounds on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }
        int readSymbolIndex = getSymbolIndex(inputStorage->alphabet, readSymbol); //индекс символа в массиве алфавита
        if (readSymbolIndex == -1) {
            printf("Error: invalid read symbol on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }
        if (inputStorage->states[readState]->transitions[readSymbolIndex]->flag == 1) { //проверяем, не инициализирован ли переход
            printf("Error: state redefinition on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }
        inputStorage->states[readState]->transitions[readSymbolIndex]->flag = 1;
        if (writeState > inputStorage->statesNumber) {
            printf("Error: write state number out of bounds on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }
        inputStorage->states[readState]->transitions[readSymbolIndex]->writeState = writeState;
        //проверяем наличие в алфавите символа, который хотим записать
        if (!repeatCheck(inputStorage->alphabet, writeSymbol, inputStorage->alphabetSize)) {
            printf("Error: invalid write symbol on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }
        inputStorage->states[readState]->transitions[readSymbolIndex]->writeSymbol = writeSymbol;
        if (readTape == 'O') {
            if (inputStorage->states[readState]->tapeNumber == -1) //если еще не инициализировали номер ленты
                inputStorage->states[readState]->tapeNumber = 0; //ставим индекс 0 (первая лента)
            else if (inputStorage->states[readState]->tapeNumber != 0) { //проверяем, что в этом состоянии указан только один номер ленты
                printf("Error: invalid read tape number on %d line\n", i);
                freeStorage(inputStorage);
                exit(-1);
            }
        } else if (readTape == 'T') {
            if (inputStorage->states[readState]->tapeNumber == -1) //если еще не инициализировали номер ленты
                inputStorage->states[readState]->tapeNumber = 1; //ставим индекс 1 (вторая лента)
            else if (inputStorage->states[readState]->tapeNumber != 1) { //проверяем, что в этом состоянии указан только один номер ленты
                printf("Error: invalid read tape number on %d line\n", i);
                freeStorage(inputStorage);
                exit(-1);
            }
        } else {
            printf("Error: invalid read tape identifier on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }

        //инициализация лент, на которые записываем
        if (writeTape == 'O')
            inputStorage->states[readState]->transitions[readSymbolIndex]->writeTape = 0;
        else if (writeTape == 'T')
            inputStorage->states[readState]->transitions[readSymbolIndex]->writeTape = 1;
        else {
            printf("Error: invalid write tape identifier on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }

        //инициализация действий
        if (action == 'L')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 0;
        else if (action == 'R')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 1;
        else if (action == 'H')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 2;
        else if (action == 'S')
            inputStorage->states[readState]->transitions[readSymbolIndex]->action = 3;
        else {
            printf("Error: invalid action identifier on %d line\n", i);
            freeStorage(inputStorage);
            exit(-1);
        }
    }
    if(fscanf(inputFile, "-%c-%c%d-%c-%c%d-%c\r\n", &readTape,
                         &readSymbol, &readState, &writeTape, &writeSymbol, &writeState, &action) != -1) {
        printf("Error: too many transitions in input file\n");
        freeStorage(inputStorage);
        exit(-1);
    }

}

void inputFileParsing(char *fileName, struct storage *inputStorage) {

    FILE *inputFile = fopen(fileName, "rb"); //открываем входной фаел
    if (inputFile == NULL) {
        printf("Error: can't open input file\n"); //вылетаем, если входной файл не открывается
        freeStorage(inputStorage);
        exit(-1);
    }
    getAlphabet(inputStorage, inputFile); //получаем алфавит
    getTape(inputStorage, inputFile, 0); //получаем состояние первой ленты
    getTape(inputStorage, inputFile, 1); //получаем состояние второй ленты
    if (fscanf(inputFile, "%d\r\n", &(inputStorage->statesNumber)) != 1) { //засовываем число состояний в storage
        printf("Error: can't file number of states\n"); //если не 1, то вылетаем
        freeStorage(inputStorage);
        exit(-1);
    }
    transitionsListParsing(inputStorage, inputFile); //получаем список переходов

}

void argsParsing(int argc, char *argv[], int *printFlag, int *executionMode, struct storage *inputStorage) {

    if (argc == 5) {
        if (strcmp(argv[4], "-p")) {
            printf("Error: invalid fourth command line argument (must be \"-p\")\n");
            freeStorage(inputStorage);
            exit(-1);
        }
        *printFlag = 1;
    }
    if (!strcmp(argv[3], "-a"))
        *executionMode = 0;
    else if (!strcmp(argv[3], "-o"))
        *executionMode = 1;
    else {
        printf("Error: invalid third command line argument (must be \"-a\" or \"-o\")\n");
        freeStorage(inputStorage);
        exit(-1);
    }

}

void parsing(struct storage *inputStorage, int argc, char *argv[], int *printFlag, int *executionMode) {

    //[1] - input.txt, [2] - output.txt, [3] - -a/-o, [4] - [-p]
    char *inputFileName;
    char *outputFileName;
    argsParsing(argc, argv, printFlag, executionMode, inputStorage); //проходим по флагам
    inputFileName = argv[1];
    outputFileName = argv[2];
    inputStorage->outputFile = fopen(outputFileName, "wt"); //открываем выходной файл
    if (inputStorage->outputFile == NULL) {
        printf("Error: can't open output file\n"); //вылетаем, если выходной файл не открывается
        freeStorage(inputStorage);
        exit(-1);
    }
    inputFileParsing(inputFileName, inputStorage); //парсим дальше
}
