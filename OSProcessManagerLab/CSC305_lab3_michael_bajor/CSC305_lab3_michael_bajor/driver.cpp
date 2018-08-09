/*
Author: Michael Bajor
Project: Lab3_CSC305
File: driver.cpp
*/

#include <iostream>
#include <string>
#include "ProcessManager.h"
using namespace std;

void user_input (ProcessManager &);
void debug_input (ProcessManager &);

int main () {
	ProcessManager processManager;
	
	user_input (processManager);
	//debug_input (processManager);

	processManager.Choose_Algorithm ();
	processManager.Print ();

	system ("PAUSE");
	return 0;
}

void user_input (ProcessManager &pm) {
	int processCount;

	cout << "How many processes? ";
	cin >> processCount;

	for (int i = 0; i < processCount; i++) {
		pm.New_Process ();
	}
}
//Use debug function to avoid manual input everytime
//Just comment out user_input in main and uncomment debug_input
void debug_input (ProcessManager &pm) {
	pm.debugInput (5, 1, 0);
	pm.debugInput (3, 2, 0);
	pm.debugInput (1, 6, 1);
	pm.debugInput (15, 3, 3);
	pm.debugInput (1, 7, 0);
	pm.debugInput (3, 1, 0);
	pm.debugInput (3, 1, 0);
	pm.debugInput (4, 0, 2);
	pm.debugInput (6, 2, 1);
}