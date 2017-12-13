/*
 * FilesHandler.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FilesHandler.h"
#include "VariableHandler.h"

#define MAX_CHARARCTERS 520


void writeInHistory(char* line) {
	char* historyPath = getVaraible("HOME");
	historyPath = strcat(historyPath, "/history.txt");
	FILE* file = fopen(historyPath, "a");	// append to file
		if (file) { // file exists
			fprintf(file, "%s\n", line);
			fclose(file);
		} else {
			perror("");
		}
}
void writeInLog(char* line, int pid) {
	char* logPath = getVaraible("HOME");
	logPath = strcat(logPath, "/log.txt");
	FILE* file = fopen(logPath, "a");	// append to file
		if (file) { // file exists
			fprintf(file, "[%d] %s\n",pid, line);
			fclose(file);
		} else {
			perror("");
		}
}

void printFile(char* path) {
	FILE* file = fopen(path, "r");	// read file
		if (file) { // file exists
			char* line = malloc(MAX_CHARARCTERS);
			while (fgets(line, MAX_CHARARCTERS, file)) {
				printf(">> %s", line);
			}
			free(line);
		}
}


