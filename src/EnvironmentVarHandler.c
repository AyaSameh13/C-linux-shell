/*
 * environmentVarGetter.c
 *
 */

#include "EnvironmentVarHandler.h"

#include <stdio.h>
#include <stdlib.h>



char * getEnvironmentVar(char* varName) {
	char* value = getenv(varName);
	return value;
}


