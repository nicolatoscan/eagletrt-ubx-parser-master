#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<json-c/json.h>
#include "config_parser.h"

//restituisce i parametri del file config.json
config_t parse_config(){
	FILE *config_json;			
	char buffer[1024];
  	parse_t type_t;
  	config_t path;
  	
  	struct json_object *parsed_json;
	struct json_object *type;
	struct json_object *inputs;
	struct json_object *input;
	size_t n_inputs;
	size_t i;	
	
    	printf("Inizio scansione\n");
    	
    	config_json = fopen("config.json","r");
	fread(buffer, 1024, 1, config_json);
	fclose(config_json);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "TYPE", &type);
	json_object_object_get_ex(parsed_json, "INPUTS", &inputs);
    	
    	path.type=what_type(json_object_get_string(type));
    	
    	path.inputs_count = json_object_array_length(inputs);

	for(i=0;i<path.inputs_count;i++) {
		input = json_object_array_get_idx(inputs, i);
		strcpy(path.path[i],json_object_get_string(input));
	}
	return path;

}

//restituisce il tipo di file di output
parse_t what_type(const char *t){		//identifica se il config.json è settato su cvs
  	if (strcmp(t,json)==0){
		return JSON;
	}
	if (strcmp(t,csv)==0){
		return CSV;
	}
}


