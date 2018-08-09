/*
Author: Michael Bajor
Project: Lab3_CSC305
File: List.h
*/

#ifndef LIST
#define LIST
#include "Process.h"

class List {
	private:
		Process *head, *tail, *currentProcess;
		int count;

	public:	
		List ();
		~List ();

		//	Element Access	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		Process* Head ()const;				//get head pointer
		Process* Tail ()const;				//get tail pointer
		Process* CurrentProcess ()const;	//get currentProcess pointer
		int Count ()const;					//get count

		//	CurrentProcess Operations	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void Next ();						//currentProcess moves to next process
		void Previous ();					//currentProcess moves to previous process
		void Set_Begin ();					//set currentProcess pointer to head
		void Set_End ();					//set currentProcess pointer to end

		//	Modifiers		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void Add_Process (Process *);		//add new process to back of list
		void Add_Front_Process (Process *);	//add new process to front of list
		void Remove_Process (Process *);	//remove process from list
		Process* Pop_First_Process ();		//removes first element from list and returns it

		//	Utilities		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void SortArrival ();				//sort by arrival time order
		void SortRequired ();				//sort by required time order
		void SortPriority ();				//sort by priority order
		void Swap_In_List (Process *, Process *);	//swap processes in same list

		//	Checks			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		bool isEmpty ();					//true if no processes in list
		bool isArrivalSorted ();			//true if list is sorted by arrival time
		bool isRequiredSorted ();			//true if list is sorted by required time
		bool isPrioritySorted ();			//true if list is sorted by priority

		//	Print			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		void Print ();						//print contents of list with header on top
};

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

List::List () {
	head = tail = currentProcess = NULL;
	count = 0;
}
List::~List () {
	Process *markForDel;	//mark for deletion
	Set_Begin ();
	while (!isEmpty ()) {
		markForDel = currentProcess;
		Next ();
		head = currentProcess;
		delete markForDel;
		count--;
	}
}

//	Element Access	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Process* List::Head ()const {
	return head;
}
Process* List::Tail ()const {
	return tail;
}
Process* List::CurrentProcess ()const {
	return currentProcess;
}
int List::Count ()const {
	return count;
}

//	CurrentProcess Operations	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void List::Next () {
	currentProcess = currentProcess->Next_Process ();
}
void List::Previous () {
	currentProcess = currentProcess->Previous_Process ();
}
void List::Set_Begin () {
	currentProcess = head;
}
void List::Set_End () {
	currentProcess = tail;
}

//	Modifiers		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void List::Add_Process (Process *newProcess) {
	//if empty list
	if (count == 0) {
		currentProcess = head = tail = newProcess;
		newProcess->Previous_Process (NULL);
		newProcess->Next_Process (NULL);
	}
	//not empty list, add to back
	else {
		tail->Next_Process (newProcess);
		newProcess->Previous_Process (tail);
		tail = newProcess;
		tail->Next_Process (NULL);
	}
	count++;
}
void List::Add_Front_Process (Process *newProcess) {
	//if list empty
	if (count == 0) {
		currentProcess = head = tail = newProcess;
		newProcess->Previous_Process (NULL);
		newProcess->Next_Process (NULL);
	}
	//not empty list, add to front
	else {
		head->Previous_Process (newProcess);
		newProcess->Next_Process (head);
		head = newProcess;
		head->Previous_Process (NULL);
	}
	count++;
}
void List::Remove_Process (Process *process) {
	if (process == head) {	//if process is head
		process->Next_Process ()->Previous_Process (NULL);
	} 
	else if (process == tail) { //if process is tail
		process->Previous_Process ()->Next_Process (NULL);
	}
	else { //process is neither head nor tail
		Process *previousProcess = process->Previous_Process ();
		Process *nextProcess = process->Next_Process ();
		previousProcess->Next_Process (nextProcess);
		nextProcess->Previous_Process (previousProcess);
	}
	count--;
	currentProcess = head;
}
Process* List::Pop_First_Process () {
	Process *firstElement = head;
	if (firstElement->Next_Process () != NULL) {
		head = firstElement->Next_Process ();
		head->Previous_Process (NULL);
	}
	else {
		head = tail = currentProcess = NULL;
	}
	count--;
	currentProcess = head;

	return firstElement;
}

