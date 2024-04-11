all:
	g++ -IC:\SDL2_devops\src\include -LC:\SDL2_devops\src\lib -o game main.cpp -lmingw32 -lSDL2main -lSDL2