// For normal implementation 
#include <stdio.h>
#include <stdlib.h>

// For Fork systemcal, etc
#include <unistd.h>

// For IPC
#include <sys/msg.h>
#include <signal.h>


#include "global.h"
#include "main.h"
#include "io.h"
#include "merge.h"
#include "ipc.h"

#define MAX_PROCESS 3 
#define MERGE_PROCESS 2
#define IO_PROCESS 1
#define MAIN_PROCESS 0

#define ERRLOG(STR) printf(STR); exit(-1)

int msgqueue; // ID of message queue for key-value request.  
kv_data * memtable; 
int* memtable_idx; // the number of stored data in memtable. 
int data_idx = 0; // Total order of data inserted. 

int pid[MAX_PROCESS]; 
int MainProcessID, IOProcessID, MergeProcessID;


int main()
{
	int ProcessType; 
	
	memtable = (kv_data *)allocSharedMemory(sizeof(struct kv_data) * 3);
	memtable_idx = (int *)allocSharedMemory(sizeof(int)); 
	*memtable_idx = 0; 


	msgqueue = createMsgQueue();
	printf("msgqueue: %d\n", msgqueue); 
	ProcessType = fork_processes();

	//printf("%d\n", msgqueue);

	switch(ProcessType)
	{
		case MAIN_PROCESS: doMain();	break; 
		case IO_PROCESS: doIO();		break;
		case MERGE_PROCESS: doMerge();	break; 
		default: 
			ERRLOG("Should not approach here.\n"); 
	}
}

int fork_processes()
{

	int ProcessType; 

	MainProcessID = getpid(); 
	IOProcessID = fork(); 
	if (IOProcessID != 0) // parent process 
	{
		MergeProcessID = fork(); 
		ProcessType = (MergeProcessID)? MAIN_PROCESS : MERGE_PROCESS;  
		
	}
	else // child process 
		ProcessType = IO_PROCESS; 


	// maintain pid of the three processes for signal handling. 

	return ProcessType;
}


void doMain()
{
	int key, type, rc; 
	char value[20]; 	
	signal(SIGINT, sigint_handler); 
	signal(SIGUSR1, io_request_handler); 
	/*

	Job of the Main process
	1. Handle IO request from IO Process
	2. Send Merge request to Merge Process (when merge mode is set, )


	// Loop until SIGINT signal is received. 
	while(1)
	{


	}



	 */

	//memtable[0].key = 123; 
	//rc = receiveKvMsg(msgqueue, &key, &value, &type);
	//printf("received item\nkey: %d\nvalue: %d\n", key, value);
	while(1); 
}

void io_request_handler(int signo)
{
	int rc, key, type;
	char value[VALSIZE];
	
	printf("mem_idx before: %d\n", *memtable_idx);

	rc = receiveKvMsg(msgqueue, &key, value, &type); 
	if (rc == -1) 
		printf("This code should not be reached\n"); 

	printf("A - mem_idx before: %d\n", *memtable_idx);
	// if type is PUT, then put key&value pair to memtable. 
	if (type == PUT)
	{
		// TODO: lock should be held
		memtable[*memtable_idx].key = key; 
		printf("B - mem_idx before: %d\n", *memtable_idx);
		strncpy(memtable[*memtable_idx].value, value, VALSIZE); 
		printf("C - mem_idx before: %d\n", *memtable_idx);
		memtable[*memtable_idx].idx = data_idx; 
		printf("D - mem_idx before: %d\n", *memtable_idx);
		*memtable_idx = *memtable_idx + 1; 
		printf("E - mem_idx before: %d\n", *memtable_idx);
		data_idx ++; 
		// TODO: lock should be held
		
		if (*memtable_idx == MAX_MEMTABLE)
		{
			// send signal to MergeProcess to make it flush the memtable
			kill(MergeProcessID, SIGUSR1);
			//signal(SIGUSR1, MergeProcessID); 
		}		
		printf("put request is well handled. mem_idx: %d\n", *memtable_idx); 
	}
	// if type is GET, then find the key from memtable and sstable and send them to IOprocess.
	else{
		//TODO: Get Request handldler. 
	}


}



void sigint_handler (int signo)
{
	// Send sigint signal to another child process, MergeProcess
	printf("sigint handler executed\n"); 
	kill(MergeProcessID, SIGINT); 

	// Do some necessary jobs like flushing memtable to storage or something 

	deleteMsgQueue(msgqueue); 



	// Kill itself
	exit(-1); 


}
