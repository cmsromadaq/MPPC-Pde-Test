#include "crate_lib.h"
#include "lecroy_2249A.h" 
#include <time.h>
#include <math.h>

///////////////////////////////////////////////********************++++++++++++++++++*******************
////////
////////	ESTE CODIGO MANEJA EL CAMAC PARA LA MEDICION DE MPPC
///////
/////////**********************************************************++++++++++++++++++*******************
/////////_________________________________________________________________________________________________


//llamar a funcion asi..  ./spectra num_event filename.dat ej:  ./spectra 10000 ejemplo.dat

int main(int argc,char *argv[]){
	
short CID, res;
char cmd_res[33];
char filename[50];
int i;
int events;
int wait_time;
int twtf=0;
// LAM is not used in combo
//int LAM;
float percent,remainingtime0,remainingtime1;
CRATE_INFO crInfo;
FILE *outfile;
cmd_res[32]='\0';
	
	if (argc>4){		/*There are not arguments for input in this executable file*/
		printf("you just can insert only three parameter \n\
			spectra [<num_events>] [<filename>] [<time_delay>]\n"); 
		return 0;
	}
	else if(argc>3){
		sprintf(filename,"%s",argv[2]);	
		events=atoi(argv[1]);
		wait_time = atoi(argv[3]);
	}
	else if(argc>2){
		sprintf(filename,"%s",argv[2]);	
		events=atoi(argv[1]);
		wait_time = 0;
		
	} 
	else if(argc>1){
		sprintf(filename,"datos.txt");
		events=atoi(argv[1]);
		wait_time = 0;
	}
	
	else {
		sprintf(filename,"datos.txt");
		events=DEFAULT_EVENTS;
		wait_time = 0;
	}
	while((twtf)!=wait_time)
	{	
		twtf++;
		printf("\tMPPC is not ready yet, elapsed time is: %.1d\r",(twtf));
		fflush(NULL);
		usleep(1000000);
	}
	//putchar('\n');
	if( !(outfile=fopen(filename,"wr")) ){	/*If file can't be opened for writting, fopen returns NULL, that's why if is true if fopen is NULL*/
			printf("couldn't open file: %s\n",filename);
			return 0;
	}
	
	if ( (CID=Linit("192.168.1.102"))<0 ){		/*Linit needs CAMAC IP. CID is return from function Linit in Lecroy 2249A driver, output negative is error*/
			printf("couldn't initialize device at ip: 192.168.1.102\n");
			return 0;
	}


	printf("CID: %d\n",CID);		/*if Linit is successful, it returns CAMAC assigned ID*/
// LAM IS DISABLED IN COMBO
/*
	LenableLAM(CID,QDCn1);
	LACK(CID);
	LenableLAM(CID,QDCn2);
	LACK(CID);
*/

	LdisableLAM(CID,QDCn1);
	LdisableLAM(CID,QDCn2);

	printf("Event to be measured: %d\n",events);
	int j=0,counter=1;
	fprintf(outfile,"pmt\tch1\tch2\tch3\tch4\tch5\tch6\tch7\tch8\tch9\tch10\tch11\tch12\tch13\tch14\tch15\tch16 \n");
	int t0=(int)time(NULL);
	while(counter<events){
		res=CRIRQ(CID, Lisr);			//// necesario, entrega estado de la comunicacion, inicia una hebra nueva
		CRGET(CID,&crInfo);				//// necesario, entrega informacion de la comuniacion
		percent=(float)counter/events*100;
		remainingtime1=(100-percent)/(percent/((int)time(NULL)-t0));
		remainingtime0=(1-0.7)*remainingtime1+0.7*remainingtime0;
		printf("\t%3.0f%%...complete\tremaining time:%3.1f\tcounts:%d\r",percent,remainingtime0,counter);
		fflush(NULL);
// ----------------------------- ESTAS LINEAS FUNCIONAN CON LAM
/*
		if (LAM==QDCx){
				for(j=0;j<17;j++){
					fprintf(outfile,"%d\t",datos[j]);
					}
					fprintf(outfile,"\n");	
			
			counter++;
			}
		pthread_join(crInfo.irq_tid,&LAM);*/
// ----------------------------- ESTAS LINEAS (ARRIBA) FUNCIONAN CON LAM
		/// entrega en data la informacion del canal, espera la informacion de la hebra
		for(j=0;j<17;j++){
					fprintf(outfile,"%d\t",datos[j]);
					}
		fprintf(outfile,"\n");
		pthread_join(crInfo.irq_tid,NULL);
		Lfcack(CID,1);
		counter++;
	}
	float rate=events/(float)((int)time(NULL)-t0);
	putchar('\n');	
	/*the remaining lines close the files*/	

	if( fclose(outfile)){	/*fclose returns EOF if it fails. In C every value BUT 0 are TRUE. Closing datosoff*/
		printf("couldn't close file: %s\n",filename);
		return 1;
	}
	printf("Elapsed time: %d Average Adquisition Rate: %5.2f\n",(int)time(NULL)-t0,rate);
//	system("ssh 192.168.1.202 'mpg123 Desktop/app/sonido1.mp3'");

	/*this lines will close connection with CAMAC, it needs CID*/
	if ( (Lclose(CID))<0 ){
			printf("couldn't close connection\n");
			return 1;	}	
	return 0;
}
