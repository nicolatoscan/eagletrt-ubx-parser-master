#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "generate_output.h"
#include "../log_utils/log_utils.h"

//ricostruisce nome file di output
char *matchtype(const char *path, parse_t type){		
	char output[1024]="outputs/";
	char *j1="json/";
	char *j2=".json";
	char *c1="csv/";
	char *c2=".csv";
	
	if (type==JSON){
		strcat(output,j1);
		strcat(output,path);
		strcat(output,j2);
	}
	if (type==CSV){
		strcat(output,c1);
		strcat(output,path);
		strcat(output,c2);
	}
}

//stampa i valori nei file di output json
void print_json(FILE *json_file, const message_t messages){
	fprintf(json_file,"  {\n");
	fprintf(json_file,"    \"utc_time\": \"%s\",\n",messages.utc_time);
	fprintf(json_file,"    \"latitude\": %.14f,\n" ,messages.latitude);
	fprintf(json_file,"    \"longitude\": %.15f,\n",messages.longitude);
	fprintf(json_file,"    \"altitude\": \"%s\",\n",messages.altitude);
	fprintf(json_file,"    \"speed_knots\": \"%s\",\n",messages.speed_knots);
	fprintf(json_file,"    \"course\": \"%s\"\n",messages.course);
	fprintf(json_file,"  },\n");
}

//inizializza la stampa dei file json
void generate_output_json(const char *output_path, const message_t* messages){
	FILE *json_file;
	int index=0;
	
	if((json_file=fopen(output_path, "w"))==NULL) {
		logError("Errore nell'apertura del file json di output");
		exit(1);
	}
	fprintf(json_file, "[\n");
	
	while(messages[index].latitude!=0){
		print_json(json_file,messages[index]);
		index++;
	}
	
	fprintf(json_file, "]\n");
	fclose(json_file);
}

//stampa i valori nei file di output csv
void print_csv(FILE *csv_file, const message_t messages){
	fprintf(csv_file,"%s	%.14f	%.14f	%s	%s	%s	\n",messages.utc_time,messages.latitude,messages.longitude,messages.altitude,messages.speed_knots,messages.course);
}

//inizializza la stampa dei file csv
void generate_output_csv(const char* output_path, const message_t* messages){
	FILE *csv_file;
	int index=0;
	if((csv_file=fopen(output_path, "w"))==NULL) {
		logError("Errore nell'apertura del file csv di output");
		exit(1);
		fprintf(csv_file, "utc_time	latitude	longitude	altitude	speed_knots	course\n");		
	}
	
	while(messages[index].latitude!=0){
		print_csv(csv_file,messages[index]);
		index++;
	}
	
	fclose(csv_file);
	
}

//fa partire le generazioni dei file di output
void generate_output(const char *path, const message_t* messages, const parse_t type){
	char output_path[1024];
	if (type==JSON){
		strcpy(output_path,matchtype(path,type));
		generate_output_json(output_path,messages);
	}
	if (type==CSV){	
		strcpy(output_path,matchtype(path,type));
		generate_output_csv(output_path,messages);
	}
}




