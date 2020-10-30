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

double parseVal(double f){		//modifica dei valori
	double temp, right;		//latitudine e longitudine
	int left;			//in un formato più leggibile
					
	temp=f/100;
	left=(int) temp;
	right=(temp-left)*(5.0/3);
	
	return (left+right);
}

void stampajson(Tmisure m, FILE *f){				//stampa su file json dei valori
	fprintf(f,"  {\n");
	fprintf(f,"    \"utc_time\": \"%s\",\n",m.utc_time);
	fprintf(f,"    \"latitude\": %.14f,\n" ,m.latitude);
	fprintf(f,"    \"longitude\": %.15f,\n",m.longitude);
	fprintf(f,"    \"altitude\": \"%s\",\n",m.altitude);
	fprintf(f,"    \"speed_knots\": \"%s\",\n",m.speed_knots);
	fprintf(f,"    \"course\": \"%s\"\n",m.course);
	fprintf(f,"  },\n");
}

//stampa su file csv dei valori
void stampacsv(Tmisure m, FILE *f){
	fprintf(f,"%s	%.14f	%.14f	%s	%s	%s	\n",m.utc_time,m.latitude,m.longitude,m.altitude,m.speed_knots,m.course);
}


int isGGA(char *s, int r){			//identifica le righe GGA
	if( s[3]=='G'){
		if( s[4]=='G'){
			if( s[5]=='A'){
				r=1;
    			}
    		}
    	}
    	return r;
}
int isGLL(char *s, int r){			//identifica le righe GLL
	if( s[3]=='G'){
		if( s[4]=='L'){
			if( s[5]=='L'){
    				r=2;
    			}
    		}
    	}
    	return r;
}
int isRMC(char *s, int r){			//identifica le righe RMC
	if( s[3]=='R'){
		if( s[4]=='M'){
			if( s[5]=='C'){
    				r=3;
    			}
    		}
    	}
    	return r;
}

void aggiorna(int r, char *d, FILE *f,int i){			//aggiorna i valori man mano che gli vengono passati	
	char *n="NaN";
	Tmisure m;
	if (r==1){						//r identifica se GGA,GLL,RMC
		char v[20][20];
		int a=0;
		int b=0;
		int c=0;
		
		while(a<70){
			c=0;
			while(d[a]!=','){
				v[b][c]=d[a];
				a++;
				c++;
			}
			v[b][c]='\0';
			if (b==1){
  				strcpy(m.utc_time,v[b]);
    			}
    			if (b==2){
    				m.latitude=strtod(v[b],NULL);
    				m.latitude=parseVal(m.latitude);  							
    			}
    			if (b==4){
  				m.longitude=strtod(v[b],NULL);
  				m.longitude=parseVal(m.longitude);
    			}
    			if (b==11){
  				strcpy(m.altitude,v[b]);
    			}
    			strcpy(m.speed_knots,n);
    			strcpy(m.course,n);
			a++;b++;
			
		
		}
		
		if (i==2){
			stampacsv(m,f);
		}
		if (i==1){
			stampajson(m,f);
		}
	
	}
	if (r==2){
		char v[20][20];
		int a=0;
		int b=0;
		int c=0;
		
		while(a<50){
			c=0;
			while(d[a]!=','){
				v[b][c]=d[a];
				a++;
				c++;
			}
			v[b][c]='\0';
			if (b==5){
				strcpy(m.utc_time,v[b]);
    			}
    			if (b==1){
    				m.latitude=strtod(v[b],NULL);
    				m.latitude=parseVal(m.latitude);  							
    			}
    			if (b==3){
  				m.longitude=strtod(v[b],NULL);
  				m.longitude=parseVal(m.longitude);
    			}
    			strcpy(m.altitude,n);
    			strcpy(m.speed_knots,n);
    			strcpy(m.course,n);
			a++;b++;
			
		
		}
		if (i==2){
			stampacsv(m,f);
		}
		if (i==1){
			stampajson(m,f);
		}
	}
	if (r==3){
		char v[20][20];
		int a=0;
		int b=0;
		int c=0;
		
		while(a<50){
			c=0;
			while(d[a]!=','){
				v[b][c]=d[a];
				a++;
				c++;
			}
			v[b][c]='\0';
			
			if (b==1){
  				strcpy(m.utc_time,v[b]);
    			}
    			if (b==3){
    				m.latitude=strtod(v[b],NULL);
    				m.latitude=parseVal(m.latitude);  							
    			}
    			if (b==5){
  				m.longitude=strtod(v[b],NULL);
  				m.longitude=parseVal(m.longitude);
    			}
    			if (b==7){
  				strcpy(m.speed_knots,v[b]);
    			}
    			if (b==8){
  				strcpy(m.course,v[b]);
    			}
    			strcpy(m.altitude,n);
    			a++;b++;
  		}
  		if (i==2){
			stampacsv(m,f);
		}
		if (i==1){
			stampajson(m,f);
		}
	}
	
}

int isjson(char *t){		//identifica se il config.json è settato su cvs
  	int r=0;
	if (strcmp(t,json)==0){
		r=1;
		return r;
	}


}

int iscsv(char *t){		//identifica se il config.json è settato su cvs
  	int r=0;
	if (strcmp(t,csv)==0){
		r=2;
		return r;
	}


}
void openf(char *fa,char *fb, int t){		//apre i file e lancia aggiorna e poi li chiude
	FILE *fu, *ft;
	char buf[200];
  	char *res;
  	int r=0;
	if((fu=fopen(fa, "rt"))==NULL) {
		printf("Errore nell'apertura del file'%s\n",fa);
		exit(1);
	}
	if((ft=fopen(fb, "w"))==NULL) {
		printf("Errore nell'apertura del file'%s\n",fb);
		exit(1);
	}
	
	if (t==1){
		fprintf(ft, "[\n");
	}
	if (t==2){
		fprintf(ft, "utc_time	latitude	longitude	altitude	speed_knots	course\n");
	}
	while(!feof(fu)){
		res=fgets(buf, 200, fu);
    		if( res==NULL ){
      			break;
      		}
      		r=0;
      		r=isGGA(buf,r);
    		r=isGLL(buf,r);
    		r=isRMC(buf,r);
    		aggiorna(r,buf,ft,t);	
 	}
	if (t==1){
		fprintf(ft, "]\n");
	}
	fclose(fu);
	fclose(ft);
}


char *matchubx(char *n){			//ricostruisce nome file ubx
	char i[100]="inputs/";
	char *u=".ubx";
	char a[100];
	
	strcat(i,n);
	strcat(i,u);

}

char *matchtype(char *n, int t){		//ricostruisce nome file di output
	char i[100]="outputs/";
	char *j1="json/";
	char *j2=".json";
	char *c1="csv/";
	char *c2=".csv";
	char a[100];
	
	
	if (t==1){
		strcat(i,j1);
		strcat(i,n);
		strcat(i,j2);
	}
	if (t==2){
		strcat(i,c1);
		strcat(i,n);
		strcat(i,c2);
	}
}
