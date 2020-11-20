#ifndef PARSE_UBX_H_
#define PARSE_UBX_H_
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<json-c/json.h>
#include <wjelement.h>
#include <wjreader.h>

static WJElement schema_load(const char *name, void *client, const char *file, const int line);
static void schema_free(WJElement schema, void *client);
static void schema_error(void *client, const char *format, ...);
							 
int validateConfig();

#endif
