#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"
#include <libsym.h>

int main(int argc, char *args[])/*n t q d len1 ct sk*/
{
        bv_sym_context_t *params;
        params = bv_sym_init(params, args[1], args[2], args[3], args[4], 10);
        long n = fmpz_get_ui(params->n);
        fmpz_t len;
        
        fmpz_init(len);
        fmpz_set_str(len,args[5],10);
        long l = fmpz_get_ui(len);
        
	long i;
        fmpz_poly_t fx,m;
        fmpz_poly_init(fx);
        fmpz_poly_set_coeff_ui(fx, 0, 1);
        fmpz_poly_set_coeff_ui(fx, n, 1);
        fmpz_poly_init(m);

	fmpz_poly_t tmp, ct, sk;
	fmpz_poly_init(tmp);
        fmpz_poly_init(ct);
        fmpz_poly_init(sk);
	fmpz_poly_zero(m);

	for( i = 1 ; i <= l ; i++) {
                fmpz_poly_set_str(ct, args[5 + i]);
                fmpz_poly_set_str(sk, args[5 + i + l]);
		fmpz_poly_mul(tmp, ct,  sk);
		fmpz_poly_add(m, m, tmp);
	}
	fmpz_poly_rem_basecase(m, m, fx);
	fmpz_poly_scalar_smod_fmpz(m, m, params->q);
	fmpz_poly_scalar_smod_fmpz(m, m, params->t);
        char *ms = fmpz_poly_get_str(m);
        printf("\"%s\"\n", ms);
        
        fmpz_clear(len);
        fmpz_poly_clear(sk);
        fmpz_poly_clear(m);
        fmpz_poly_clear(tmp);
        fmpz_poly_clear(ct);
        fmpz_poly_clear(fx);
        return 0;
}