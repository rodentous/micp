#!/usr/bin/bash
(cd ./src && g++ ./*.cpp libraylib.a -lm -lglfw -o main) && ./src/main

