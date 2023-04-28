#include <stdlib.h>

#include <signal.h>


#define VALSIZE 20 

 
typedef struct kv_data{
	int idx;
	int key; 
	char value[VALSIZE]; 
}kv_data;
