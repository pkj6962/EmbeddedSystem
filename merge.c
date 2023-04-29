#include "global.h"
#include "merge.h"


int sstable_idx;
int nextSSTableIndex=0; 
int minSSTableIndex=0; 


extern kv_data * memtable; 
extern int* memtable_idx; 

void doMerge()
{
	signal(SIGUSR1, memtable_flush_handler); 
	//sleep(3); 
	//printf("key in memtable: %d\n", memtable[0].key); 

	//printf("This is Merge Process\n"); 
	while(1); 
}

void memtable_flush_handler()
{

	char sstname[30]; 
	sprintf(sstname, "sst/%d.sst", nextSSTableIndex); 
	
	//TODO: Lock should be held. 
	nextSSTableIndex ++; 

	FILE * sst = fopen(sstname, "w"); 
	performCompaction(memtable, sst); 
	fclose(sst); 
	
	printf("Compaction Completed"); 
	puts("\n");

	/*
	1. Sort memtable
	2. create sstable named 'nextSStableIndex.sst' in directory ./sst/
	3. increment nextSSTableIndex
	4. init memtable_idx to zero
	   */


}

void performCompaction(kv_data * memtable, FILE * sst)
{
	int i, j; 
	int order[MAX_MEMTABLE]; 

	// Sort memtable	
	memtable_sort(); 

	// Flush memtable to sstable
	memtable_flush(sst);

	//TODO: Lock should be acquired; 
	// Init memtable entry counter 
	*memtable_idx = 0; 

}

void memtable_flush(FILE * sst)
{
	int i; 
	// record the number of the entry in memtable 
	fprintf(sst, "%d\n", MAX_MEMTABLE); 
	// flush memtable to sstable
	for (i = 0; i < MAX_MEMTABLE; i ++)
	{
		fprintf(sst, "%d %d %s\n", memtable[i].idx, memtable[i].key, memtable[i].value); 
	}
}

void memtable_swap(int a, int b)
{
	int temp_key, temp_idx; 
	char temp_value[20]; 

	temp_key = memtable[b].key; 
	temp_idx = memtable[b].idx;
	strncpy(temp_value, memtable[b].value, VALSIZE); 

	memtable[b].key = memtable[a].key; 
	memtable[b].idx = memtable[a].idx;
	strncpy(memtable[b].value, memtable[a].value, VALSIZE); 

	memtable[a].key = temp_key;
	memtable[a].idx = temp_idx; 
	strncpy(memtable[a].value, temp_value, VALSIZE);
}
void memtable_sort ()
{
	if (memtable[0].key > memtable[1].key)
		memtable_swap(0, 1); 
	if (memtable[1].key > memtable[2].key)
		memtable_swap(1, 2); 
	if (memtable[0].key > memtable[1].key)
		memtable_swap(0, 1); 
}
