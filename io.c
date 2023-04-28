#include "global.h"
#include "io.h"
#include "ipc.h"
#include "device.h"

#define PUT 0
#define GET 1 
#define MERGE 2 
#define NOOP -1
#define MODENUM 3

#define UP 0 
#define DOWN 1

#define VALSIZE 20 

extern int msgqueue; // message queue for key-value store request 
int IOMode = 0; 

extern int MainProcessID; 

void doIO()
{
	int modekey;
	int key;
	char value[VALSIZE];

	while(1)
	{
		modekey = BACKKEY;//device_read_mode_key();
		switch(modekey)
		{
			case BACKKEY: processExit(); break; 
			case VOLUP: mode_change(UP); //device_init(); break;
			case VOLDOWN: mode_change(DOWN); //device_init(); break; 
			default:
				;
		};

		switch(IOMode)
		{
			case NOOP:
			case PUT: 
			case GET:
				//key = device_read_data_key();
				//device_read_data_value(value); 
				sendKvMsg(msgqueue, key, value, IOMode); 
				break; 
			case MERGE:
			default:
				; 
		}

		modekey = NOOP ;
		IOMode = NOOP; 

	}
	
	//int rc = sendKvMsg(msgqueue, 123, 456); 

}

void processExit()
{
	// send kill signal to parent process and kill itself.
	sleep(5);
	printf("processExit function called\n"); 
	kill(MainProcessID, SIGINT); 
	exit(-1); 
}


void mode_change(int direction)
{
	// change IOMode (there exists three type.)
}






