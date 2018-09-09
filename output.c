#include "input.h"


void printTape(char *tape, int maxTapeSize, FILE *outFile) {
    int i = 0;
    while (tape[i] != '\0' && i < maxTapeSize) {
        printf("%c", tape[i]);
        fprintf(outFile, "%c", tape[i]);
        i++;
    }
    printf("\n");
    fprintf(outFile, "\n");
}

void printHead(int headState, int maxTapeSize, FILE *outFile) {
    printf("Tape");
    fprintf(outFile, "Tape");
    printf("\n");
    fprintf(outFile, "\n");
    int i = 0;
    while (i != maxTapeSize) {
        if (i == headState) {
            printf("v");
            fprintf(outFile, "v");
        }
        if (i != headState) {
            printf("_");
            fprintf(outFile, "_");
        }
        i++;
    }
    printf("\n");
    fprintf(outFile, "\n");
}


void printCommand(struct Command command, FILE *outFile) {
    printf("Running command");
    fprintf(outFile, "Running command");
    printf("\n");
    fprintf(outFile, "\n");
    printf("[%c,%c]%d[%c,%c]", command.newChar.first, command.newChar.second,
            command.newStateNum, command.move.first, command.move.second);
    fprintf(outFile, "[%c,%c]%d[%c,%c]", command.newChar.first, command.newChar.second,
            command.newStateNum, command.move.first, command.move.second);
    printf("\n");
    fprintf(outFile, "\n");
}

void printCommands(struct Command **command_array, int maxArraySize, FILE *outFile) {
    printf("Command matrix");
    fprintf(outFile, "Command matrix");
    printf("\n");
    fprintf(outFile, "\n");
    for (int k = 0; k < maxArraySize; ++k) {
        for (int j = 0; j < maxArraySize; ++j) {
            printf("[%c,%c]%d[%c,%c]", command_array[k][j].newChar.first,
                   command_array[k][j].newChar.second, command_array[k][j].newStateNum, command_array[k][j].move.first,
                   command_array[k][j].move.second);
        }
        printf("\n");
        fprintf(outFile, "\n");
    }
}

