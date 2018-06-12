#include "input.h"

#define ARRAY_SIZE 1
#define TAPE_SIZE 20

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

int main(int argc, char *argv[]) {
    FILE *inputOne;
    FILE *inputTwo;
    FILE *output;

    if (argc == 5 && (strcmp(argv[4], "-a") == 0 || strcmp(argv[4], "-o") == 0)) {

        inputOne = fopen(argv[1], "rt");

        if (inputOne == NULL) {
            printf("Can not find file %s", argv[1]);
            exit(100);
        }

        inputTwo = fopen(argv[2], "rt");
        if (inputTwo == NULL) {
            printf("Can not find file %s", argv[2]);
            exit(100);
        }

        output = fopen(argv[3], "wr");
        if (output == NULL) {
            printf("Can not find file %s", argv[3]);
        }
    }

    if (argc != 5) {
        printf("Wrong number of arguments");
        exit(101);
    }

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

    printf("\n");
    //убрать печать
    for (int k = 0; k < maxArraySize; ++k) {
        for (int j = 0; j < maxArraySize; ++j) {
            printf("[%c,%c]%d[%c,%c]", command_array[k][j].newChar.first,
                   command_array[k][j].newChar.second, command_array[k][j].newStateNum, command_array[k][j].move.first,
                   command_array[k][j].move.second);
        }
        printf("\n");
    }

    struct ArrayPair head;
    head.first = calloc(TAPE_SIZE, sizeof(char));
    head.second = calloc(TAPE_SIZE, sizeof(char));
    int firstHeadState = -1;
    int secondHeadState = -1;
    struct ArrayPair tape;
    tape.first = calloc(TAPE_SIZE, sizeof(char));
    tape.second = calloc(TAPE_SIZE, sizeof(char));
    int maxTapeSize = TAPE_SIZE;

    ret = fscanf(inputTwo, " %s", head.first);

    if (ret == NULL) {
        printf("No input data in %s line 1", argv[2]);
        exit(107);
    }


    while (head.first[maxTapeSize] != '\0') {
        maxTapeSize = maxTapeSize + 1;
        head.first = realloc(head.first, maxTapeSize * sizeof(int));
        for (int j = maxTapeSize - 1; j < maxTapeSize; ++j) {
            head.first[j] = '\0';
        }
    }

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

    ret = fscanf(inputTwo, " %s", tape.first);

    if (ret == NULL) {
        printf("No input data in %s line 2", argv[2]);
        exit(107);
    }

    fclose(inputTwo);

    //убрать печать
    printf("%d", firstHeadState);
    printf("\n");
    for (int m = 0; m < maxTapeSize; ++m) {
        printf("%c", tape.first[m]);
    }
    exit(0);

}
