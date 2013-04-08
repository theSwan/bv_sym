keygen:
	gcc -o keygen local.c bv-keygen.c -lflint -lgmp -lmpfr -lpthread -lm
enc:
	gcc -o enc local.c bv-enc.c -lflint -lgmp -lmpfr -lpthread -lm
dec:
	gcc -o dec local.c bv-dec.c -lflint -lgmp -lmpfr -lpthread -lm
mul:
	gcc -o mul local.c bv-mul.c -lflint -lgmp -lmpfr -lpthread -lm
add:
	gcc -o add local.c bv-add.c -lflint -lgmp -lmpfr -lpthread -lm
clear:
	rm keygen && rm enc && rm dec && rm mul && rm add
all:
	make keygen && make enc && make dec && make mul && make add
