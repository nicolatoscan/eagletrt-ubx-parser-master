#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<json-c/json.h>
#include "file.h"
#include "file.c"


int main(){
	
	FILE *fj;			
	char buffer[1024], fo[100],ff[100];
  	int a=0;
  	int t,j,c;
  	
  	struct json_object *parsed_json;
	struct json_object *type;
	struct json_object *inputs;
	struct json_object *input;
	size_t n_inputs;
	size_t i;	
	
    	printf("Inizio scansione\n");
    	
    	fj = fopen("config.json","r");
	fread(buffer, 1024, 1, fj);
	fclose(fj);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "TYPE", &type);
	json_object_object_get_ex(parsed_json, "INPUTS", &inputs);
    	
    	j=isjson(json_object_get_string(type));
    	c=iscsv(json_object_get_string(type));
    	if(j==1){
		t=j;
	}
	if(c==2){
		t=c;
	}
    	
    	n_inputs = json_object_array_length(inputs);

	for(i=0;i<n_inputs;i++) {
		input = json_object_array_get_idx(inputs, i);
		strcpy(fo,matchubx(json_object_get_string(input)));
  		strcpy(ff,matchtype(json_object_get_string(input),t));
  		openf(fo,ff,t);
	}
	printf("Fine scansione\n");

}
