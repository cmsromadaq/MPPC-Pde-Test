#ifndef LECROY_2249A
#define LECROY_2249A

#include <stdio.h>
#include "crate_lib.h"
#define QDCx	0x280000 //esta en el slot 6
#define QDCn1	20 //esta en el slot x
#define QDCn2	22 //esta en el slot x 
#define address 1 // donde leo la direccion
#define DEFAULT_EVENTS 1000

/*
typedef struct AllData{
	int ch0;
	int ch1;
	int ch2;
	int ch3;
	int ch4;
	int ch5;
	int ch6;
	int ch7;
	int ch8;
	int ch9;	
	int ch10;
	int ch11;
}AllData;
*/

int datos[17];

short Lreadall(short CID);
short Linit(char* ip);
short Lclose(short crate_id);
short Lread(short CID, short card, short add);
float Lreadf(short CID, short card, short add);
short LenableLAM(short CID, short card);
short LdisableLAM(short CID, short card);
void *Lisr(short CID, short irq_type, unsigned int irq_data);
void *Lisrall(short CID, short irq_type, unsigned int irq_data);
double Lgen(short CID,int width,double period,short polar,short on_off);
short LenCombo(short CID,short combo);
short LdisCombo(short CID,short combo);
short Lcack(short CID,short combo);
int Lcreset(short CID,short combo);
short Lcfreset(short CID,short combo);
int LCevent(short CID,short combo);
short Lsetcount(short CID,short in,short pol,short on_off,short ext);
short Lfsetcount(short CID,short in,short pol);
int LINcount(short CID,short in);
int LINreset(short CID,short in);
short LINfreset(short CID,short in);
int LINRFreset(short CID,short in);
short Lfcack(short CID,short combo);

#endif


