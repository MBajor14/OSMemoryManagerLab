#pragma once
#include "Job.h"
#include "Partition.h"
#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;
enum OrderUsed { firstFit, bestFit, nextFit, worstFit };

class MemoryManager{
private:
	int MAXPARTS, MAXJOBS, MAXMEM, jobsCount, partsCount;
	bool executeOnce;
	Job *currentJob, *jobHead, *jobTail, *newJob;
	Partition *currentPart, *partHead, *partTail, *newPart;
	OrderUsed orderUsed;
	int col1, col2, col3, col4, col5, col6, col7; //column width for ouput table
	int *savedPartSizeInputs;
	int *savedJobSizeInputs;

public:
	MemoryManager ();
	~MemoryManager ();

	void resetJobsAndParts ();

	int getJobsCount () { return jobsCount; }
	int getPartsCount () { return partsCount; }

	void setOrderUsed (OrderUsed used) { orderUsed = used; }
	void setAllCurrentPointersToHead ();
	void setExecuteOnce (bool b) { executeOnce = b; }
	bool getExecuteOnce () { return executeOnce; }
	int getTotalMemWaste ();

	void setMaxJobs (int max) { MAXJOBS = max; }
	int getMaxJobs () { return MAXJOBS; }
	void setMaxParts (int max) { MAXPARTS = max; }
	int getMaxParts () { return MAXPARTS; }
	void setMaxMem (int max) { MAXMEM = max; }
	int getMaxMem () { return MAXMEM; }

	void addJob ();
	Job* getJobTail () { return jobTail; }
	void jumpToNextJob () { currentJob = currentJob->getNextJob (); } //move up the list
	void jumpToPrevJob () { currentJob = currentJob->getPrevJob (); } //move down the list
	void goToNextWaitingJob ();
	int getNumOfWaitingJobs ();
	bool jobIsEmpty () { if (jobsCount > 0) return false; else return true; }


	void addPart ();
	Partition* getPartTail() { return partTail; }
	void createPartsList ();
	void jumpToNextPart () { currentPart = currentPart->getNextPart (); }
	void jumpToPrevPart () { currentPart = currentPart->getPrevPart (); }
	bool partIsEmpty () { if (partsCount > 0) return false; else return true; }
	bool isAllPartsFilled ();

	void printPreSortedData ();
	void printProcess ();
	void printJob ();
	void printPartition ();
	void printHeader ();
	void printTotalMemWaste ();
	void printAllMemoryManagementTypes ();

