.PHONY: compile run clean

run: giga
	./giga

compile giga: giga.o cursor.o setup.o config.o util.o read.o modify.o helpbar.o visual.o tabs.o copypaste.o
	gcc -Wall -Werror -o giga giga.o cursor.o setup.o config.o util.o read.o modify.o helpbar.o visual.o tabs.o copypaste.o -lncursesw

giga.o: giga.c giga.h cursor.c cursor.h setup.c setup.h util.c util.h read.c read.h modify.c modify.h helpbar.c helpbar.h visual.c visual.h tabs.c tabs.h copypaste.c copypaste.h config.c config.h
	gcc -c giga.c

cursor.o: cursor.c cursor.h giga.c giga.h util.c util.h visual.c visual.h helpbar.c helpbar.h
	gcc -c cursor.c

setup.o: setup.c setup.h giga.c giga.h config.c config.h visual.c visual.h tabs.c tabs.h read.c read.h util.c util.h
	gcc -c setup.c

config.o: config.c config.h giga.c giga.h util.c util.h
	gcc -c config.c

util.o: util.c util.h
	gcc -c util.c

read.o: read.c read.h giga.c giga.h util.c util.h helpbar.c helpbar.h cursor.c cursor.h
	gcc -c read.c

modify.o: modify.c modify.h giga.c giga.h util.c util.h
	gcc -c modify.c

helpbar.o: helpbar.c helpbar.h giga.c giga.h
	gcc -c helpbar.c

visual.o: visual.c visual.h giga.c giga.h helpbar.c helpbar.h cursor.c cursor.h util.c util.h
	gcc -c visual.c

tabs.o: tabs.c tabs.h giga.c giga.h visual.c visual.h helpbar.c helpbar.h util.c util.h read.c read.h cursor.c cursor.h
	gcc -c tabs.c

copypaste.o: copypaste.c util.h modify.h
	gcc -c copypaste.c

clean:
	rm giga
	rm *.o
