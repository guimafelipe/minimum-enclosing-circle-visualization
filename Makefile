
build:
	g++ -c mecrep.cpp
	g++ mecrep.o -o mecrep -lsfml-graphics -lsfml-window -lsfml-system

run:
	./mecrep < in$(teste)
