#ifdef WIN32

#include <stdio.h>

#else
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <math.h>

#endif
#include "crate_lib.h"
#include "lecroy_2249A.h"




short Linit(char* ip){			/*function to initialize CAMAC*/
	short crate_id;			
	char res[33];
	res[32]='\0';
	if ( ( crate_id=CROPEN(ip) ) < 0){		/*CROPEN is Library Function to open connection to CAMAC, negative output is error*/
		printf("Error: couldn't find the specific device at ip: %s\n",ip);
		return -1;
	}
	
	if ( CCCZ(crate_id)<0 ) {	/*Initialize dataway, negative output is error*/
		printf("Error: couldn't initialize DATA WAY\n");
		return -1;
	}

	if ( CCCC(crate_id)<0 ) {	/*Generate Crate Clear, negative output is error*/
		printf("Error: couldn't send clear signal\n");
		return -1;
       	}

	printf("DATA WAY initialized\n");
	//-----------------This Lines are used to work with LAM only
	//LdisCombo(crate_id,1);//Disable combo 1
	//LdisCombo(crate_id,2);//Disable combo 2
	//---------------- This Lines MUST be enabled to use combo
	LenCombo(crate_id,1);//Enable combo 1
	Lcfreset(crate_id,1);//Combo 1 Reset Event Counter
	Lcack(crate_id,1);//Combo 1 acknowledgement
	return crate_id;		/* if successfull, CAMAC assigned ID*/
}

short LenCombo(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_enablecombo %d 0\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Enable combo
		printf("Error: couldn't enable combo %d\n",combo);
		return -1;
	}
printf("Combo %d Enabled\n",combo);
return 0;
}

short LdisCombo(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_enablecombo %d 1\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Disable combo
		printf("Error: couldn't disable combo %d\n",combo);
		return -1;
	}
	printf("Combo %d Disabled\n",combo);
return 0;
}

short Lcack(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_cack %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Combo acknowledgement
		printf("Error: couldn't acknowledge combo %d\n",combo);
		return -1;
	}	
printf("Combo %d acknowledgement\n",combo);
return 0;
}

short Lfcack(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_cack %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Combo acknowledgement
		printf("Error: couldn't acknowledge combo %d\n",combo);
		return -1;
	}	
return 0;
}

int Lcreset(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_getcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get Combo Counter Value
	       printf("Error: couldn't get combo %d counter value\n",combo);
			return -1;
	}
int counts=atoi(res);
sprintf(command,"nim_resetcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Combo Reset Event Counter
	       printf("Error: couldn't reset combo %d counter\n",combo);
			return -1;
	}
	printf("Combo %d Event Counter Reset\n",combo);
return counts;
}

short Lcfreset(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_resetcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Combo Reset Event Counter
	       printf("Error: couldn't reset combo %d counter\n",combo);
			return -1;
	}
	printf("Combo %d Event Counter Reset\n",combo);
return 0;
}

int LCevent(short CID,short combo){
char res[33];
res[32]='\0';
char command[32];
sprintf(command,"nim_getcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get Combo Counter Value
	       printf("Error: couldn't get combo %d counter value\n",combo);
			return -1;
	}
int counts=atoi(res);
return counts;
}

short Lsetcount(short CID,short in,short pol,short on_off,short ext){
// sets input counters, in is input 1 or 3, pol is nim polarity, ext is enable or disable external reset
//on is on_off=1, off is 0
char res[33];
res[32]='\0';
char command[32];
sprintf(command,"nim_setievcnt %d %d %d %d\r",in,on_off,pol,ext);
if (on_off==1){
	if ( CMDSR(CID,command,res,32)<0 ) {	       //settings counter
	       printf("Error: couldn't set counter %d\n",in);
			return -1;
	}
	printf("Counter %d is ON with ",in);
	if (pol==0) printf("NIM Polarity HI to LOW (0)");
		else printf("NIM Polarity LOW to HI (1)");
	if (ext==1) printf(" and external reset enabled\n");
		else printf(" and external reset disabled\n");
	return 0;
	}
else {
	if ( CMDSR(CID,command,res,32)<0 ) {	       //turning off counter
	       printf("Error: couldn't turn off counter %d\n",in);
			return -1;
	}
	printf("Counter %d is OFF\n ",in);
	return 0;	
	}
}

short Lfsetcount(short CID,short in,short pol){
// sets input counters, in is input 1 or 3, pol is nim polarity
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_setievcnt %d 1 %d 0\r",in,pol);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //settings counter
	       printf("Error: couldn't set counter %d\n",in);
	return -1;
	}
return 0;
}

int LINcount(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_geticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get IN Counter Value
	       printf("Error: couldn't get in %d counter value\n",in);
			return -1;
	}
int ok,counts;
sscanf(res,"%d %d",&ok,&counts);
return counts;
}

int LINreset(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_geticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get IN Counter Value
	       printf("Error: couldn't get in %d counter value\n",in);
			return -1;
	}
