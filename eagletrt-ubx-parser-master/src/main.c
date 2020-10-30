#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "file.h"


int main(){
	
	FILE *fj;			
	char buffer[150], type[10], *riga, fo[100],ff[100];
  	int a=0;
  	int t,j,c,x;
  	char *p="]\n}";	
	
    	printf("Inizio scansione\n");
	fj = fopen("config.json","r");
	fread(buffer, 150, 1, fj);
	fclose(fj);
	
	riga = strtok(buffer, " \"");
  	while (riga!=NULL){
  		x=strcmp(p,riga);
    		if (x==-10){
    			printf("Fine\n");
    			exit(0);
      		}	
  		if (a==3){
  			strcpy(type,riga);
  			j=isjson(type);
  			c=iscsv(type);
  			if(j==1){
  				t=j;
  			}
  			if(c==2){
  				t=c;
  			}
    		}
    		if ((a>7)&&((a%2)==0)){
  			strcpy(fo,matchubx(riga));
  			strcpy(ff,matchtype(riga,t));
  			openf(fo,ff,t);	
    		}
    		if(a==12){
    			x=strcmp(p,riga);
    		}
    		
    		riga = strtok (NULL, " \"");
    		a++;
    	}	
	

}
