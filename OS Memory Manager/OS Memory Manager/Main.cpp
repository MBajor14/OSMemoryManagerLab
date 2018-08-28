#include <iostream>
#include <string>
#include "MemoryManager.h"
#include "UserInput.h"
using namespace std;

int main () {
	MemoryManager *memoryManager = new MemoryManager ();
	UserInput UI;

	UI.promptForMaxMem (memoryManager); //prompt user for maximum memory allocated
	UI.promptForNumOfParts (memoryManager); //prompt user for max number of partitions
	UI.promptForPartSizes (memoryManager); //prompt user for the sizes of the partitions
	UI.promptForNumOfJobs (memoryManager); //prompt user for max number of jobs
	UI.promptForJobSizes (memoryManager); //prompt user for the sizes of the jobs
	UI.promptForOrderUsed (memoryManager); //prompt user for which method of memory management to use

	memoryManager->processData (); //excecute memory management based on previous selections
	memoryManager->printProcess (); //print results
	memoryManager->printTotalMemWaste ();

	system ("PAUSE");

	delete memoryManager;
	return 0;
}