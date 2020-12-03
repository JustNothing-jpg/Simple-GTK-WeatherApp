compile: main.c
	LANG=en gcc main.c -o main.o `pkg-config --cflags --libs gtk+-3.0` -lcurl -ljson-c -Wall