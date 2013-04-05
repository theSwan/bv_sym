kg:
	gcc -o kg bv_sym.c bv-keygen.c -lflint -lgmp -lmpfr -lpthread -lm
enc:
	gcc -o enc bv_sym.c bv-enc.c -lflint -lgmp -lmpfr -lpthread -lm
dec:
	gcc -o dec bv_sym.c bv-dec.c -lflint -lgmp -lmpfr -lpthread -lm
pro:
	gcc -o pro bv_sym.c bv-product.c -lflint -lgmp -lmpfr -lpthread -lm
sum:
	gcc -o sum bv_sym.c bv-sum.c -lflint -lgmp -lmpfr -lpthread -lm
