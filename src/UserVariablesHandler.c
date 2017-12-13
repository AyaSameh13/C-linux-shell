/*
 * UserVariablesGetter.c
 *
 */

#include "UserVariablesHandler.h"

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER_VARIABLES 100
#define MAX_CHARACTERS 520


typedef struct { // table that contains all user variables
	char* name;
	char* value;
}userVarTable;


static userVarTable* table;
static int tableSize;
int variableIndex;

void initializeTable (){
	table = malloc(MAX_USER_VARIABLES);
	table->name = malloc(MAX_CHARACTERS);
	table->value = malloc(MAX_CHARACTERS);
	tableSize = 0;
}

char* getUserVariable(char* name) {
	for (int i = 0; i < tableSize ; i++) {
		char* nameInTable = table[i].name;
		if (nameInTable == NULL) { // table ends and var not found
			return NULL;
		}
		if (strcasecmp(nameInTable, name) == 0) { // var found
			variableIndex = i;
			char* toReturn = malloc(MAX_CHARACTERS);
			strcpy(toReturn, table[i].value);
			return toReturn;
			free(toReturn);
		}
	}
	return NULL;
}

void setUserVariable(char* name, char* value) {
	// check if it exists
		if (getUserVariable(name) == NULL) { // doesnot exist
			table[tableSize].name = malloc(MAX_CHARACTERS);
			table[tableSize].value = malloc(MAX_CHARACTERS);
			// insert at the end
			strcpy(table[tableSize].name, name);
			if (value == NULL) {
				value = "";
			}
			strcpy(table[tableSize].value, value);
			tableSize++;
		} else { // it exists so update it
			strcpy(table[variableIndex].value, value);

		}
}

void removeUserVariable(char* name) {
	if (getUserVariable(name) != NULL) {
		table[variableIndex].name = NULL;
		table[variableIndex].value = NULL;
	}
}



