/*
 * CommandExecution.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include <unistd.h>
#include  <sys/types.h>
#include <sys/wait.h>

#include "CommandExecution.h"
#include "CommandParser.h"
#include "VariableHandler.h"
#include "StringHandler.h"
#include "FilesHandler.h"


#define MAX_NO_PATHS 20
#define MAX_CHARACTERS 520

char* dir;
int batch;


int changeDir(char* newDir) {
	if (strlen(newDir) == 0) { // cd (home)
		newDir = getVaraible("HOME");

	} else if (strchr(newDir, '~')) { // ~ >> home
		if (strlen(newDir) == 1 || newDir[1] == '/') { // only ~ > replace with home dir normally
			newDir = replaceCharWithString(newDir, getVaraible("HOME"), '~');
		} else { // another user home directory
			char* userDir = malloc(MAX_CHARACTERS);
			char* rest = malloc(MAX_CHARACTERS);
			strcpy(userDir, newDir);
			userDir = strtok(userDir, "/");
			char* temp = strtok(NULL, "");
			if (temp != NULL) {
				rest= strcat(rest, "/");
				rest = strcat(rest, temp);
			}
			char* newUser = replaceCharWithString(userDir, "", '~');
			char* home = getVaraible("HOME");
			char* oldUser = extractCommandNameFromPath(home);
			userDir = replaceCharWithString(userDir, home, '~');
			userDir = replaceStringWithString(userDir, "", oldUser);
			newDir = strcat(userDir, rest);

			free(userDir);
			free(rest);
			free(temp);
		}
	}
	dir = newDir;
	return chdir(newDir);
}

void runExecv(char** command) {
	dir = getcwd(0,0);
	char* path = getVaraible("PATH");
	path = strcat(path,":");
	char* fileName = malloc(MAX_CHARACTERS);

	char* commandName = extractCommandNameFromPath(command[0]);// get command name
	char* pathToExecute = extractExecutionPath(command[0], commandName);// check if command name is in path extract it and extract path

	if (strlen(pathToExecute) > 0) {// check we dont need to look in PATH
		path = pathToExecute;

	}

	char** pathArray = seperatePath(path);


	for (int i = 0; i < getPathSize(); i++) {
		strcpy(fileName, pathArray[i]);
		strcat(fileName, "/");
		strcat(fileName, commandName);
		if( access(fileName, F_OK ) == 0 ) { // file exists
			if (execv(fileName, command) < 0) {
				free(fileName);
					perror("");
					changeDir(dir);
			} else {
				free(fileName);
				changeDir(dir);
			}
		}
	}
	free(fileName);
	perror("");
	changeDir(dir);
}


int isInBackground(char** command) {
	int sizeOfCommand = getCommandSize();
	char * word = command[sizeOfCommand - 2]; // last one is null , prev is last word
	if (strstr(word,"&")) {
		if (strlen(word) == 1) { // & is alone
			// decrease command;
			command[sizeOfCommand - 2] = NULL;
			return 1;
		} else { // next to a word
			if (word[strlen(word)-1] == '&') {// check it is last char
				char* newWord = malloc(strlen(word)-1);
				for (int i = 0; i < strlen(word)-1; i++) { // copy all letters except &
					newWord[i] = word[i];
				}
				strcpy(command[sizeOfCommand - 2], newWord);
				free(newWord);

				return 1;
			}
		}
	}
	return 0;

}

int commandIsDir(char** command) {
	if (strcasecmp(command[0], "cd") == 0) { // <<<< cd
		char* setDir;
		if (getCommandSize() <= 2) {
			setDir = "";
		} else {
			setDir = command[1];
		}
		if (changeDir(setDir) == -1) {
			perror("");
			// no signal kill cause it is parent
		} else {
		}
		return 1;
	} else {
		return 0;
	}
}

int commandIsExport(char** command) {
	if (strcasecmp(command[0], "export") == 0) { // <<<< cd
		char* name = strtok(command[1], "=");
		char* value = strtok(NULL, "=");
		setVariable(name, value);

		return 1;
	} else if (strchr(command[0], '=')) {
		char* name = strtok(command[0], "=");
		char* value = strtok(NULL, "=");
		setVariable(name, value);
		return 1;
	} else {
		return 0;
	}
}

int commandIsSet(char** command) {
	if (strcasecmp(command[0], "set") == 0) { // <<<< cd
//		if (batch) {
//			char* name = strtok(command[1], "=");
//			char* value = strtok(NULL, "=");
//			setVariable(name, value);
//		}
		return 1;
	} else {
		return 0;
	}
}

int commandIsHistory(char** command) {
	if (strcasecmp(command[0], "history") == 0) { // <<<< cd
		char* historyPath = getVaraible("HOME");
		historyPath = strcat(historyPath, "/history.txt");
		printFile(historyPath);
		return 1;
	} else {
		return 0;
	}
}

int commandIsExit(char** command) {
	if (strcasecmp(command[0], "exit") == 0) { // <<<< cd
		return 1;
	} else {
		return 0;
	}

}




void execute(char** command, int isBatch) {
	batch = isBatch;
	if (command[0] == NULL) {
		return; // empty line
	}

	if (commandIsDir(command)) { 	// changing directory should stay in parent
		return;
	} else if (commandIsExport(command)) {
		return;
	} else if (commandIsSet(command)) {
		return;
	} else if (commandIsHistory(command)) {
		return;

	} else if (commandIsExit(command)) {
		exit(EXIT_SUCCESS);

	}

	int status;
	int background = isInBackground(command);	// check if it ends with &

	if (background == 1) { // >> run in background
		pid_t pid = fork();
		if (pid == 0) { // child
			pid_t pid2 = fork();
			if (pid2 == 0) { // child-child
				runExecv(command);

			} else { // child-parent
				int stat;
				waitpid(pid2, &stat, 0); // wait for background to write in log file
				kill(pid2, SIGKILL);
				char* log = "Child terminated successfully";
				writeInLog(log, pid2);
				kill(getpid(), SIGKILL);
			}

		} else { // parent
			// do no thing as it runs in the background
		}
	} else {
			pid_t pid = fork();
			if (pid == 0) { // >>>>>> child process
				runExecv(command);

			} else { // >>>>>>> parent
				usleep(500);
				waitpid(pid,&status,0);
				char* log = "Child terminated successfully";
				writeInLog(log, pid);
				kill(pid, SIGKILL);
			}
	}




//	pid_t pid = fork();
//	if (pid == 0) { // >>>>>> child process
//		runExecv(command);
//
//	} else { // >>>>>>> parent
//		usleep(500);
//		if (background == 1) { // run in background
//
//
//		} else { // wait for it to finish
//			waitpid(pid,&status,0);
//				char* log = "Child terminated successfully";
//				writeInLog(log, pid);
//				kill(pid, SIGKILL);
//		}
//
//	}
}

