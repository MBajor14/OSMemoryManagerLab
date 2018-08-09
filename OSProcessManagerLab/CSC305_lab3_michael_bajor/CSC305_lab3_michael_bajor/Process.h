/*
Author: Michael Bajor
Project: Lab3_CSC305
File: Process.h
*/

#ifndef PROCESS
#define PROCESS
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Process {
	private:
		string name;
		int requiredTime, timeLeft, arrivalTime, startTime, terminateTime, priority;
		Process *nextProcess, *previousProcess;
		int columnWidth[7];

	public:
		Process (string, int, int, int);	//constructor
		Process (Process &);				//copy constructor
		~Process ();						//destructor

		//	Element Access	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		string Name ()const;				//get name
		int Required_Time () const;			//get required time
		int Time_Left () const;				//get time left
		int Arrival_Time () const;			//get arrival time
		int Start_Time () const;			//get start time
		int Terminate_Time () const;		//get terminating time
		Process* Next_Process () const;		//get process pointed to by nextProcess pointer
		Process* Previous_Process () const;	//get process pointed to by previousProcess pointer
		int Priority () const;				//get priority
		int* Column_Widths ();				//get column widths array
		
		//	Modifiers		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void Next_Process (Process *);		//set nextProcess pointer 
		void Previous_Process (Process *);	//set previousProcess pointer
		void Terminate_Time (int);			//set terminate time
		void Start_Time (int);				//set start time
		void TimeLeft_Decrement ();			//decrement timeLeft by one 

		//	Calculations	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		int Turnaround_Time ();				//calculate and return turn around time for process

		//	Print			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void Print ();						//prints process details
};

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

Process::Process (string n, int rt, int at, int p) {
	this->name = n;
	this->requiredTime = rt;
	this->timeLeft = rt;
	this->arrivalTime = at;
	this->startTime = -1;
	this->priority = p;
	this->previousProcess = NULL;
	this->nextProcess = NULL;
	this->columnWidth[0] = 6;
	this->columnWidth[1] = 9;
	this->columnWidth[2] = 7;
	this->columnWidth[3] = 6;
	this->columnWidth[4] = 9;
	this->columnWidth[5] = 10;
	this->columnWidth[6] = 10;
}
Process::Process (Process &process) {
	this->name = process.name;
	this->requiredTime = process.requiredTime;
	this->timeLeft = process.timeLeft;
	this->arrivalTime = process.arrivalTime;
	this->startTime = process.startTime;
	this->terminateTime = process.terminateTime;
	this->priority = process.priority;
	this->previousProcess = process.previousProcess;
	this->nextProcess = process.nextProcess;
	this->columnWidth[0] = process.columnWidth[0];
	this->columnWidth[1] = process.columnWidth[1];
	this->columnWidth[2] = process.columnWidth[2];
	this->columnWidth[3] = process.columnWidth[3];
	this->columnWidth[4] = process.columnWidth[4];
	this->columnWidth[5] = process.columnWidth[5];
	this->columnWidth[6] = process.columnWidth[6];
}
Process::~Process () {
	nextProcess = previousProcess = NULL;
}

//	Element Access	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
string Process::Name ()const {
	return name;
}
int Process::Required_Time ()const {
	return requiredTime;
}
int Process::Time_Left ()const {
	return timeLeft;
}
int Process::Arrival_Time ()const {
	return arrivalTime;
}
int Process::Start_Time ()const {
	return startTime;
}
int Process::Terminate_Time ()const {
	return terminateTime;
}
Process* Process::Next_Process ()const {
	return nextProcess;
}
Process* Process::Previous_Process ()const {
	return previousProcess;
}
int Process::Priority ()const {
	return priority;
}
int* Process::Column_Widths () {
	return columnWidth;
}

//	Modifiers		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Process::Next_Process (Process *np) {
	nextProcess = np;
}
void Process::Previous_Process (Process *pp) {
	previousProcess = pp;
}
void Process::Terminate_Time (int tt) {
	terminateTime = tt;
}
void Process::Start_Time (int st) {
	startTime = st;
}
void Process::TimeLeft_Decrement () {
	if (timeLeft > 0) {
		timeLeft--;
	}
}

//	Calculations	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int Process::Turnaround_Time () {
	return (terminateTime - arrivalTime);
}

//	Print			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Process::Print () {
	cout << setw (columnWidth[0]) << left << name
		<< setw (columnWidth[1]) << left << priority
		<< setw (columnWidth[2]) << left << arrivalTime
		<< setw (columnWidth[3]) << left << startTime
		<< setw (columnWidth[4]) << left << requiredTime
		<< setw (columnWidth[5]) << left << terminateTime
		<< setw (columnWidth[6]) << left << Turnaround_Time () << endl;
}

#endif