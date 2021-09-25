COMPILER	=	clang++ -std=c++2a -ferror-limit=1000

OBJECTS 	= 	bin/JSONObject.o bin/main.o bin/Packet.o bin/PacketStream.o bin/PrintBigNumbers.o bin/ProtocolInformation.o bin/State.o

rm_parser: ${OBJECTS} Makefile
	clear
	-mkdir bin

	${COMPILER} ${OBJECTS} -o rm_parser
	
.PHONY: clean
clean: 
	-rm -rv bin

bin/main.o: main.cpp Protocols/Protocols.hpp
	-mkdir bin
	${COMPILER} -c $< -o $@
	
bin/%.o: %.cpp %.hpp
	-mkdir bin
	${COMPILER} -c $< -o $@
