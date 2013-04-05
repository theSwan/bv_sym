#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"
#include <libsym.h>

int main(int argc, char *args[]) /*n t q d*/
{
        bv_sym_context_t *params;
        params = bv_sym_init(params, args[1], args[2], args[3], args[4], 10);
        long n = fmpz_get_ui(params->n);
        fmpz_poly_t sk,fx;
        fmpz_poly_init(sk);
        fmpz *ctr;
        ctr = _fmpz_vec_init(n);
	bv_sym_guassian_poly(ctr, sk, n);
        fmpz_poly_init(fx);
        fmpz_poly_set_coeff_ui(fx, 0, 1);
        fmpz_poly_set_coeff_ui(fx, n, 1);
        
	sk_node_t *s, *r, *sk_head;
	sk_head = (sk_node_t *)malloc(sizeof(sk_node_t));
	sk_head->next = NULL;
	s = sk_head;
        char *output;
	fmpz_poly_init(s->sknode);
	fmpz_poly_set_coeff_si(s->sknode, 0, 1);
        output = fmpz_poly_get_str(s->sknode);
        printf("%s\n",output);

	long dn = fmpz_get_ui(params->d);
	long i;
	for( i = 1 ; i <= dn ; i++ ) {
		r = (sk_node_t *)malloc(sizeof(sk_node_t));
		fmpz_poly_init(r->sknode);
		fmpz_poly_mul( r->sknode, sk, s->sknode);
		fmpz_poly_rem_basecase(r->sknode, r->sknode, fx);
                fmpz_poly_scalar_smod_fmpz(r->sknode, r->sknode, params->q);
                
                output = fmpz_poly_get_str(r->sknode);
                printf("%s\n",output);
                
		s->next = r;
		s = r;
	}
	s->next = NULL;
        fmpz_poly_clear(fx);
        fmpz_poly_clear(sk);
        _fmpz_vec_clear(ctr, n);
        
        return 0;
}


