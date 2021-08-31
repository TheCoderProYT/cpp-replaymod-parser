COMPILER = clang++ -std=c++2a

rm_parser: bin/main.o bin/functions/processJSONLine.o Makefile
	${COMPILER} bin/*.o bin/functions/*.o -o rm_parser

bin/functions/processJSONLine.o: functions/processJSONLine.*
	${COMPILER} -c functions/processJSONLine.cpp -o bin/functions/processJSONLine.o

bin/main.o: main.cpp include/* protocolInfo.hpp include/gameState/* include/packets/*
	${COMPILER} -c main.cpp -o bin/main.o