#!/bin/bash

gcc -std=gnu90 main.c -o bin/main -ljson-c -lwjelement -lwjreader

./bin/main
