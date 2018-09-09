#include "input.h"

int * step(int firstHeadState, int secondHeadState, int lastStateNum, struct ArrayPair tape, struct Pair *symbols, int *states,
           struct Command **arr, int maxArraySize, int maxTapeSize, int quit, FILE *outFile, _Bool flag) {
    while (quit != 1000) {
        quit++;
        struct Pair lastChar;
        lastChar.first = tape.first[firstHeadState];
        lastChar.second = tape.second[secondHeadState];
        int i = arrayContainsPair(lastChar, symbols, maxArraySize); // поиск индексов
        int j = arrayContainsInt(lastStateNum, states, maxArraySize);
        if (i == -1 || j == -1) {
            printf("error 203: Don't have command for the state\n");
            exit(203);
        }

        printCommand(arr[i][j], outFile);
        lastStateNum = arr[i][j].newStateNum;
        tape.first[firstHeadState] = arr[i][j].newChar.first;
        tape.second[secondHeadState] = arr[i][j].newChar.second;
        switch (arr[i][j].move.first) {
            case 'L':
                firstHeadState--;
                break;
            case 'R':
                firstHeadState++;
                break;
        }
        switch (arr[i][j].move.second) {
            case 'L':
                secondHeadState--;
                break;
            case 'R':
                secondHeadState++;
                break;
        }
        if (firstHeadState == maxTapeSize || secondHeadState == maxTapeSize) {
            maxTapeSize++;
            tape.first = realloc(tape.first, maxTapeSize * sizeof(struct Pair));
            tape.second = realloc(tape.second, maxTapeSize * sizeof(struct Pair));
            if (tape.first == NULL || tape.second == NULL) {
                printf("error 102: Memory allocation error\n");
                exit(102);
            }
            tape.first[maxTapeSize - 1] = ' ';
            tape.second[maxTapeSize - 1] = ' ';
        }
        if (firstHeadState < 0 || secondHeadState < 0) {
            printf("error 200: Head index out of bounds\n");
            exit(200);
        }
        if (arr[i][j].move.first == 'S') {
            break;
        }
        printHead(firstHeadState, maxTapeSize, outFile);
        printTape(tape.first, maxTapeSize, outFile);
        printHead(secondHeadState, maxTapeSize, outFile);
        printTape(tape.second, maxTapeSize, outFile);
        if (flag) {
            char comand[2]; //сюда будет записана команда
            while (1) {
                scanf(" %2s", comand);
                fflush(stdin); //очистка буфера
                if (!strcmp(comand, "b")) {
                    printf("Program break\n");
                    fclose(outFile);
                    exit(0);
                }
                if (!strcmp(comand, "s")) {
                    break;
                }
                if (!strcmp(comand, "f")) {
                    flag = false;
                    break;
                } else printf("No such instruction. Enter other\n");
            }
        }
    }

    printHead(firstHeadState, maxTapeSize, outFile);
    printTape(tape.first, maxTapeSize, outFile);
    printHead(secondHeadState, maxTapeSize, outFile);
    printTape(tape.second, maxTapeSize, outFile);

    if (quit == 1000) {
        printf("error 201: Runtime error\n");
        exit(201);
    }
    int *headstates;
    headstates = calloc(2, sizeof(int));
    headstates[0] = firstHeadState;
    headstates[1] = secondHeadState;
    return headstates;
}



