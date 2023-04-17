all: myshell looper mypipe

myshell: myshell.c LineParser.c
	gcc -m32 -Wall -o myshell myshell.c LineParser.c
looper: looper.c
	gcc -m32 -Wall -o looper looper.c
mypipe: mypipe.c
	gcc -m32 -Wall -o mypipe mypipe.c

.PHONY: clean

clean:
	rm -f *.o myshell looper
