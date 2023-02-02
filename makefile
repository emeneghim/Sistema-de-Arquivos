prefix = .
bindir = $(prefix)/
builddir = $(prefix)/build
includedir = $(prefix)/include
srcdir = $(prefix)/src
INCLUDES = -I$(includedir) 

CCFLAGS = -g

OBJS = $(builddir)/dispositivoDeBloco.o



all:  dispositivoDeBloco  sistemaDeArquivos data main

sistemaDeArquivos:
	gcc -c sistemaDeArquivos.c

dispositivoDeBloco:
	gcc -c dispositivoDeBloco.c


data:
	truncate -s 1M $(bindir)/bla.data

main:
	gcc main.c 

clean:
	rm -f *.o
	rm -f *.out
