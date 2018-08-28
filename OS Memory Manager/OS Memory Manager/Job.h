#pragma once
#include <iostream>
#include <string>
using namespace std;
class Partition;

class Job {
private:
	string name;
	Job* nextJob;
	Job* prevJob;
	bool status;
	Partition *partAssigned; //Partition assigned to 
	int size;

public:
	Job () { name = ""; size = 0; nextJob = NULL; prevJob = NULL; }
	~Job () { cout << "Job deleted."; nextJob = NULL; prevJob = NULL; }

	void setName (string n) { name = n; }
	string getName () { return name; }

	void setSize (int s) { size = s; }
	int getSize () { return size; }

	void setNextJob (Job &j) { nextJob = &j; }
	Job* getNextJob () { return nextJob; }

	void setPrevJob (Job &j) { prevJob = &j; }
	Job* getPrevJob () { return prevJob; }

	void setStatus (bool b) { status = b; }
	bool getStatus () { return status; }

	void setPartAssigned (Partition &p) { partAssigned = &p; }
	Partition* getPartAssigned () { return partAssigned; }
};

