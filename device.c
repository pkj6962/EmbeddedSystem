#include "global.h"

int device_read_data_key()
{
	// Temporary code before implementing device handling code. 
	int key; 
	printf("key: "); 
	puts("\n");
	scanf("%d", &key); 

	return key; 
}

void device_read_data_value (char * value)
{
	char temp[20]; 

	printf("value: "); 
	scanf("%s", temp); 

	strncpy(value, temp, 20);
}
