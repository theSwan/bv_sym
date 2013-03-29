#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"
#include <libsym.h>

int main(int argc, char *args[])/* n t q d sk m*/
{
        bv_sym_context_t *params;
        params = bv_sym_init(params, args[1], args[2], args[3], args[4], 10);
        long n = fmpz_get_ui(params->n);
        
        fmpz_poly_t sk,fx,m;
        fmpz_poly_init(fx);
        fmpz_poly_set_coeff_ui(fx, 0, 1);
        fmpz_poly_set_coeff_ui(fx, n, 1);
        fmpz_poly_init(sk);
        fmpz_poly_set_str(sk, args[5]);
        fmpz_poly_init(m);
        fmpz_poly_set_str(m, args[6]);
        fmpz *ctr;
        ctr = _fmpz_vec_init(n);
	fmpz_poly_t e, a, b;
	fmpz_poly_init(e);
	fmpz_poly_init(a);
	fmpz_poly_init(b);
	bv_sym_guassian_poly(ctr, e, n);
	bv_sym_unif_poly(a, n, params->q);
	fmpz_poly_mul(b, a, sk);
	fmpz_poly_scalar_addmul_fmpz(b, e, params->t);
	fmpz_poly_t sk1, sk2;
	fmpz_poly_init(sk1);
	fmpz_poly_init(sk2);
        
	fmpz_poly_add(sk1, b, m);
        fmpz_poly_rem_basecase(sk1, sk1, fx);
        fmpz_poly_scalar_smod_fmpz(sk1, sk1, params->q);
	fmpz_poly_neg(sk2, a);
	char *s1 = fmpz_poly_get_str(sk1);
        char *s2 = fmpz_poly_get_str(sk2);
        printf("\"%s\" \"%s\"\n", s1, s2);
      
        fmpz_poly_clear(sk);
        fmpz_poly_clear(m);
        fmpz_poly_clear(fx);
	fmpz_poly_clear(e);
	fmpz_poly_clear(a);
	fmpz_poly_clear(b);
	return 0;
}