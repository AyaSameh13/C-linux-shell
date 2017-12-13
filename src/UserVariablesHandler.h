/*
 * UserVariablesGetter.h
 *
 */

#ifndef USERVARIABLESHANDLER_H_
#define USERVARIABLESHANDLER_H_

	void initializeTable ();
	char* getUserVariable(char* name);
	void setUserVariable(char* name, char* value);
	void removeUserVariable(char* name);

#endif /* USERVARIABLESHANDLER_H_ */
