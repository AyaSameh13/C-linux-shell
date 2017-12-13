/*
 ============================================================================
 Name        : shell.c
 Author      : aya
 Version     :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#include "SingleMode.h"
#include "BatchMode.h"
#include "UserVariablesHandler.h"
#include "VariableHandler.h"


int isBatch;
char* commands;



void determineMode(int argc, char* argv[]) {
	if (argc > 1) {
		// batch execution
		isBatch = 1;;
		startBatch(argv[1]);
	} else {
		// single line execution
		isBatch = 0;
		startSingle();
	}
}



int main(int argc, char* argv[]) {


	initializeTable(); // user variables table

	determineMode(argc, argv);





	return EXIT_SUCCESS;
}
