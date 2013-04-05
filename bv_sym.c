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

void hcrypt_random(fmpz_t r)
{
	mpz_t tmp;
	FILE *fp;
	mpz_init(tmp);
	fp = fopen("/dev/urandom", "rb");
        int len = 9;
	if (fp) {
		unsigned char *bytes;
		bytes = (unsigned char *) malloc (len);
                
                if (fread(bytes, 1, len, fp)) {
                        mpz_import(tmp, len, 1, 1, 0, 0, bytes);
                }
                
                else {
                        printf("file read error\n");
                }
                
		fclose(fp);
		free(bytes);
	}
        
        else {
                printf("random number generation error\n");
        }
	
        
	fmpz_set_mpz(r, tmp);
	mpz_clear(tmp);
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
	fmpz_t randseed;
	fmpz_init(randseed);
	hcrypt_random(randseed);
	unsigned long int useed = fmpz_get_ui(randseed);
	srand(useed);
	for( i = 0 ; i < n ; i++) {
		do {
			x = rand()%(b - a) + a;
			p = exp(-pi*x / ( tdvn * tdvn));
		} while ( !( p > 0 && p <= 1) );

		vec[i] = x;
	}
	fmpz_clear(randseed);
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
	fmpz_t randseed;
	fmpz_init(randseed);
	hcrypt_random(randseed);
	unsigned long int useed = fmpz_get_ui(randseed);
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

	fmpz_clear(randseed);
	fmpz_clear(rndfmpz);
	gmp_randclear(gmpstate);
	mpz_clear(rndnum);
	mpz_clear(rndbd);
}
