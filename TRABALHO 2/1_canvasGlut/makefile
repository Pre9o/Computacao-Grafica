modules = gl_canvas2d Image ImagesFunctions Controle Bloco Bola Canhao Tabuleiro Explosoes
files = $(addprefix ./.src/, $(modules))
objs = $(addsuffix .o, $(modules))
libs = libfreeglut32.a libopengl32.a libglu32.a
libpaths = $(addprefix ../lib/, $(libs))
err = -Wall -Wpedantic -Wextra #-Werror
opt = -O3

all: main
	./main

main: $(modules)
	g++ ./src/main.cpp -o main.exe $(objs) $(libpaths) $(err) $(opt) -lwinmm -I../include

$(modules): %: src/%.cpp
	g++ -g -c src/$@.cpp -o $@.o $(libpaths) $(opt) $(err) -lwinmm -I../include

.PHONY: clean
clean:
	rm *.o *.out *.exe main.exe -rf
