/*
 * StringHandler.h
 *
 *  Created on: Oct 6, 2017
 *      Author: aya
 */

#ifndef STRINGHANDLER_H_
#define STRINGHANDLER_H_

char* replaceCharWithString(char* word, char* sub, char c);
char** seperatePath(char* path);
char* replaceStringWithString(char* string, char* newString, char* rmString);
int getPathSize();  // after seperating path
char* extractCommandNameFromPath(char* command);
char* extractExecutionPath(char* command, char* commandName);


#endif /* STRINGHANDLER_H_ */
