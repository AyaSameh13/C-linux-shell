/*
 * CommandParser.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CommandParser.h"
#include "StringHandler.h"
#include "VariableHandler.h"

#define MAX_ELEMENTS 10
#define MAX_CHARACTERS 520



char** shrinkArray(char** array, int size) {
	char ** newArray = calloc(size,MAX_CHARACTERS);
	for (int i = 0; i < size; i++) {
		newArray[i] = array[i];
	}
	commandSize = size;
	return newArray;
}

char* replaceVariables(char* array) {
	char c;
	int start;
	int end;
	char* var = malloc(MAX_CHARACTERS);
	char* varInTable = malloc(MAX_CHARACTERS);
	char* val;
	char* toReplace;

		int j = 0;
		c = array[j];
		while (c != '\0') {
			if (c == '$') { // there is variable
				if (j > 1) {
					if (array[j-1] == '\\') { // he wants to print the $
						j++;
						c = array[j];
						continue;
					}
				}
				// should replace it
					start = j;
					end;
					j++;
					c = array[j];
					while (c != '\0') {
						// check number / letter
						if ((c >= 0 && c <= 9) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
							j++;
							c = array[j];
						} else {
							end = j -1;
							break;
						}
					}
					end = j - 1;
					memset(var, 0, MAX_CHARACTERS);
					memcpy(var, &array[start], end-start+1);
					// get variable from env table
					memset(varInTable, 0, MAX_CHARACTERS);
					memcpy(varInTable, &var[1], end - start);
					val = getVaraible(varInTable);
					// replace it in word
					if (val == NULL) {
						val = "";
					}
					toReplace = replaceStringWithString(array, val, var);
					array = toReplace;
					j--;

			} else {

				j++;
				c = array[j];
			}
		}

	return array;
}

char* extractWordInQuotes(char* line) {
	int firstQ = strcspn(line, "\"");
	if (firstQ >= strlen(line )) { // no quotes
		return NULL;
	}
	char* temp = strrchr(line,'\"');
	int lastQ = strlen(line) - strlen(temp);
	if (firstQ == lastQ) { // no ending quotes
		return NULL;
	}
	int len = lastQ - firstQ - 1;
	char* sub = malloc(MAX_CHARACTERS);
	memcpy(sub, &line[firstQ]+1, len);
	return sub;



}

char** removeSpaces(char* line, char** array, int* i) {
	char * word = strtok (line," ");


	while( word != NULL )
	   {
		array[*i] = word;
		*i = *i +1;
		word = strtok(NULL, " ");

	   }

	return array;
}

char** getCommandArguments(char* line, int maxSize) {
	char ** array = calloc(MAX_ELEMENTS, MAX_CHARACTERS);
	if (strlen(line) > 512) {
		perror("Line longer than 512 character");
		return array;
	}
	if (line[0] == '#') { // comment
		return array;
	}
	int *i = malloc(sizeof(int));
	*i = 0;


	line = replaceVariables(line);
	char* inQuotes = extractWordInQuotes(line);

	if (inQuotes != NULL) {// there is word in quotes
		int SpaceBefore = 0;
		int spaceAfter = 0;

		char* beforeQuotes = strtok(line, "\"");
		char* rest = malloc(MAX_CHARACTERS);
		strcpy(rest, strtok(NULL, ""));
		beforeQuotes = strtok(beforeQuotes, "#");

		if (beforeQuotes != NULL) {
			if (beforeQuotes[strlen(beforeQuotes)-1] != ' ') { // attach quotes with it
				SpaceBefore = 1;
			}
		}
		removeSpaces(beforeQuotes, array, i);
		char* temp = malloc(MAX_CHARACTERS);
		memcpy(temp, &inQuotes[0], strlen(inQuotes));
		if (SpaceBefore) {
			*i = *i - 1;
			array[*i] = strcat(array[*i], temp);
		} else {
			array[* i] = temp;
		}
		* i = * i + 1;
		char* toRemove = malloc(MAX_CHARACTERS);
		strcat(toRemove, inQuotes);
		strcat(toRemove, "\"");
		char * afterQuotes = replaceStringWithString(rest, "", toRemove);
		afterQuotes = strtok(afterQuotes, "#");
		if (afterQuotes != NULL) {
			if (afterQuotes[0] != ' ') { // attach quotes with it
				spaceAfter = 1;
			}
		}

		removeSpaces(afterQuotes, array, i);

		if (afterQuotes != NULL) {
			if (strlen(afterQuotes) > 0) {

				if (spaceAfter) {
					*i = *i - 1;
					if (array[*i] != NULL) {
					char* after = malloc(MAX_CHARACTERS);
					strcpy(after,array[*i]);
					array[*i] = NULL;
					*i = *i - 1;
					array[*i] = strcat(array[*i], after);
					*i = *i + 1;
					}
				}
			}
		}


	} else {
		line = strtok(line, "#");
		removeSpaces(line, array, i);
	}



	// terminate by null
	array[*i] = NULL;
	* i = *i +1;


	// move the content of the array to another one equal in size

	char** returnArray = shrinkArray(array, *i);

	free(i);
	return returnArray;

}

int getCommandSize() {
	return commandSize;
}


