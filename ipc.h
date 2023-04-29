#include <sys/msg.h>

#define KEY 1 
#define VALSIZE 20 
struct kvMsg
{
	//long mtype; 
	//char mtext[80];
	int key; 
	char value[VALSIZE]; 
	int type; 
}; 



void * allocSharedMemory(int size); 


int createMsgQueue(); 
int openMsgQueue(int msgid); 
void deleteMsgQueue(int msgid); 

int sendKvMsg(int msgid, int key, char * value, int type);
int receiveKvMsg(int msgid, int *key, char * value, int *type);
