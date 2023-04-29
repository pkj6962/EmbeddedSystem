#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "global.h"
#include "ipc.h"
#include "util.h"





void * allocSharedMemory(int size)
{
	key_t key; 
	int shmid;
	void * ptr; 

	key = ftok("keyfile", KEY); 
	shmid = shmget(key, size, IPC_CREAT | 777); 
	if (shmid == -1)
	{
		perror("shmget error"); 
		exit(-1); 
	}

	ptr = shmat(shmid, NULL, 0);

	return ptr; 
}



int createMsgQueue()
{
	key_t key;
	int msgid; 

	key = ftok("keyfile", KEY); 

	msgid = msgget(key, IPC_CREAT|0644);
	//msgid = msgget(key, IPC_CREAT | 0666);
	if (msgid == -1)
	{
		perror("create message queue failed");
	}

	return msgid;
}

int openMsgQueue(int msgid)
{
	int msgqueue = msgget(msgid, 0); 
	if (msgqueue == -1)
		printf("Opening queue error\n"); 
	return msgqueue;
}

void deleteMsgQueue(int msgid)
{
	msgctl(msgid, IPC_RMID, NULL); 
}

int sendKvMsg(int msgid, int key, char * value, int type)
{
	int rc;
	struct kvMsg kvItem ;

	kvItem.key = key;
	strncpy(kvItem.value, value, VALSIZE); 
	//kvItem.value = value; 
	kvItem.type = type; 

	puts("z");
	printf("size: %ld", sizeof(struct kvMsg));
	puts("z");
	rc = msgsnd(msgid, (void *)&kvItem, sizeof(struct kvMsg), IPC_NOWAIT);
	printf("rc: %d\n", rc); 
	
	if (rc == -1)
		perror("Send message failed");
	puts("v");

	return rc; 
}

int receiveKvMsg(int msgid, int *key, char *value, int *type)
{
	int rc; 
	struct kvMsg * kvItem;

	kvItem = (struct kvMsg*)malloc(sizeof(struct kvMsg)); 
	if (kvItem == NULL)
		Exit("Memory allocation error.\n"); 

	rc = msgrcv(msgid, kvItem, sizeof(struct kvMsg), 0, IPC_NOWAIT); 
	if (rc == -1){

	}
		// No message to receive 
		//	perror("Receiving message failed");
	else{
		*key = kvItem->key;
		strncpy(value, kvItem->value, VALSIZE); 
		*type = kvItem->type;
		free(kvItem);
	}

	return rc; 
}
