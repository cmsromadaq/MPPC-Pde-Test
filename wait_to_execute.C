#include <iostream>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{	
	int howmuch;
	if(argc < 3){
		printf("\tingresar parametros, tiempo a esperar e instruccion");
		return 1;}
	howmuch =  atoi(argv[1]);
	char  command[50] = argv[2];
	int t1=0;
	while((t1)!=howmuch)
	{	
		t1++;
		printf("\tPMT is not ready yet, elapsed time is: %.1d\r",(t1));
		fflush(NULL);
		usleep(1000000);
	}	
	printf("\n\nPMT is ready!!!!!!!!!!\n\n");
	
	system(command.c_string());
	return 0;
}

