#ifndef LIBSYM_H
#define LIBSYM_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"

#ifdef __cplusplus
extern "C" {
#endif
        
typedef struct sk_node_t{
	fmpz_poly_t sknode;
	struct sk_node_t *next;
} sk_node_t;

typedef struct bv_sym_ct{
	sk_node_t *front, *rear;
	int len;
} bv_sym_ct;

typedef struct bv_sym_context_t {
        fmpz_t q;
        fmpz_t t;
        fmpz_t d;
        fmpz_t n;
} bv_sym_context_t;

static const double pi = 3.1415926;
static double dvn = 1.01; /* standard deviation of Guassian distribution*/
static long chrnd = 0;

bv_sym_context_t *bv_sym_init(bv_sym_context_t *ctx, char *nn, char *tt, char *qq, char *dd, int b);
double bv_sym_get_dvn();
bv_sym_ct *bv_sym_ctinit(bv_sym_ct *ct);
bv_sym_ct *bv_sym_ctsetlen(bv_sym_ct *ct, int nlen);
int bv_sym_ctgetlen(bv_sym_ct *ct);
bv_sym_ct *bv_sym_ctadd(bv_sym_ct *ct, fmpz_poly_t fp);
void hcrypt_random(mpz_t r, int len);
fmpz *bv_sym_samplez(fmpz *vec, long n);
void bv_sym_guassian_poly(fmpz *c, fmpz_poly_t poly, long n);
void bv_sym_unif_poly(fmpz_poly_t poly, long n, fmpz_t q);

#ifdef __cplusplus
}
#endif
#endif