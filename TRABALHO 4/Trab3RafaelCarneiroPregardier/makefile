modules = gl_canvas2d Pecas CilindroECubos FuncoesGerais Engrenagem
files = $(addprefix ./.src/, $(modules))
objs = $(addsuffix .o, $(modules))
libs = libfreeglut32.a libopengl32.a libglu32.a
libpaths = $(addprefix ../lib/, $(libs))
err = -Wall -Wpedantic -Wextra #-Werror
opt = -O3

all: main
	./main

main: $(modules)
	g++ ./src/main.cpp -o main.exe $(objs) $(libpaths) $(err) $(opt) -I../include

$(modules): %: src/%.cpp
	g++ -g -c src/$@.cpp -o $@.o $(libpaths) $(opt) $(err) -I../include

.PHONY: clean
clean:
	rm *.o *.out *.exe main.exe -rf
