#include "global.h"
#include "merge.h"

extern kv_data * memtable; 
void doMerge()
{

	sleep(3); 
	printf("key in memtable: %d\n", memtable[0].key); 

	printf("This is Merge Process\n"); 
	while(1); 
}
