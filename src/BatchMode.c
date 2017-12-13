/*
 * BatchMode.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<signal.h>
#include <sys/wait.h>


#include "BatchMode.h"
#include "VariableHandler.h"
#include "CommandExecution.h"
#include "StringHandler.h"
#include  "CommandParser.h"
#include "FilesHandler.h"

#define MAX_CHARARCTERS 520



void startBatch(char* filePath) {
	char ** command;
	FILE* file = fopen(filePath, "r");	// read file
	if (file) { // file exists
		char* line = malloc(MAX_CHARARCTERS);
		pid_t pid = fork();
		// fork for checking EOF
		if (pid == 0) { // child
			pid_t pid2 = fork();
			if (pid2 == 0) { //child-child
				while ( (getchar()) != EOF) {}
				kill(getppid(), SIGKILL); // kill parent

			} else { // child-parent
				while (fgets(line, MAX_CHARARCTERS, file)) {
					if (strlen(line) > 0) {
						line = replaceCharWithString(line, "", '\n');
						printf(">> %s\n", line);
						writeInHistory(line);
						command = getCommandArguments(line, MAX_CHARARCTERS);
						execute(command, 1);
					}



				}
				kill(getpid(), SIGKILL);
			}

		} else { // parent
			int status;
			waitpid(pid, &status,0);
			kill(pid, SIGKILL);
			exit(EXIT_SUCCESS);

		}

		fclose(file);

	} else {
		perror("fopen");
	}

}
