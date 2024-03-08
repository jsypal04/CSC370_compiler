main: main.o lexer.o
	g++ -c src/lexer.cpp src/main.cpp
	g++ -o main lexer.o main.o