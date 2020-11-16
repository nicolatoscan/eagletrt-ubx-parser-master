#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "parse_ubx.h"
#include "../log_utils/log_utils.h"

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
message_name_t get_line_message(char *buf){			//identifica le righe GGA GLL RMC
	int i=0;
	char line[10];
	
	if (buf[i]!='$'){
		return ERR;
	}
	while(buf[i]!=','){
		if ((i>0)&&((buf[i]>90)||(buf[i]<65))){
			return ERR;
		}
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
message_t set_void_message(){
	char *nan="NaN";
	message_t m;
		
	strcpy(m.utc_time,nan);
	m.latitude=0;
	m.longitude=0;
	strcpy(m.speed_knots,nan);
	strcpy(m.course,nan);
	strcpy(m.altitude,nan);
	return m;

}

//filtra i messaggi nei formati voluti dividendoli in variabili
void parse_ubx(char* path, message_t* messages, int limit_warning){
	FILE *file_ubx;
	char buf[300],ubx_path[1024];
  	char *res;
  	int n_line=1;
  	int n_parse_line=0;
  	int n_warning=0;
  	message_name_t name;
  	
  	strcpy(ubx_path,matchubx(path));
	if((file_ubx=fopen(ubx_path, "rt"))==NULL) {
		logError("Errore nell'aperturna del file ubx di input");
		exit(1);
	}
	
	while(!feof(file_ubx)){
		int n=0;
		res=fgets(buf, 300, file_ubx);
    		if( res==NULL ){
      			break;
      		}
      		name=get_line_message(buf);
      		if (name==ERR){
      			n_warning++;
      			if (n_warning<limit_warning){
      				logWarning("Errore nel file ubx");
      				printf("Line %d: %s\n", n_line, buf);
      			}
      			messages[n_parse_line]=set_void_message();
      		}
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
	if (n_warning>limit_warning){
      				printf("e altri %d errori in questo file\n",(n_warning-limit_warning));
      			}
	fclose(file_ubx);
}
