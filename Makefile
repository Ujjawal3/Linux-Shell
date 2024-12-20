
shell: shell.o command.o prompt.o utils.o 
	g++ -Wall shell.o command.o prompt.o utils.o -o shell -lreadline

shell.o: shell.cpp command.cpp command.h prompt.h prompt.cpp utils.h utils.cpp
command.o: utils.h command.cpp command.h
prompt.o: prompt.h prompt.cpp utils.h command.h
utils.o: prompt.h utils.h utils.cpp


clean: 
	rm -f *.o taaj sheel