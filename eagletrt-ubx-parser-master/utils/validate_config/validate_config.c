#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<json-c/json.h>
#include <wjelement.h>
#include <wjreader.h>
#include "validate_config.h"

static WJElement schema_load(const char *name, void *client,
							 const char *file, const int line) {
	char *format;
	char *path;
	FILE *schemafile;
	WJReader readschema;
	WJElement schema;

	schema = NULL;
	if(client && name) {
		format = (char *)client;
		path = malloc(strlen(format) + strlen(name));
		sprintf(path, format, name);

		if ((schemafile = fopen(path, "r"))) {
			if((readschema = WJROpenFILEDocument(schemafile, NULL, 0))) {
				schema = WJEOpenDocument(readschema, NULL, NULL, NULL);
			} else {
				fprintf(stderr, "json document failed to open: '%s'\n", path);
			}
			fclose(schemafile);
		} else {
			fprintf(stderr, "json file not found: '%s'\n", path);
		}
		free(path);
	}
  WJEDump(schema);
	return schema;
}

/*
  callback: cleanup/free open schema
*/
static void schema_free(WJElement schema, void *client) {
	WJECloseDocument(schema);
	return;
}

/*
  callback: plop validation errors to stderr
*/
static void schema_error(void *client, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}


int validateConfig(){
	FILE *jsonfile;
	FILE *schemafile;
	WJReader readjson;
	WJReader readschema;
	WJElement json;
	WJElement schema;
	char *format;

	if(!(jsonfile = fopen("config.json", "r"))) {
		fprintf(stderr, "json file not found");
		return 1;
	}
	if(!(schemafile = fopen("config.schema", "r"))) {
		fprintf(stderr, "schema file not found");
		return 2;
	}

	if(!(readjson = WJROpenFILEDocument(jsonfile, NULL, 0)) ||
	   !(json = WJEOpenDocument(readjson, NULL, NULL, NULL))) {
		fprintf(stderr, "json could not be read.\n");
		return 3;
	}
	if(!(readschema = WJROpenFILEDocument(schemafile, NULL, 0)) ||
	   !(schema = WJEOpenDocument(readschema, NULL, NULL, NULL))) {
		fprintf(stderr, "schema could not be read.\n");
		WJECloseDocument(json);
		return 4;
	}

	//WJEDump(json);
	printf("json: %s\n", readjson->depth? "bad" : "good");
	
	//WJEDump(schema);
	printf("schema: %s\n", readschema->depth ? "bad" : "good");

	if((WJESchemaValidate(schema, json, schema_error, schema_load, schema_free,
						 format))&&(readjson->depth==0)&&(readschema->depth==0)) {
		printf("validation: PASS\n");
	} else {
		printf("validation: FAIL\n");
		return 1;
	}
	WJECloseDocument(json);
	WJECloseDocument(schema);
	WJRCloseDocument(readjson);
	WJRCloseDocument(readschema);
	fclose(jsonfile);
	fclose(schemafile);
	return 0;
}
