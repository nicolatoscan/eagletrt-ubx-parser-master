#ifndef FILE_H_
#define FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


const char csv[10]="CSV";		//char per identificazione dal config.json
const char json[10]="JSON";

typedef struct Tmisure{		//struct per i valori
	char utc_time[20];
	double latitude;
	double longitude;
	char altitude[20];
	char speed_knots[20];
	char course[20];

}Tmisure;

double parseVal(double f);
void stampajson(Tmisure m, FILE *f);
void stampacsv(Tmisure m, FILE *f);
int isGGA(char *s, int r);
int isGLL(char *s, int r);
int isRMC(char *s, int r);
void aggiorna(int r, char *d, FILE *f,int i);
int isjson(const char *t);
int iscsv(const char *t);
void openf(char *fa,char *fb, int t);
char *matchubx(const char *n);
char *matchtype(const char *n, int t);

#endif
