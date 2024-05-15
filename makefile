all : kmnRandomCheck

kmnRandomCheck : kmnRandomCheck.o
	gcc -m32 -g -Wall -o kmnRandomCheck kmnRandomCheck.o -lm

kmnRandomCheck.o : kmnRandomCheck.c
	gcc -m32 -g -Wall -c -o kmnRandomCheck.o kmnRandomCheck.c

clean :
	rm -f kmnRandomCheck kmnRandomCheck.o