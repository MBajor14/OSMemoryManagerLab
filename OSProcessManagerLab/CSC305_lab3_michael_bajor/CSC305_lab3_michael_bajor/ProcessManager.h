/*
Author: Michael Bajor
Project: Lab3_CSC305
File: ProcessManager.h
*/

#ifndef PROCESSMANAGER
#define PROCESSMANAGER
#include "Process.h"
#include "List.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <utility>
using namespace std;

class ProcessManager {
	private:
		List inputLine, queue, terminated;
		Process *currentlyProcessing;
		int totalRequiredTime;

	public:
		ProcessManager ();
		~ProcessManager ();

		//	Element Access				=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		Process* Currently_Processed ();		//get process currently being processed

		//	Modifiers		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void New_Process ();					//get inputs for process 
		void Choose_Algorithm ();				//choose algorithm to be used
		void InputLine_To_Queue ();				//pop process from inputLine to queue (push_back)
		void Queue_To_Processing ();			//pop process from front of queue to currentlyProcessing pointer
		void Processing_To_Terminated ();		//move process from processing pointer to terminated (push_back)

		//	Utilities		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		string To_String (int);					//take int and return as string

		//	Calculations	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		double Average_Turnaround_Time ();		//calulate and return average turn around time ||***run after processes finished***||

		//	Algorithm		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void FirstCome_FirstServe ();			//first come first serve algorithm	*Non-preemptive
		void Shortest_Job_Next ();				//shortest job next algorithm		*Non-preemptive
		void Priority ();						//priority algorithm

		//	Print			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void Print ();							//print process details

		//	Debug			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void debugInput (int,int,int);
};

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

ProcessManager::ProcessManager () {
	currentlyProcessing = NULL;
	totalRequiredTime = 0;
}
ProcessManager::~ProcessManager () { ; }

//	Element Access	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Process* ProcessManager::Currently_Processed () {
	return currentlyProcessing;
}

//	Modifiers		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ProcessManager::New_Process () {
	string name = "P" + To_String (inputLine.Count() + 1);
	int arrivalTime, requiredTime, priority;

	cout << endl << "What is the time required for job execution of " << name << "? ";
	cin >> requiredTime;
	cout << "What is the arrival time? ";
	cin >> arrivalTime;
	cout << "What is the priority? (integer greater than 0) ";
	cin >> priority;

	Process *newProcess = new Process (name, requiredTime, arrivalTime, priority);
	totalRequiredTime += newProcess->Required_Time (); //extend total required time for all processesing
	inputLine.Add_Process (newProcess);
}
void ProcessManager::Choose_Algorithm () {
	char algorithmChoice;
	cout << "Which process algorithm would you like to use?\na)First Come First Serve\nb)Shortest Job Next\nc)Priority" << endl;
	cin >> algorithmChoice;

	switch (algorithmChoice) {
		case 'A':
		case 'a':
			cout << endl << "-------------------------------------------------" << endl;
			cout  << "FIRST COME FIRST SERVE --------------------------" << endl;
			FirstCome_FirstServe ();
			break;
		case 'B':
		case 'b':
			cout << endl << "-------------------------------------------------" << endl;
			cout << "SHORTEST JOB NEXT --------------------------------" << endl;
			Shortest_Job_Next ();
			break;
		case 'C':
		case 'c':
			cout << endl << "-------------------------------------------------" << endl;
			cout << "PRIORITY -----------------------------------------" << endl;
			Priority ();
			break;
		default:
			cout << "Improper input" << endl;
			break;
	}
}
void ProcessManager::InputLine_To_Queue () {
	queue.Add_Process (inputLine.Pop_First_Process());		//pop first element from inputLine and push_back on queue
}
void ProcessManager::Queue_To_Processing () {
	currentlyProcessing = queue.Pop_First_Process ();
}
void ProcessManager::Processing_To_Terminated () {
	terminated.Add_Process (currentlyProcessing);	//push_back process in terminated list
	currentlyProcessing = NULL;			//currentlyProcessing pointer set to NULL
}

//	Utilities		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
string ProcessManager::To_String (int n) {
	stringstream ss;
	ss << n;
	return ss.str ();
}

//	Calculations	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
double ProcessManager::Average_Turnaround_Time () {	//run after all processes finished
	double sum = 0;

	terminated.Set_Begin ();	//currentProcess pointer set to first process
	while (terminated.CurrentProcess() != NULL) {
		sum += terminated.CurrentProcess()->Turnaround_Time ();
		terminated.Next ();
	}

	return (sum / terminated.Count());
}

