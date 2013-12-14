all:
	g++ -ansi -pedantic -Wall -Wextra -Werror -Wfatal-errors -O2 -c gl.cpp
	g++ -shared gl.o -o luaglew.dll -llua52 -lopengl32 -lglew32 -lglu32