//	Utilities		=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void List::SortArrival () {
	Set_Begin ();	//currentProcess pointer set to first process
	//while list NOT sorted by arrival time
	while (count > 1 && !isArrivalSorted ()) { 
		//while NOT last element  
		while (currentProcess != NULL && currentProcess->Next_Process () != NULL) {
			//if current arrival time is greater than next arrival time 
			if (currentProcess->Arrival_Time () > currentProcess->Next_Process ()->Arrival_Time ()) {
				Swap_In_List (currentProcess, currentProcess->Next_Process ());	
			}
			Next ();	//currentProcess pointer points to next process
		}
		Set_Begin ();
	}
}
void List::SortRequired () {
	Set_Begin ();	//currentProcess pointer set to first process
					//while list NOT sorted by arrival time
	while (count > 1 && !isRequiredSorted ()) {
		//while last two elements are not empty  
		while (currentProcess != NULL && currentProcess->Next_Process () != NULL) {
			//if current required time is greater than next required time 
			if (currentProcess->Required_Time () > currentProcess->Next_Process ()->Required_Time ()) {
				Swap_In_List (currentProcess, currentProcess->Next_Process ());
			}
			Next ();	//currentProcess pointer points to next process
		}
		Set_Begin ();
	}
}
void List::SortPriority () {
	Set_Begin ();	//currentProcess pointer set to first process
					//while list NOT sorted by priority
	while (count > 1 && !isPrioritySorted ()) {
		//while NOT last element  
		while (currentProcess != NULL && currentProcess->Next_Process () != NULL) {
			//if current priority is greater than next priority
			if (currentProcess->Priority () < currentProcess->Next_Process ()->Priority ()) {
				Swap_In_List (currentProcess, currentProcess->Next_Process ());
			}
			Next ();	//currentProcess pointer points to next process
		}
		Set_Begin ();
	}
}
void List::Swap_In_List (Process *a, Process *b) {
	Process *headEnd = a->Previous_Process ();
	Process *tailEnd = b->Next_Process ();

	//	'a' side
	a->Next_Process (tailEnd);
	a->Previous_Process (b);
	if (tailEnd == NULL) tail = a;
	else tailEnd->Previous_Process (a);
	//	'b' side
	b->Next_Process (a);
	b->Previous_Process (headEnd);
	if (headEnd == NULL) head = b;
	else headEnd->Next_Process (b);
}

//	Checks			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool List::isEmpty () {
	if (count == 0) return true;
	else return false;
}
bool List::isArrivalSorted () {
	Set_Begin ();	//currentProcess pointer set to first element in list
	while (currentProcess != NULL && currentProcess->Next_Process () != NULL) {
		if (currentProcess->Arrival_Time () > currentProcess->Next_Process ()->Arrival_Time ()) {
			Set_Begin ();
			return false;
		}
		else {
			Next ();
		}
	}
	Set_Begin ();
	return true;
}
bool List::isRequiredSorted () {
	Set_Begin ();	//currentProcess pointer set to first element in list
	while (currentProcess != NULL && currentProcess->Next_Process () != NULL) {
		if (currentProcess->Required_Time () > currentProcess->Next_Process ()->Required_Time ()) {
			Set_Begin ();
			return false;
		}
		else {
			Next ();
		}
	}
	Set_Begin ();
	return true;
}
bool List::isPrioritySorted () {
	Set_Begin ();	//currentProcess pointer set to first element in list
	while (currentProcess != NULL && currentProcess->Next_Process () != NULL) {
		if (currentProcess->Priority () < currentProcess->Next_Process ()->Priority ()) {
			Set_Begin ();
			return false;
		}
		else {
			Next ();
		}
	}
	Set_Begin ();
	return true;
}

//	Print			=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void List::Print () {
	Set_Begin ();	//set currentpointer to first element
	if (!isEmpty()) {
		int* colWidth = currentProcess->Column_Widths ();

		//header
		cout << endl << setw (colWidth[0]) << left << "NAME"
			<< setw (colWidth[1]) << left << "PRIORITY"
			<< setw (colWidth[2]) << left << "ARRIVE"
			<< setw (colWidth[3]) << left << "START"
			<< setw (colWidth[4]) << left << "REQUIRED"
			<< setw (colWidth[5]) << left << "TERMINATE"
			<< setw (colWidth[6]) << left << "TURNAROUND" << endl;

		//processes
		for (int i = 0; i < count; i++) {
			currentProcess->Print ();
			Next ();
		}
	}
	else cout << "Empty list" << endl;
	Set_Begin ();
}

#endif