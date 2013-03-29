#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"
#include <libsym.h>

int main(int argc, char *args[])/*n t q d len1 len2 ct1 ct2*/
{
        bv_sym_context_t *params;
        params = bv_sym_init(params, args[1], args[2], args[3], args[4], 10);
        long n = fmpz_get_ui(params->n);
        fmpz_t len1, len2;
        fmpz_init(len1);
        fmpz_set_str(len1,args[5],10);
        fmpz_init(len2);
        fmpz_set_str(len2, args[6],10);
        long l1 = fmpz_get_ui(len1), l2 = fmpz_get_ui(len2);
        
	long ii;
        fmpz_poly_t fx, ctnode;
        fmpz_poly_init(fx);
        fmpz_poly_init(ctnode);
        fmpz_poly_set_coeff_ui(fx, 0, 1);
        fmpz_poly_set_coeff_ui(fx, n, 1);
        
        bv_sym_ct *ct1, *ct2, *ct;
        ct1 = bv_sym_ctinit(ct1);
        ct2 = bv_sym_ctinit(ct2);
        ct = bv_sym_ctinit(ct);
        for( ii = 1 ; ii <= l1 ; ii++) {
                fmpz_poly_set_str(ctnode, args[6+ii]);
                ct1 = bv_sym_ctadd(ct1, ctnode);
        }
        for( ii = 1 ; ii <= l2 ; ii++) {
                fmpz_poly_set_str(ctnode, args[6+ii+l1]);
                ct2 = bv_sym_ctadd(ct2, ctnode);
        }

	fmpz_poly_t tmp, tmp1;
	fmpz_poly_init(tmp);
	fmpz_poly_init(tmp1);
	sk_node_t *list1, *list2;
	long len = l1 + l2 - 1;
	long i, j, k;
	for(k = 0 ; k < len ; k++) {
		fmpz_poly_zero(tmp);
		list1 = ct1->front;
                
		for(i=0 ; i<l1 ; i++) {
			list2 = ct2->front;
			for(j=0 ; j<l2 ; j++) {
				if ( i + j == k ) {
					fmpz_poly_mul(tmp1, list1->sknode, list2->sknode);
					fmpz_poly_add(tmp, tmp, tmp1);
				}
				list2 = list2->next;
			}
                        list1 = list1->next;
		}
		fmpz_poly_rem_basecase(tmp, tmp, fx);
		fmpz_poly_scalar_smod_fmpz(tmp, tmp, params->q);
		ct = bv_sym_ctadd(ct, tmp);
                
	}
        sk_node_t *prt;
        prt = ct->front;
        while(prt!=NULL) {
                char *sum = fmpz_poly_get_str(prt->sknode);
                printf("\"%s\" \n", sum);
                prt = prt->next;
        }
        printf("\n");
        
        fmpz_clear(len1);
        fmpz_clear(len2);
        fmpz_poly_clear(ctnode);
        fmpz_poly_clear(fx);
	fmpz_poly_clear(tmp);
	fmpz_poly_clear(tmp1);
	return 0;
}