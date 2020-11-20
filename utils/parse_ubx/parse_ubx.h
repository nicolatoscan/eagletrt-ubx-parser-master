#ifndef PARSE_UBX_H_
#define PARSE_UBX_H_
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//struct per i messaggi
typedef struct {		
	char utc_time[20];
	double latitude;
	double longitude;
	char altitude[20];
	char speed_knots[20];
	char course[20];

}message_t;

//enum per le tipologie di messaggio
typedef enum {
	GGA,
	GLL,
	RMC,
	GSA,
	GSV,
	VTG,
	UNKNOWN,
	ERR
}message_name_t;

//restituisce il nome del file ubx da analizzare
char *matchubx(const char *path);
//modifica i valori in un formato più leggibile per Google Earth
double parseVal(double value);

//restituisce la tipologia di messaggio
message_name_t get_line_message(char *line);

//restituisce i valori dei vari messaggi
message_t extract_message_GGA(const char *line);
message_t extract_message_GLL(const char *line);
message_t extract_message_RMC(const char *line);

//filtra i messaggi nei formati voluti dividendoli in variabili
void parse_ubx(char* ubx_path, message_t* messages,int limit_warning);



#endif
