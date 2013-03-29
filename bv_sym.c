#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"
#include "libsym.h"

bv_sym_context_t *bv_sym_init(bv_sym_context_t *ctx, char *nn, char *tt, char *qq, char *dd, int b)
{
        ctx = (bv_sym_context_t *)malloc(sizeof(bv_sym_context_t));
        fmpz_init(ctx->q);
        int flag = fmpz_set_str(ctx->q, qq, b);
        if(flag == -1){
                printf("invalid number q\n");
                exit(0);
        }
        fmpz_init(ctx->t);
        flag = fmpz_set_str(ctx->t, tt, b);
        if(flag == -1){
                printf("invalid number t\n");
                exit(0);
        }
        fmpz_init(ctx->n);
        flag = fmpz_set_str(ctx->n, nn, b);
        if(flag == -1){
                printf("invalid number n\n");
                exit(0);
        }
        fmpz_init(ctx->d);
        flag = fmpz_set_str(ctx->d, dd, b);
        if(flag == -1){
                printf("invalid number d\n");
                exit(0);
        }
        return ctx;
}

double bv_sym_get_dvn()
{
	return dvn;
}

bv_sym_ct *bv_sym_ctinit(bv_sym_ct *ct)
{
	ct = (bv_sym_ct *)malloc(sizeof(bv_sym_ct));
	ct->len = 0;
	ct->front = ct->rear = NULL;
	return ct;
}

bv_sym_ct *bv_sym_ctsetlen(bv_sym_ct *ct, int nlen)
{
	ct->len = nlen;
	return ct;
}

int bv_sym_ctgetlen(bv_sym_ct *ct)
{
	return ct->len;
}

bv_sym_ct *bv_sym_ctadd(bv_sym_ct *ct, fmpz_poly_t fp)
{
	sk_node_t *tmp;
	tmp = (sk_node_t *)malloc(sizeof(sk_node_t));
	fmpz_poly_init(tmp->sknode);
	fmpz_poly_set(tmp->sknode, fp);
	tmp->next = NULL;
	if ( ct->rear == NULL) {
		ct->front = ct->rear = tmp;
	}
	else {
		ct->rear->next = tmp;
		ct->rear = tmp;
	}
	ct->len = ct->len + 1;
	return ct;
}

void hcrypt_random(mpz_t r, int len)
{
	FILE *fp;
	int flag = 0;
	fp = fopen("/dev/urandom", "rb");
	if (fp) {
		int bytecount, leftover;
		unsigned char *bytes;
		bytecount = (len + 7) / 8;
		leftover = len % 8;
		bytes = (unsigned char *) malloc(bytecount);

		if (fread(bytes, 1, bytecount, fp)) {
			if (leftover) {
				*bytes = *bytes % (1 << leftover);
			}
			mpz_import(r, bytecount, 1, 1, 0, 0, bytes);
			flag = 1;
		}
		fclose(fp);
		free(bytes);
	}
	if(!fp || !flag) {
		gmp_randstate_t gmpRandState;
		gmp_randinit_default(gmpRandState);
		gmp_randseed_ui(gmpRandState, (unsigned long)time(0)+(chrnd++));
		while( 1 ) {
			mpz_urandomb(r, gmpRandState, len);
			if( mpz_sizeinbase(r, 2) == len)
				break;
		}
		gmp_randclear(gmpRandState);
	}
}

fmpz *bv_sym_samplez(fmpz *vec, long n)
{
	if ( n == 0 )
		return;
	double tdvn = bv_sym_get_dvn();
	long a = (long)ceil(-10*tdvn);
	long b = (long)floor(+10*tdvn);
	long x, i;
	double p;
	int len = sizeof(unsigned long int);
	mpz_t randseed;
	mpz_init(randseed);
	hcrypt_random(randseed, len);
	unsigned long int useed = mpz_get_ui(randseed);
	srand(useed);
	for( i = 0 ; i < n ; i++) {
		do {
			x = rand()%(b - a) + a;
			p = exp(-pi*x / ( tdvn * tdvn));
		} while ( !( p > 0 && p <= 1) );

		vec[i] = x;
	}
	mpz_clear(randseed);
	return vec;
}

void bv_sym_guassian_poly(fmpz *c, fmpz_poly_t poly, long n)
{
	fmpz *tmp = bv_sym_samplez(c, n);
	long k;
	for( k = 0 ; k < n ; k++ ) {
		fmpz_poly_set_coeff_si(poly, k, tmp[k]);
	}
}

void bv_sym_unif_poly(fmpz_poly_t poly, long n, fmpz_t q)
{
	int i;
	int len = sizeof(unsigned long int);
	mpz_t randseed;
	mpz_init(randseed);
	hcrypt_random(randseed, len);
	unsigned long int useed = mpz_get_ui(randseed);
	mpz_t rndnum,rndbd;
	fmpz_t rndfmpz;
	gmp_randstate_t gmpstate;

	mpz_init(rndnum);
	mpz_init(rndbd);
	fmpz_get_mpz(rndbd, q);
	fmpz_init(rndfmpz);
	gmp_randinit_default(gmpstate);
	gmp_randseed_ui(gmpstate, useed);

	for( i = 0 ; i < n ; i++ ) {
		mpz_urandomm(rndnum, gmpstate, rndbd);
		fmpz_set_mpz(rndfmpz, rndnum);
		fmpz_poly_set_coeff_fmpz(poly, i, rndfmpz);
	}
        fmpz_poly_scalar_smod_fmpz(poly, poly, q);

	mpz_clear(randseed);
	fmpz_clear(rndfmpz);
	gmp_randclear(gmpstate);
	mpz_clear(rndnum);
	mpz_clear(rndbd);
}
