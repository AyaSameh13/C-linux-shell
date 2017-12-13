/*
 * variableGetter.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VariableHandler.h"
#include "EnvironmentVarHandler.h"
#include "UserVariablesHandler.h"

#define MAX_CHARACTERS 520

char* getVaraible(char* name) {

	char* fromEnvTable = getEnvironmentVar(name);
			if (fromEnvTable == NULL) {  // var not defined try in users
				char* fromUserTable = getUserVariable(name);
				if (fromUserTable == NULL) { // not in users table
					return NULL;
				} else {
					char* toReturn = malloc(MAX_CHARACTERS);
					strcpy(toReturn, fromUserTable);
					return toReturn;
				}

			} else {
				char* toReturn = malloc(MAX_CHARACTERS);
				strcpy(toReturn, fromEnvTable);
				return toReturn;
			}


}

char* setVariable (char* name, char* value) {
	// check if it is in env variable > change it
	char* env = getEnvironmentVar(name);
	if (env == NULL) { // not in it > change it in user variables
		setUserVariable(name, value);
	} else {
		setenv(name, value, 1);
	}
}

void removeVariable(char* name) {
	removeUserVariable(name);
}
