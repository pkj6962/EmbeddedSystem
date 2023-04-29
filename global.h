#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#define MAX_MEMTABLE 3
#define VALSIZE 20 

 
typedef struct kv_data{
	int idx;
	int key; 
	char value[VALSIZE]; 
}kv_data;
