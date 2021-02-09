a.out : BILL_FUNCTIONS.o Dummy_main.o
	gcc BILL_FUNCTIONS.o Dummy_main.o -lallegro -lallegro_font -lallegro_image -lallegro_ttf -lallegro_primitives -lallegro_image  -lm
BILL_FUNCTIONS.o : BILL_FUNCTIONS.c BILL_HEADER.h
	gcc -c BILL_FUNCTIONS.c
Dummy_main.o : Dummy_main.c BILL_HEADER.h
	gcc -c Dummy_main.c