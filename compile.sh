#!/bin/bash

gcc -std=gnu90 main.c utils/parse_ubx/parse_ubx.c utils/config_parser/config_parser.c utils/generate_output/generate_output.c utils/validate_config/validate_config.c utils/log_utils/log_utils.c -o bin/main -ljson-c -lwjelement -lwjreader

./bin/main
