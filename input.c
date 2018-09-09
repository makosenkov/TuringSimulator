#include "input.h"

#define ARRAY_SIZE 1
#define TAPE_SIZE 10

int arrayContainsPair(struct Pair symbols, struct Pair *arr, int maxSize) {
    for (int i = 0; i < maxSize - 1; ++i) {
        if (arr[i].first == symbols.first && arr[i].second == symbols.second) return i;
    }
    return -1;
}

int arrayContainsInt(int symbol, int *arr, int maxSize) {
    for (int i = 0; i < maxSize - 1; ++i) {
        if (arr[i] == symbol) return i;
    }
    return -1;
}

int input (FILE *inputOne, FILE *inputTwo, FILE *outFile, int argc, char *argv[]) {

    _Bool flag = false;

    struct Pair lastChar;
    struct Pair newChar;
    struct Pair move;
    char lastState;
    int lastStateNum;
    char dash;
    char newState;
    int newStateNum;
    int maxArraySize = ARRAY_SIZE;

    int *states;
    states = calloc(ARRAY_SIZE, sizeof(states));
    struct Pair *symbols;
    symbols = calloc(ARRAY_SIZE, sizeof(symbols));

    int i = 0;
    int statesCount = 0;
    int symbolsCount = 0;
    struct Pair blankPair = {'\0', '\0'};
    struct Command blankCommand = {blankPair, 0, blankPair};

    struct Command **command_array;
    command_array = calloc(ARRAY_SIZE, sizeof(struct Command *));

    for (int j = 0; j < ARRAY_SIZE; ++j) {
        command_array[j] = calloc(ARRAY_SIZE, sizeof(struct Command));
    }

    if (command_array == NULL) {
        printf("Memory allocation error");
        exit(102);
    }

    int ret = 0;

    while (true) {
        ret = fscanf(inputOne, " %c %c %c %c %c %c %d %c %c %c %c %c %c %c %d  %c %c %c %c %c",
                     &dash, &lastChar.first, &dash, &lastChar.second, &dash, &lastState, &lastStateNum,
                     &dash, &dash, &newChar.first, &dash, &newChar.second, &dash, &newState, &newStateNum,
                     &dash, &move.first, &dash, &move.second, &dash);
        if (ret == EOF) break;

        //поменять коды ошибок и сообщения

        if (ret != 20) {
            printf("Wrong number of input chars");
            exit(103);
        }

        if (lastState != 'q' || newState != 'q') {
            printf("Can not find states");
            exit(104);
        }

        if (arrayContainsPair(lastChar, symbols, maxArraySize) == -1) {
            symbols[symbolsCount] = lastChar;
            symbolsCount++;
        }

        if (arrayContainsInt(lastStateNum, states, maxArraySize) == -1) {
            states[statesCount] = lastStateNum;
            statesCount++;
        }

        i++;

        if (i == maxArraySize) {
            maxArraySize = maxArraySize + 1;
            states = realloc(states, maxArraySize * sizeof(int));
            symbols = realloc(symbols, maxArraySize * sizeof(char));
            command_array = realloc(command_array, maxArraySize * sizeof(struct Command *));

            for (int j = 0; j < maxArraySize - 1; ++j) {
                command_array[j] = realloc(command_array[j], maxArraySize * sizeof(struct Command));
                for (int k = maxArraySize - 1; k < maxArraySize; ++k) {
                    command_array[j][k] = blankCommand;
                }
            }

            for (int k = maxArraySize - 1; k < maxArraySize; ++k) {
                command_array[k] = calloc(maxArraySize, sizeof(struct Command));
            }
        }

        command_array[arrayContainsPair(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                           maxArraySize)].newChar = newChar;
        command_array[arrayContainsPair(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                           maxArraySize)].newStateNum = newStateNum;
        command_array[arrayContainsPair(lastChar, symbols, maxArraySize)][arrayContainsInt(lastStateNum, states,
                                                                                           maxArraySize)].move = move;
    }

    fclose(inputOne);

     // печать матрицы смежности
    //if (argc == 6 && (strcmp(argv[5], "-p") == 0)) {
        printCommands(command_array, maxArraySize, outFile);
   // }

    struct ArrayPair head;
    head.first = calloc(TAPE_SIZE, sizeof(char));
    head.second = calloc(TAPE_SIZE, sizeof(char));
    if (head.first == NULL || head.second == NULL) {
        printf("error 102: Memory allocation error\n");
        exit(102);
    }
    
    int firstHeadState = -1;
    int secondHeadState = -1;
    struct ArrayPair tape;
    tape.first = calloc(TAPE_SIZE, sizeof(char));
    tape.second = calloc(TAPE_SIZE, sizeof(char));
    if (tape.first == NULL || tape.second == NULL) {
        printf("error 102: Memory allocation error\n");
        exit(102);
    }

    ret = fscanf(inputTwo, " %s", head.first); //читаем первую строку

    if (ret == -1) {
        printf("No input data in %s line 1", argv[2]);
        exit(107);
    }

    int maxTapeSize = ret;

    //память для первой головки
    while (head.first[maxTapeSize] != '\0') {
        maxTapeSize = maxTapeSize + 1;
        head.first = realloc(head.first, maxTapeSize * sizeof(int));
        for (int j = maxTapeSize - 1; j < maxTapeSize; ++j) {
            head.first[j] = '\0';
        }
    }

    //нахождение первой головки
    for (int l = 0; l < maxTapeSize; ++l) {
        if (head.first[l] != '_' && head.first[l] != 'v' && head.first[l] != '\0') {
            printf("Wrong input char %c in %s line 1", head.first[l], argv[2]);
            exit(108);
        }
        if (head.first[l] == 'v'){
            firstHeadState = l;
        }
    }

    if (firstHeadState == -1) {
        printf("Wrong input data in %s line 1", argv[2]);
        exit(109);
    }

    ret = fscanf(inputTwo, " %s", tape.first); //читаем вторую строку

    if (ret == NULL) {
        printf("No input data in %s line 2", argv[2]);
        exit(107);
    }

    ret = fscanf(inputTwo, " %s", head.second); //читаем третью строку

    if (ret == NULL) {
        printf("No input data in %s line 3", argv[2]);
        exit(107);
    }

    //память для второй головки
    while (head.second[maxTapeSize] != '\0') {
        maxTapeSize = maxTapeSize + 1;
        head.second = realloc(head.second, maxTapeSize * sizeof(int));
        for (int j = maxTapeSize - 1; j < maxTapeSize; ++j) {
            head.second[j] = '\0';
        }
    }

    //нахождение второй головки
    for (int l = 0; l < maxTapeSize; ++l) {
        if (head.second[l] != '_' && head.second[l] != 'v' && head.second[l] != '\0') {
            printf("Wrong input char %c in %s line 2", head.second[l], argv[2]);
            exit(108);
        }
        if (head.second[l] == 'v'){
            secondHeadState = l;
        }
    }



    ret = fscanf(inputTwo, " %s", tape.second); //читаем четвертую строку

    if (ret == NULL) {
        printf("No input data in %s line 4", argv[2]);
        exit(107);
    }

    fclose(inputTwo);

    //убрать печать

    if (strcmp(argv[4], "-o") == 0) { // вызван ли режим debug
        flag = true;
    }

    printHead(firstHeadState, maxTapeSize, outFile);
    printTape(tape.first, maxTapeSize, outFile);
    printHead(secondHeadState, maxTapeSize, outFile);
    printTape(tape.second, maxTapeSize, outFile);

    int quit = 0; //количество шагов

    step(firstHeadState, secondHeadState, 1, tape, symbols, states, command_array, maxArraySize, maxTapeSize, quit, outFile, flag);

    free(head.first);
    free(head.second);
    free(tape.first);
    free(tape.second);
    free(states);
    free(symbols);
    for (int m = 0; m < maxArraySize; ++m) {
        free(command_array[m]);
    }
    fclose(outFile);
    exit(0);

}



