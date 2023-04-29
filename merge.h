#include <stdio.h>

void doMerge();

void memtable_flush_handler();

void performCompaction(); 

void memtable_flush();
void memtable_sort();
void memtable_swap(int a, int b);