	void processData ();
	void BestFit ();
	void FirstFit ();
	void NextFit ();
	void WorstFit ();
};
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
MemoryManager::MemoryManager () {
	jobsCount = partsCount = MAXPARTS = 0;
	currentJob = jobHead = jobTail = NULL;
	executeOnce = true;
	col1 = 6, col2 = 9, col3 = 6, col4 = 11, col5 = 6, col6 = 11;
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
MemoryManager::~MemoryManager () {
	currentJob = jobTail;// start at end of list
	currentPart = partTail;
	Job *del_J;	// marked for deletion
	Partition *del_P;

	while (!jobIsEmpty()) {
		del_J = currentJob;
		if (jobHead != jobTail) {
			jumpToPrevJob ();
			jobTail = currentJob;
		}
		delete del_J;	//delete Job
		jobsCount--;
	}

	while (!partIsEmpty ()) {
		del_P = currentPart;
		if (partHead != partTail) {
			jumpToPrevPart ();
			partTail = currentPart;
		}
		delete del_P;
		partsCount--;
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::setAllCurrentPointersToHead () {
	currentJob = jobHead;
	currentPart = partHead;
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::resetJobsAndParts () {
	currentPart = partHead;

	while (currentPart != NULL) {
		currentPart->setJobAssignedNULL ();
		jumpToNextPart ();
	}
	setAllCurrentPointersToHead ();
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
int MemoryManager::getTotalMemWaste () {
	int accumulatedMemWaste = 0;
	currentPart = partHead;

	while (currentPart != NULL) {
		if (currentPart->getJobAssigned () != NULL) {
			accumulatedMemWaste += currentPart->getMemoryWaste ();
			jumpToNextPart();
		}
		else jumpToNextPart ();
	}

	return accumulatedMemWaste;
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::addJob () {
	newJob = new Job;

	if (jobsCount == 0) { 
		newJob->setName ("Job1");
		//newJob->setSize (s);
		currentJob = jobHead = jobTail = newJob;
		jobsCount++;
	}
	else {
		string jobName = "Job" + to_string (jobsCount + 1);//name of Job 
		newJob->setName (jobName);
		//newJob->setSize (s);
		jobTail->setNextJob (*newJob);
		newJob->setPrevJob (*jobTail);
		jobTail = newJob;
		jobsCount++;
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::goToNextWaitingJob () {
	while (currentJob != NULL ) { 
		if (!currentJob->getStatus ()) return;	//if currently pointed to job is waiting, return
		else jumpToNextJob ();							//otherwise go to next job
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
int MemoryManager::getNumOfWaitingJobs () {
	int waitingCount = 0;
	currentJob = jobHead;

	while(currentJob != NULL) {
		if (!currentJob->getStatus ()) {
			waitingCount++;
			jumpToNextJob ();
		}
		else {
			jumpToNextJob ();
		}
	}

	return waitingCount;
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::addPart () {
	newPart = new Partition;

	if (partsCount == 0) {
		newPart->setName ("Par1");
		currentPart = partHead = partTail = newPart;
		partsCount++;
	}
	else {
		string partName = "Par" + to_string (partsCount + 1); //name of Partition
		newPart->setName (partName);
		partTail->setNextPart (*newPart);
		newPart->setPrevPart (*partTail);
		partTail = newPart;
		partsCount++;
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::createPartsList () {  //temporary method to create partions list quickly
	int size;

	srand (time (NULL));

	for (int i = 0; i < MAXPARTS; i++) {
		size = (rand () % 701 + 300)/100*100;
		addPart ();
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
bool MemoryManager::isAllPartsFilled () {
	Partition *tempPart = partHead;
	for (int i = 0; i < getPartsCount(); i++) {
		if (tempPart->getJobAssigned () == NULL) { //if partition empty
			return false;
		}
		else {
			tempPart = tempPart->getNextPart (); //go to next Partition
		}
	}

	return true; //partition list is not filled with jobs
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::printProcess () {
	int count = MAXPARTS + getNumOfWaitingJobs ();

	setAllCurrentPointersToHead ();//reset pointers to heads of list

	printHeader ();

	//data
	for (int i = 0; i < count; i++) {
		printJob ();
		printPartition ();
	}

	cout << endl; //extra line for cleaner output
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::printJob () {
	//print job info
	if (currentPart != NULL && currentPart->getJobAssigned() != NULL) {//if pointer to partition is not NULL and partision assigned job is not NULL
		cout << left << setw (col1) << currentPart->getJobAssigned ()->getName () << left << setw (col2);
		if (currentPart->getJobAssigned ()->getStatus ()) { cout << "Running"; }
		else { cout << "Waiting"; }
		cout << left << setw (col3) << currentPart->getJobAssigned ()->getSize ();
	}
	//print jobs that are waiting
	else {
		if (currentJob !=NULL && currentPart == NULL) {
			if (executeOnce) {
				currentJob = jobHead; //set current job pointer to head of list
				if (currentJob->getStatus ()) goToNextWaitingJob ();	//if first job in list is running, search for next waiting job
				setExecuteOnce (false); 
			}
			
			cout << left << setw (col1) << currentJob->getName() << left << setw (col2);
			if (currentJob->getStatus()) { cout << "Running"; }
			else { cout << "Waiting"; }
			cout << left << setw (col3) << currentJob->getSize();

			jumpToNextJob ();
			goToNextWaitingJob ();
		}
		else cout << left << setw (col1) << "-" << left << setw (col2) << "-" << left << setw (col3) << "-";
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::printPartition () {
	//print partition info
	if (currentPart != NULL) {
		cout << left << setw (col4) << currentPart->getName () << left << setw (col5) << to_string (currentPart->getSize ());
		if (currentPart->getJobAssigned () == NULL) cout << left << setw (col6) << "-" << endl;
		else cout << left << setw (col6) << currentPart->getMemoryWaste () << endl;
		jumpToNextPart (); 
	}
	else cout << left << setw (col4) << "-" << left << setw (col5) << "-" << left << setw (col6) << "-" << endl;
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::printHeader () {
	//header
	cout << endl << left << setw (col1) << "JOB" << left << setw (col2) << "STATUS" << left << setw (col3) << "SIZE"
		<< left << setw (col4) << "PARTITION" << left << setw (col5) << "SIZE" << left << setw (col6) << "MEM-WASTE" << endl;
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::printTotalMemWaste () {
	cout << endl << right << setw (col1 + col2 + col3 + col4 + col5) << "TOTAL MEMORY WASTE: " << left << setw (col6) << getTotalMemWaste () << endl << endl << endl;
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::printPreSortedData () {
	int count;
	if (jobsCount >= partsCount) count = jobsCount;
	else count = partsCount;

	setAllCurrentPointersToHead ();//reset pointers to heads of list

	printHeader ();

	for (int i = 0; i < count; i++) {
		//jobs
		if (currentJob != NULL) {
			cout << left << setw (col1) << currentJob->getName() << left << setw (col2);
			if (currentJob->getStatus()) { cout << "Running"; }
			else { cout << "Waiting"; }
			cout << left << setw (col3) << currentJob->getSize();
			jumpToNextJob ();
		}
		else cout << left << setw (col1) << "-" << left << setw (col2) << "-" << left << setw (col3) << "-";

		//Partitions
		printPartition ();
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::printAllMemoryManagementTypes () {
	orderUsed = firstFit;
	cout << "FIRST FIT"<< endl;
	processData ();
	printProcess ();
	printTotalMemWaste ();

	//---------------------------------
	orderUsed = nextFit;
	cout << "NEXT FIT" << endl;
	resetJobsAndParts ();
	processData ();
	printProcess ();
	printTotalMemWaste ();

	//---------------------------------
	orderUsed = bestFit;
	cout << "BEST FIT" << endl;
	resetJobsAndParts ();
	processData ();
	printProcess ();
	printTotalMemWaste ();

	//---------------------------------
	orderUsed = worstFit;
	cout << "WORST FIT" << endl;
	resetJobsAndParts ();
	processData ();
	printProcess ();
	printTotalMemWaste ();

}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::processData () {
	switch (orderUsed) {
		case firstFit:
			FirstFit ();
			break;
		case bestFit:
			BestFit ();
			break;
		case nextFit:
			NextFit ();
			break;
		case worstFit:
			WorstFit ();
			break;
		default:
			break;
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::FirstFit () {
	setAllCurrentPointersToHead ();
	if (!isAllPartsFilled ()) { //if there are available partitions excecute this code
		for (int i = 0; i < getJobsCount (); i++) {
			currentPart = partHead; //reset partition pointer to top

			for (int k = 0; k < getPartsCount (); k++) {
				if ((currentPart->getJobAssigned () == NULL) && (currentJob->getSize () <= currentPart->getSize ())) { //if job is smaller than partition and partition is available
					currentPart->setJobAssigned (*currentJob);
					currentJob->setStatus (true); //set to running
					currentPart = partHead; //go to top of partitions list
					break;
				}
				else { //if job cannot fit into partition
					if (currentPart != partTail) jumpToNextPart ();
				}
			}

			if (isAllPartsFilled()) break; //check if partitions list filled with jobs and break the loop if it is
			if (currentJob != jobTail)	jumpToNextJob ();
		}
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::NextFit () {
	setAllCurrentPointersToHead ();

	if (!isAllPartsFilled ()) { //if there are available partitions excecute this code
		for (int i = 0; i < getJobsCount (); i++) {
			

			for (int k = 0; k < getPartsCount (); k++) {
				if ((currentPart->getJobAssigned () == NULL) && (currentJob->getSize () <= currentPart->getSize ())) { //if job is smaller than partition and partition is available
					currentPart->setJobAssigned (*currentJob);
					currentJob->setStatus (true); //set to running
					if (currentPart->getNextPart() == NULL) currentPart = partHead; //if last Partition in list, point back to head
					else jumpToNextPart ();
					break;
				}
				else { //if job cannot fit into partition
					if (currentPart != partTail) jumpToNextPart ();
					else if (currentPart->getNextPart () == NULL) currentPart = partHead;
				}
			}
			if (isAllPartsFilled ()) break; //check if partitions list filled with jobs and break the loop if it is
			if (currentJob != jobTail)	jumpToNextJob ();
		}
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::BestFit () {
	setAllCurrentPointersToHead ();
	Partition *leastMemLossPart = NULL;
	int leastMemLoss = MAXMEM;

	if (!isAllPartsFilled ()) { //if there are available partitions excecute this code
		for (int i = 0; i < getJobsCount (); i++) {
			leastMemLossPart = currentPart = partHead; //reset partition pointer to top
			leastMemLoss = MAXMEM;

			for (int k = 0; k < getPartsCount (); k++) {
				if (currentPart->getJobAssigned () == NULL && currentPart->getSize () > currentJob->getSize ()) {
					if ((currentPart->getSize () - currentJob->getSize ()) < leastMemLoss) {//if difference of current partition and current job is less than previous partion and current job, overwrite
						leastMemLossPart->setJobAssignedNULL ();
						leastMemLossPart = currentPart;
						leastMemLoss = currentPart->getSize () - currentJob->getSize ();
						currentPart->setJobAssigned (*currentJob);
					}
					jumpToNextPart ();
				}
				else jumpToNextPart ();
			}

			if (leastMemLossPart->getSize() > currentJob->getSize()) { 
				leastMemLossPart->setJobAssigned (*currentJob);
				currentJob->setStatus (true);
			}
			else leastMemLossPart->setJobAssignedNULL ();

			if (isAllPartsFilled ()) break; //check if partitions list filled with jobs and break the loop if it is
			if (currentJob != jobTail)	jumpToNextJob ();
		}
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//
void MemoryManager::WorstFit () {
	setAllCurrentPointersToHead ();
	Partition *mostMemLossPart = NULL;
	int mostMemLoss = 0;

	if (!isAllPartsFilled ()) { //if there are available partitions excecute this code
		for (int i = 0; i < getJobsCount (); i++) {
			mostMemLossPart = currentPart = partHead; //reset partition pointer to top
			mostMemLoss = 0;

			for (int k = 0; k < getPartsCount (); k++) {
				if (currentPart->getJobAssigned () == NULL && currentPart->getSize () > currentJob->getSize ()) {
					if ((currentPart->getSize () - currentJob->getSize ()) > mostMemLoss) {//if difference of current partition and current job is less than previous partion and current job, overwrite
						mostMemLossPart->setJobAssignedNULL ();
						mostMemLossPart = currentPart;
						mostMemLoss = currentPart->getSize () - currentJob->getSize ();
						currentPart->setJobAssigned (*currentJob);
					}
					jumpToNextPart ();
				}
				else jumpToNextPart ();
			}

			if (mostMemLossPart->getSize () > currentJob->getSize ()) {
				mostMemLossPart->setJobAssigned (*currentJob);
				currentJob->setStatus (true);
			}
			else mostMemLossPart->setJobAssignedNULL ();

			if (isAllPartsFilled ()) break; //check if partitions list filled with jobs and break the loop if it is
			if (currentJob != jobTail)	jumpToNextJob ();
		}
	}
}
//+--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--++--=--+--=--+--=--+--=--+//