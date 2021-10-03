COMPILER	=	clang++ -std=c++2a -ferror-limit=1000

OBJECTS 	= 	bin/JSONObject.o bin/main.o bin/Packet.o bin/PacketStream.o bin/PrintBigNumbers.o bin/ProtocolInformation.o bin/protocols.o

PACKETS		=	756_2_2.hpp 756_3_21.hpp


PACKETS_	=	$(patsubst %,Protocols/Packets/%,$(PACKETS))

rm_parser: ${OBJECTS} Makefile
	-mkdir bin

	${COMPILER} ${OBJECTS} -o rm_parser
	
.PHONY: clean rebuild build run
build: rm_parser
rebuild: clean rm_parser
clean: 
	-rm -rv bin

bin/main.o: main.cpp State.hpp
	-mkdir bin
	${COMPILER} -c $< -o $@

bin/protocols.o: Protocols/Protocols.cpp Protocols/Protocols.hpp ${PACKETS_} State.hpp
	-mkdir bin
	${COMPILER} -c $< -o $@

bin/%.o: %.cpp %.hpp State.hpp
	-mkdir bin
	${COMPILER} -c $< -o $@
