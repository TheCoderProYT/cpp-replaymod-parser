COMPILER = clang++ -std=c++2a

rm_parser: bin/*.o bin/functions/*.o Makefile
	clear
	${COMPILER} bin/*.o bin/functions/*.o -o rm_parser
	
.PHONY: clean
clean: 
	-rm -f bin/*.o bin/functions/*.o

bin/main.o: main.cpp
	${COMPILER} -c $< -o $@

bin/%.o: %.cpp %.hpp
	${COMPILER} -c $< -o $@

bin/functions/%.o: functions/%.cpp functions/%.hpp
	${COMPILER} -c $< -o $@