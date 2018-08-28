#pragma once
#include <iostream>
using namespace std;
class MemoryManager;

class UserInput {
private:
	int intVar;
	char charVar;
public:
	void promptForMaxMem (MemoryManager *);
	void promptForNumOfParts (MemoryManager *);
	void promptForPartSizes (MemoryManager *);
	void promptForNumOfJobs (MemoryManager *);
	void promptForJobSizes (MemoryManager *);
	void promptForOrderUsed (MemoryManager *);
};

//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void UserInput::promptForMaxMem (MemoryManager *memManager) {
	cout << "What is the max memory allocated? ";
	cin >> intVar;
	memManager->setMaxMem (intVar);
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void UserInput::promptForNumOfParts (MemoryManager *memManager) {
	cout << endl << "How many partitions are there? ";
	cin >> intVar;
	memManager->setMaxParts (intVar);
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void UserInput::promptForPartSizes (MemoryManager *memManager) {
	int memCount = memManager->getMaxMem();
	

	for (int i = 0; i < memManager->getMaxParts(); i++) {
		memManager->addPart ();
		cout << endl << "You have " << memCount << "k of data left" << endl;
		
		if (i == (memManager->getMaxParts () - 1)) {
			cout << "The last partition has been set to the remaining memory available: " << memCount << endl;
			memManager->getPartTail ()->setSize (memCount);
			memCount -= memCount;
		}
		else {
			cout << "What is the size of " << memManager->getPartTail ()->getName () << " ? ";
			cin >> intVar;
			memManager->getPartTail ()->setSize (intVar);
			memCount -= intVar;
		}
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void UserInput::promptForNumOfJobs (MemoryManager *memManager) {
	cout << endl << "How many jobs are there? ";
	cin >> intVar;
	memManager->setMaxJobs (intVar);
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void UserInput::promptForJobSizes (MemoryManager *memManager) {
	for (int i = 0; i < memManager->getMaxJobs(); i++) {
		memManager->addJob ();
		cout << endl << "What is the size of " << memManager->getJobTail ()->getName () << " ? ";
		cin >> intVar;
		memManager->getJobTail ()->setSize (intVar);
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void UserInput::promptForOrderUsed (MemoryManager *memManager) {
	bool exitMenu = false;

	while (!exitMenu) {
		cout << "What method would you like to use?" << endl;
		cout << "a)First Fit" << endl;
		cout << "b)Best Fit" << endl;
		cout << "c)Next Fit" << endl;
		cout << "d)Worst Fit" << endl;
		cout << "e)Screw it... print them all" << endl;
		cin >> charVar;

		switch (charVar) {
			case'A':
			case'a':
				memManager->setOrderUsed (firstFit);
				exitMenu = true;
				break;

			case'B':
			case'b':
				memManager->setOrderUsed (bestFit);
				exitMenu = true;
				break;

			case'C':
			case'c':
				memManager->setOrderUsed (nextFit);
				exitMenu = true;
				break;

			case'D':
			case'd':
				memManager->setOrderUsed (worstFit);
				exitMenu = true;
				break;

			case 'E':
			case 'e':
				memManager->printAllMemoryManagementTypes ();
				exitMenu = true;
				break;

			default:
				cout << "Invalid entry." << endl;
				break;
		}
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//