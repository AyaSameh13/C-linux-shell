/*
 * WordHandler.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StringHandler.h"


#define MAX_CHARACTERS 520
#define MAX_NO_PATHS 20

int pathSize;

char* replaceCharWithString(char* word, char* sub, char c) {
	int i = 0;
	char x[2];
	x[0] = word[i];
	x[1] = '\0';
	char* newWord = malloc(MAX_CHARACTERS);
	memset(newWord, 0, MAX_CHARACTERS);
	while (x[0] != c) {
		i++;
		strcat(newWord, x);
		if (i < strlen(word)) {
			x[0] = word[i];
		} else {
			break;
		}

	}
	strcat(newWord, sub);
	i++;
	x[0] = word[i];
	while (x[0] != 0) {
		i++;
		strcat(newWord, x);
		x[0] = word[i];
	}

	return newWord;

}

char* replaceStringWithString(char* string, char* newString, char* rmString) {
	int i = 0;
	char c;
	char* temp =  malloc(MAX_CHARACTERS);
	char* formed =  malloc(MAX_CHARACTERS);
	c = string[i];
	while (c != '\0') {
		memset(temp,0,strlen(temp));
		if (c == rmString[0]) { // could be the target
			int length = strlen(rmString);
			strncpy(temp, &string[i], length);
			if (strcmp(temp, rmString) == 0) { // it is the word
				i = i + length;
				formed = strcat(formed, newString);
			} else { // not the word
				i = i + length;
				formed = strcat(formed, temp);
			}

			c = string[i];

		} else { // continue adding it normally
			char add[2];
			add[0] = c;
			add[1] = '\0';
			formed = strcat(formed, add);
			i++;
			c = string[i];
		}
	}

	return formed;


}

char** seperatePath(char* path) {
	char** seperatePath = calloc(MAX_NO_PATHS, MAX_CHARACTERS);
	char* line = strtok(path, ":");
	int i = 0;
	while (line != NULL) {
		seperatePath[i++] = line;
		line = strtok(NULL, ":");
	}
	pathSize = i;
	return seperatePath;

}

char* extractCommandNameFromPath(char* command) {
	char* commandName = malloc(MAX_CHARACTERS);
	char* temp = malloc(MAX_CHARACTERS);
		strcpy(temp, command);
		temp = strtok(temp, "/");
		while (temp != NULL) {
			commandName = temp;
			temp = strtok(NULL, "/");
		}
	return commandName;
}


char* extractExecutionPath(char* command, char* commandName) {
	char* pathToExecute = malloc(MAX_CHARACTERS);
	strcpy(pathToExecute, command);
	// remove command name from path
	char *p = strstr(pathToExecute, commandName);
	if(p) {
		strcpy(p,p+strlen(commandName));
	}
	return pathToExecute;


}

int getPathSize() {
	return pathSize;
}
