#pragma once
#include <iostream>
#include <string>
using namespace std;
class Job;

class Partition {
private:
	string name;
	int size, memoryWaste;
	Partition *nextPart;
	Partition *prevPart;
	Job *jobAssigned;

public:
	Partition () { name = ""; size = 0; memoryWaste = 0; }
	Partition (int s) { name = "", size = s; memoryWaste = 0; }
	~Partition () { cout << "Partition deleted." << endl; }

	void setName (string n) { name = n; }
	string getName () { return name; }

	void setSize (int s) { size = s; }
	int getSize () { return size; }

	void setNextPart (Partition &p) { nextPart = &p; }
	Partition* getNextPart () { return nextPart; }

	void setPrevPart (Partition &p) { prevPart = &p; }
	Partition* getPrevPart () { return prevPart; }

	void setJobAssignedNULL () { jobAssigned = NULL; }
	void setJobAssigned (Job &j) { jobAssigned = &j; setMemoryWaste (*jobAssigned); }
	Job* getJobAssigned () { return jobAssigned; }

	void setMemoryWaste (Job &j) { memoryWaste = size - j.getSize (); }
	int getMemoryWaste () { return memoryWaste; } 
};

