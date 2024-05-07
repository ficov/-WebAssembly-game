all:
	g++ -Isrc/Include -Lsrc/lib -o snake *.cpp -lmingw32 -lSDL2main -lSDL2