main: main.o lexer.o
	g++ -c src/lexer.cpp src/parser.cpp src/main.cpp 
	g++ -o main lexer.o parser.o main.o