#ifndef GENERATE_OUTPUT_H_
#define GENERATE_OUTPUT_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../parse_ubx/parse_ubx.h"
#include "../config_parser/config_parser.h"

//ricostruisce nome file di output
char *matchtype(const char *n, parse_t type);

//stampa i valori nei file di output json
void print_json(FILE *json_file, const message_t messages);
//inizializza la stampa dei file json
void generate_output_json(const char* output_path, const message_t* messages);

//stampa i valori nei file di output csv
void print_csv(FILE *csv_file, const message_t messages);
//inizializza la stampa dei file csv
void generate_output_csv(const char* output_path, const message_t* messages);

//fa partire le generazioni dei file di output
void generate_output(const char* path, const message_t* messages, const parse_t type);

#endif
