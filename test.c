#include <stdio.h>

int main(void)
{
	int idx = 0; 
	char name[30];
	sprintf(name, "./sst/%d.sst", idx);

	FILE * fd = fopen(name, "w"); 
	if (fd == NULL)
	{
		printf("error\n"); 
	}


}
