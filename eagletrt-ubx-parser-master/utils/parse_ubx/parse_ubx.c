#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "parse_ubx.h"

//restituisce il nome del file ubx da analizzare
char *matchubx(const char *path){			
	char input[1024]="inputs/";
	char *ubx=".ubx";
	char *path_ubx;
	
	strcat(input,path);
	strcat(input,ubx);
}

//modifica i valori in un formato più leggibile per Google Earth
double parseVal(double value){		
	double temp, right;		
	int left;			
					
	temp=value/100;
	left=(int) temp;
	right=(temp-left)*(5.0/3);
	
	return (left+right);
}

//restituisce la tipologia di messaggio
message_name_t get_line_message(char *buf){			//identifica le righe GGA
	int i=0;
	char line[10];
	while(buf[i]!=','){
		
		line[i]=buf[i];
		i++;
	}
	line[i]='\0';
	
	if( strcmp(line,GNGGA)==0){
		return GGA;
    	}
    	if( strcmp(line,GNGLL)==0){
		return GLL;
    	}
    	if( strcmp(line,GNRMC)==0){
		return RMC;
    	}
    	return UNKNOWN;
}


//restituisce i valori dei vari messaggi
message_t extract_message_GGA(const char *line){
	char *nan="NaN";
	message_t m;
	int index_line=0;
	int type_message=0;
	int index_message=0;
	char line_arr[100][100];
	
	while(index_line<100){
		index_message=0;
		while(line[index_line]!=','){
			line_arr[type_message][index_message]=line[index_line];
			index_line++;
			index_message++;
		}
		line_arr[type_message][index_message]='\0';
		
		if (type_message==1){
			strcpy(m.utc_time,line_arr[type_message]);
		}
		if (type_message==2){
			m.latitude=strtod(line_arr[type_message],NULL);
			m.latitude=parseVal(m.latitude);  							
		}
		if (type_message==4){
			m.longitude=strtod(line_arr[type_message],NULL);
			m.longitude=parseVal(m.longitude);
		}
		if (type_message==11){
			strcpy(m.altitude,line_arr[type_message]);
		}
		strcpy(m.speed_knots,nan);
		strcpy(m.course,nan);
		index_line++;type_message++;
		
	
	}
	return m;
}

message_t extract_message_GLL(const char *line){
	char *nan="NaN";
	message_t m;
	int index_line=0;
	int type_message=0;
	int index_message=0;
	char line_arr[100][100];
	
	while(index_line<70){
		index_message=0;
		while(line[index_line]!=','){
			line_arr[type_message][index_message]=line[index_line];
			index_line++;
			index_message++;
		}
		line_arr[type_message][index_message]='\0';
		if (type_message==5){
			strcpy(m.utc_time,line_arr[type_message]);
		}
		if (type_message==1){
			m.latitude=strtod(line_arr[type_message],NULL);
			m.latitude=parseVal(m.latitude);  							
		}
		if (type_message==3){
			m.longitude=strtod(line_arr[type_message],NULL);
			m.longitude=parseVal(m.longitude);
		}
		strcpy(m.altitude,nan);
		strcpy(m.speed_knots,nan);
		strcpy(m.course,nan);
		index_line++;type_message++;
		
	
	}
	return m;

}

message_t extract_message_RMC(const char *line){
	char *nan="NaN";
	message_t m;
	int index_line=0;
	int type_message=0;
	int index_message=0;
	char line_arr[100][100];
	
	while(index_line<70){
		index_message=0;
		while(line[index_line]!=','){
			line_arr[type_message][index_message]=line[index_line];
			index_line++;
			index_message++;
		}
		line_arr[type_message][index_message]='\0';
		if (type_message==1){
			strcpy(m.utc_time,line_arr[type_message]);
		}
		if (type_message==3){
			m.latitude=strtod(line_arr[type_message],NULL);
			m.latitude=parseVal(m.latitude);  							
		}
		if (type_message==5){
			m.longitude=strtod(line_arr[type_message],NULL);
			m.longitude=parseVal(m.longitude);
		}
		if (type_message==7){
			strcpy(m.speed_knots,line_arr[type_message]);
		}
		if (type_message==8){
			strcpy(m.course,line_arr[type_message]);
		}
		strcpy(m.altitude,nan);
		index_line++;type_message++;
		
	
	}
	return m;

}

//filtra i messaggi nei formati voluti dividendoli in variabili
void parse_ubx(char* path, message_t* messages){
	FILE *file_ubx;
	char buf[200],ubx_path[1024];
  	char *res;
  	int n_line=0;
  	int n_parse_line=0;
  	message_name_t name;
  	
  	strcpy(ubx_path,matchubx(path));
	if((file_ubx=fopen(ubx_path, "rt"))==NULL) {
		printf("Errore nell'apertura del file'%s\n",ubx_path);
		exit(1);
	}
	
	while(!feof(file_ubx)){
		res=fgets(buf, 200, file_ubx);
    		if( res==NULL ){
      			break;
      		}
      		name=get_line_message(buf);
      		if (name==GGA){
      			messages[n_parse_line]=extract_message_GGA(buf);
      			n_parse_line++;
      		}
      		if (name==GLL){
      			messages[n_parse_line]=extract_message_GLL(buf);
      			n_parse_line++;
      		}
      		if (name==RMC){
      			messages[n_parse_line]=extract_message_RMC(buf);
      			n_parse_line++;
      		}
    		n_line++;      		
 	}
	
	fclose(file_ubx);
}