int ok,counts;
sscanf(res,"%d %d",&ok,&counts);
sprintf(command,"nim_reseticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //IN Reset Event Counter
	       printf("Error: couldn't reset IN %d counter\n",in);
			return -1;
	}
	printf("IN %d Event Counter Reset\n",in);
return counts;
}

int LINRFreset(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_geticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get IN Counter Value
	       printf("Error: couldn't get in %d counter value\n",in);
			return -1;
	}
int ok,counts;
sscanf(res,"%d %d",&ok,&counts);
sprintf(command,"nim_reseticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //IN Reset Event Counter
	       printf("Error: couldn't reset IN %d counter\n",in);
			return -1;
	}
return counts;
}

short LINfreset(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_reseticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //IN Reset Event Counter
	       printf("Error: couldn't reset IN %d counter\n",in);
			return -1;
	}
	printf("IN %d Event Counter Reset\n",in);
return 0;
}

short LenableLAM(short CID, short card){
	short res;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=26;			/*Function F(26) of 2249A to Enable LAM*/
	op.A=1;				/*Needs any subaddress*/
	op.N=card;			/*Number of 2249A SLOT*/
	res=CFSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't enable LAM\n");
		return -1;
	}
	printf("LAM initialized on card %d\n",card);
	return 0;
}

short LdisableLAM(short CID, short card){
	short res;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=24;			/*Function F(24) of 2249A to Disable LAM*/
	op.A=1;				/*Needs any subaddress*/
	op.N=card;			/*Number of 2249A SLOT*/
	res=CFSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't disable LAM\n");
		return -1;
	}
	printf("LAM disabled on card %d\n",card);
	return 0;
}

short Lclose(short crate_id){
	
	if ( (CRCLOSE(crate_id) ) < 0){		/*Close connection to CAMAC*/
		printf("Error: can't close the specific device\n");
		return -1;
	}
	
	return 1;
}

short Lread(short CID, short card, short add){		/*This function is specifically designed for this CAMAC module*/
	
	short res;
	short dato;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=2;				/*Function F(0) of 2249A to Read Registers erasing it and LAM in A(11)*/
	op.N=card;			/*Number of 2249A SLOT*/
	op.A=add;			/*Address of Register. Correspondes to ANALOG Input NUMBER in MODULE*/
	res=CFSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't read module\n");
		return -1;
	}
	dato=op.DATA;			/*gets data response from F(0) CAMAC command, this is register data. It is saved inside op STRUCTURE*/
//	op.F=9;				/*Clears MODULE and LAM (look at me)*/
//	CFSA(CID,&op);			/*Executes CAMAC command F(9)*/
//	if (res<0){			/*Error responses are negative*/
//		printf("Error: CFSA command not executed\n");
//		return -1;
//	}
	op.A=11;			/* A(11) clears and Module */
	op.F=2;				/*Clears MODULE and LAM (look at me)*/
	res=CFSA(CID,&op);			/*Executes CAMAC command F(2)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't clear module and LAM\n");
		return -1;
	}
		
	return dato;			/*output of program function Lread is dato with is DATA from register*/
}

float Lreadf(short CID, short card, short add){		/*it is exactly the same function than before, BUT it shows data without overflow bit*/
	
	short res;
	int dato;
	CRATE_OP op;
	op.F=2;				/*Function F(2) of 2249A to Read Registers WITHOUT erasing it*/
	op.N=card;
	op.A=add;
	res=CFSA(CID,&op);
	if (res<0){
		printf("Error: Couldn't read module\n");
		return -1;
	}
	dato=op.DATA;
//	op.F=9;				/*Executes CAMAC command F(9) - clear module and LAM*/
//	CFSA(CID,&op);
//	if (res<0){
//		printf("Error: CFSA command not executed\n");
//		return -1;
//	}
		
	
	op.A=11;			/* A(11) clears and Module */
	op.F=2;				/*Clears MODULE and LAM (look at me)*/
	CFSA(CID,&op);			/*Executes CAMAC command F(2)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't clear module and LAM\n");
		return -1;
	}
	return dato&(1023)*256/1024;		/*deletes the overflow bit to see the data correctly*/
	
}


short Lreadall(short CID){		
	short res;
	short i;
	CRATE_OP op;
	op.F=2;				/*Function F(2) of 2249A to Read Registers WITHOUT erasing it*/
	op.N=QDCn1;
		
	for (i=3; i<12;i++)
	{		//para todos los canales
		op.A=i;
		res=CFSA(CID,&op);
		if (res<0){
			printf("Error: Couldn't read module\n");
			return -1;
		}
		datos[i-3]=op.DATA;
	}
	op.N=QDCn2;
	for (i=4; i<12;i++)
	{		//para todos los canales
		op.A=i;
		res=CFSA(CID,&op);
		if (res<0){
			printf("Error: Couldn't read module\n");
			return -1;
		}
		datos[i+5]=op.DATA;
	}
	return 0;		/*deletes the overflow bit to see the data correctly*/
}



