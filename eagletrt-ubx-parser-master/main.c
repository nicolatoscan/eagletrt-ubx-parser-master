#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<json-c/json.h>
#include <wjelement.h>
#include <wjreader.h>

#include "utils/config_parser/config_parser.h"
#include "utils/config_parser/config_parser.c"
#include "utils/parse_ubx/parse_ubx.h"
#include "utils/parse_ubx/parse_ubx.c"
#include "utils/generate_output/generate_output.h"
#include "utils/generate_output/generate_output.c"
#include "utils/validate_config/validate_config.h"
#include "utils/validate_config/validate_config.c"
#include "utils/log_utils/log_utils.h"
#include "utils/log_utils/log_utils.c"
#include "utils/log_utils/log_utils_colors.h"

int main(){
	message_t messages[2000];
	config_t path;
	int i,validate;
	
	validate=validateConfig();
	if (validate){
		logError("Invalide config.json");
		return 1;
	}	
	path=parse_config();
	for(i=0;i<path.inputs_count;i++){
		printf("%d %s\n",i+1, path.path[i]);
		parse_ubx(path.path[i], messages, path.limit_warning);
		generate_output(path.path[i],messages,path.type);
	}		
	logSuccess("Fine scansione\n");
	
}