//	Algorithms		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ProcessManager::FirstCome_FirstServe () {
	inputLine.SortArrival ();	//sort inputLine by arrivalTime
	int runTime = 0, downTime = 0;

	while (runTime <= (totalRequiredTime + downTime)) {

		//move all processes (arrival time <= runTime) from inputLine to queue
		while ((inputLine.CurrentProcess () != NULL) && (inputLine.CurrentProcess ()->Arrival_Time () <= runTime)) {
			InputLine_To_Queue ();
		}

		//check if currentlyProcessing is pointing to anything
		if (currentlyProcessing != NULL) {
			if (currentlyProcessing->Time_Left () == 0) {
				currentlyProcessing->Terminate_Time (runTime);	//set terminate time for process
				Processing_To_Terminated ();
			}
			else {
				currentlyProcessing->TimeLeft_Decrement ();
				if (currentlyProcessing->Time_Left () == 0) {
					currentlyProcessing->Terminate_Time (runTime);	//set terminate time for process
					Processing_To_Terminated ();
				}
			}
		}

		//if currentlyProcessing is not pointing to anything and there is at least one process in queue list
		if (currentlyProcessing == NULL && !queue.isEmpty ()) {
			Queue_To_Processing ();	//pops first process from queue into currentlyProcessing pointer
			currentlyProcessing->Start_Time (runTime); //set start time for process 
		}


		//time pass
		if (currentlyProcessing == NULL && runTime < totalRequiredTime) downTime++;	//while not empty and not last second
		runTime++;
		inputLine.Set_Begin ();
	}
}
void ProcessManager::Shortest_Job_Next () {
	inputLine.SortArrival ();	//sort inputLine by arrivalTime
	int runTime = 0, downTime = 0;

	while (runTime <= (totalRequiredTime + downTime)) {

		//move all processes (arrival time <= runTime) from inputLine to queue
		while ((inputLine.CurrentProcess () != NULL) && (inputLine.CurrentProcess ()->Arrival_Time () <= runTime)) {
			InputLine_To_Queue ();
		}

		//sort queue by shortest job
		queue.SortRequired ();

		//check if currentlyProcessing is pointing to anything
		if (currentlyProcessing != NULL) {
			if (currentlyProcessing->Time_Left () == 0) {
				currentlyProcessing->Terminate_Time (runTime);	//set terminate time for process
				Processing_To_Terminated ();
			}
			else {
				currentlyProcessing->TimeLeft_Decrement ();
				if (currentlyProcessing->Time_Left () == 0) {
					currentlyProcessing->Terminate_Time (runTime);	//set terminate time for process
					Processing_To_Terminated ();
				}
			}
		}

		//if currentlyProcessing is not pointing to anything and there is at least one process in queue list
		if (currentlyProcessing == NULL && !queue.isEmpty ()) {
			Queue_To_Processing ();	//pops first process from queue into currentlyProcessing pointer
			currentlyProcessing->Start_Time (runTime); //set start time for process 
		}


		//time pass
		if (currentlyProcessing == NULL && runTime < totalRequiredTime) downTime++;	//while not empty and not last second
		runTime++;
		inputLine.Set_Begin ();
	}
}
void ProcessManager::Priority () {
	inputLine.SortArrival ();	//sort inputLine by arrivalTime
	int runTime = 0, downTime = 0;

	while (runTime <= (totalRequiredTime + downTime)) {
		//move all processes (arrival time <= runTime) from inputLine to queue
		while ((inputLine.CurrentProcess () != NULL) && (inputLine.CurrentProcess ()->Arrival_Time () <= runTime)) {
			InputLine_To_Queue ();
		}

		//sort queue by priority
		queue.SortPriority ();

		//check if currentlyProcessing is pointing to anything
		if (currentlyProcessing != NULL) {
			if (currentlyProcessing->Time_Left () == 0) {
				currentlyProcessing->Terminate_Time (runTime);	//set terminate time for process
				Processing_To_Terminated ();
			}
			else {
				currentlyProcessing->TimeLeft_Decrement ();
				if (currentlyProcessing->Time_Left () == 0) {
					currentlyProcessing->Terminate_Time (runTime);	//set terminate time for process
					Processing_To_Terminated ();
				}
			}

			//interupt processing and put highest priority in currentlyProcessing pointer
			if (currentlyProcessing != NULL && !queue.isEmpty () && queue.Head ()->Priority () > currentlyProcessing->Priority ()) {
				queue.Add_Front_Process (currentlyProcessing);
				currentlyProcessing = NULL;
				queue.SortPriority ();
				Queue_To_Processing ();
				if (currentlyProcessing->Start_Time () == -1) currentlyProcessing->Start_Time (runTime);
				downTime++;
			}	
		}

		//if currentlyProcessing is not pointing to anything and there is at least one process in queue list
		if (currentlyProcessing == NULL && !queue.isEmpty ()) {
			Queue_To_Processing ();	//pops first process from queue into currentlyProcessing pointer
			if (currentlyProcessing->Start_Time () == -1) currentlyProcessing->Start_Time (runTime); //set start time for process 
		}

		//time pass
		if (currentlyProcessing == NULL && runTime < totalRequiredTime) downTime++;	//while not empty and not last second
		runTime++;
		inputLine.Set_Begin ();
	}
}

//	Print			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ProcessManager::Print () {
	int* colWidth = terminated.CurrentProcess ()->Column_Widths ();

	//print terminated's list content
	terminated.Print ();

	//average turnaround print
	cout << endl << setw (colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3] + colWidth[4] + colWidth[5]) << right << "AVG TURNAROUND: ";
	if (fmod (Average_Turnaround_Time (), 1.0) != 0) cout << setprecision (2) << fixed << Average_Turnaround_Time () << endl;
	else cout << int (Average_Turnaround_Time ()) << endl;

	cout << "-------------------------------------------------" << endl;
}

//	Debug			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ProcessManager::debugInput (int p1_0, int p1_1, int p1_2) {

	Process *newProcess = new Process ("P"+(To_String(inputLine.Count()+1)), p1_0, p1_1, p1_2);
	totalRequiredTime += newProcess->Required_Time (); //extend total required time for all processesing
	inputLine.Add_Process (newProcess);
}

#endif