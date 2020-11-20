#ifndef CONFIG_PARSER_H_
#define CONFIG_PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<json-c/json.h>

//enum per identificare tipo file output
typedef enum {
	CSV,
	JSON
}parse_t;

//struct che definisce i parametri passati dal config.json
typedef struct{
	parse_t type;
	char path[100][1024];
	int inputs_count;
	int limit_warning;
}config_t;

//restituisce i parametri del file config.json
config_t parse_config();

//restituisce il tipo di file di output
parse_t what_type(const char *t);

#endif