double Lgen(short CID,int width,double period,short polar,short on_off){
/*This function SETS special output of CAMAC Controller, OUTPUT 1 is Pulse Generator*/
/*If on_off=1 generator will be ON, if on_off=0, generator will be off*/
/*POLAR 0 means Reverse Polarity (HI to LOW), POLAR 1 means Direct Polarity (LOW to HI)*/
/*width MUST be set in multiples of 88 nS up to 616 ns*/
char res [33];
res[32]='\0';
int mult_width;
int mult_per;
if (on_off==1){
	mult_width = width/88;
	int mod_width = width%88;
		if (width>88 && width<616 && mod_width != 0){
			printf("Warning! you should set pulse time of width in multiples of 88 nS\n");
			printf("Pulse Width Setting: %d[nS]\tUser Seting:%d[nS]\tError is:%d[nS]\n",mult_width*88,width,mod_width);}
		else if (width>=88 && width<=616 && mod_width == 0){
			printf("Pulse Width Setting: %d[nS]\n",width);}
		else if (width>616){mult_width=616/88;
			printf("Warning! maximum value for width is 616 nS\nPulse Width Setting: 616[nS]\tUser Seting:%d[nS]\tError is:%d[nS]\n",width,width-616);}
		else {mult_width=1;
			printf("Warning! minimum value for width is 88 nS\nPulse Width Setting: 88[nS]\tUser Seting:%d[nS]\tError is:%d[nS]\n",width,88-width);}
/*Period MUST be set in steps of 93.1uS, from 186.2uS up to 95325.2*/
	mult_per = (int)floor(period/93.1);
	double mod_per = fmod(period,93.1);
		if (period>186.2 && period<95325.2 && mod_per != 0.0){
			printf("Warning! you should set pulse period in multiples of 93.1 uS but more than 186.2 uS\n");
			printf("Pulse Period Setting: %5.2f[uS]\tUser Seting:%5.2f[uS]\tError is:%5.2f[uS]\n",((double)mult_per)*93.1,period,mod_per);}
		else if (period>=186.2 && period<=95325.2 && mod_per == 0.0){
			printf("Pulse Period Setting: %5.2f[uS]\n",period);}
		else if (period<186.2){mult_per=2;
			printf("Warning! Minimum Period Value is 186.2[uS]\n");
			printf("Pulse Period Setting: 186.2[uS]\tUser Seting:%5.2f[uS]\tError is:%5.2f[uS]\n",period,186.2-period);}
		else {mult_per=(int)(95325.2/93.1);
			printf("Warning! Maximum Period Value is 95325.2[uS]\n");
			printf("Pulse Period Setting: 95325.2[uS]\tUser Seting:%5.2f[uS]\tError is:%5.2f[uS]\n",period,period-95325.2);}
	if (polar==0) printf("NIM Polarity is HI to LOW (0)\n");
	else printf("NIM Polarity is LOW to HI (1)\n");
	char command[32];
/*nim_setpulse sets pulse, first number is period second number is width third number is NIM polarization*/
	sprintf(command,"nim_setpulse %d %d %d\r",mult_per,mult_width,polar);
		if ( (CMDSR(CID,command,res,32))<0 ){	
			printf("couldn't set nim pulse\n");				
			return -1;						
		}								
	return ((double)mult_per)*93.1;
	}
else	{printf("Pulse generator will be turn off, previous settings in generator are:\n");
	if ( (CMDSR(CID,"nim_getpulse\r",res,32))<0 ){		/*getting previous settings*/
			printf("couldn't get previous settings\n");
			return -1;
		}
	int ok,pol;
	sscanf(res,"%d %d %d %d",&ok,&mult_per,&mult_width,&pol);
	if (ok==0 && mult_per==0 && mult_width==0 && pol==0)
		{printf("Generator was already off\n");
		return 0;
		}
	else printf("Pulse Period: %5.2f[uS]\tPulse Width: %d[nS]\tPolarity:%d\n",((double)mult_per)*93.1,mult_width*88,pol);
	if ( (CMDSR(CID,"nim_pulseoff\r",res,32))<0 ){		/*turn off pulse*/
			printf("couldn't turn off pulse\n");
			return -1;
		}
	return 0;
	}
}
/// THIS LISR WORKS WITH LAM
/*
void *Lisr(short CID, short irq_type, unsigned int irq_data){
	switch (irq_type){
		case LAM_INT:
			{
			if (irq_data==QDCx){
			Lreadall(CID);
			}	
			LACK(CID);
			break;
			}
	}
	pthread_exit(irq_data);
}
*/

/// THIS LISR WORKS WITH COMBO
void *Lisr(short CID, short irq_type, unsigned int irq_data){
	switch (irq_type){
		case COMBO_INT:
			{
			nanosleep(60000,NULL);
			Lreadall(CID);
			break;
			}
	}
	pthread_exit(irq_data);
}

