rm_parser: main.cpp include/* protocolInfo.hpp Makefile include/gameState/* include/packets/*
	clang++ -std=c++2a -o rm_parser main.cpp
