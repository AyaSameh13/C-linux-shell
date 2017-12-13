/*
 * SingleMode.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "SingleMode.h"
#include "CommandParser.h"
#include "CommandExecution.h"
#include "FilesHandler.h"



void startSingle() {
	char * dir = getcwd(0,0);
	int maxSize = 520 * sizeof(char); // max size of each word
	char* command = malloc(maxSize);
	while (1) {
		dir = getcwd(0,0);
		printf("[%s]~>",dir);
		scanf("%[^\n]s", command);
		while (getchar() != '\n' ); // clear buffer
		if (strlen(command) > 0) {
			writeInHistory(command);
		}
		char** commandArray = getCommandArguments(command, maxSize);
		execute(commandArray, 0);




	}
}
