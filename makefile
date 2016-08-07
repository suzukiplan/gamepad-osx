all:
	gcc -DDEBUG -o test test.c gamepad.c -framework Foundation -framework IOKit

format:
	sh format.sh gamepad.c
	sh format.sh gamepad.h
	sh format.sh test.c
