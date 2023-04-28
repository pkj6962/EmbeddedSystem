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




int pid[MAX_PROCESS]; 
int MainProcessID, IOProcessID, MergeProcessID;


int main()
{
	int ProcessType; 
	
	memtable = (kv_data *)allocSharedMemory(sizeof(struct kv_data) * 3);
	msgqueue = createMsgQueue(); 
	ProcessType = fork_processes();

	printf("%d\n", msgqueue);

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
	int key, value, type, rc; 
	
	signal(SIGINT, sigint_handler); 

	memtable[0].key = 123; 
	rc = receiveKvMsg(msgqueue, &key, &value, &type);
	printf("received item\nkey: %d\nvalue: %d\n", key, value);
	while(1); 
}



void sigint_handler (int signo)
{
	// Send sigint signal to another child process, MergeProcess
	printf("sigint handler executed\n"); 
	kill(MergeProcessID, SIGINT); 

	// Do some necessary jobs like flushing memtable to storage or something 
		


	// Kill itself
	exit(-1); 


}
