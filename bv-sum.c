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
        long l1 = fmpz_get_ui(len1), l2 = fmpz_get_ui(len2), i;
        
        fmpz_poly_t fx, ctnode;
        fmpz_poly_init(fx);
        fmpz_poly_init(ctnode);
        fmpz_poly_set_coeff_ui(fx, 0, 1);
        fmpz_poly_set_coeff_ui(fx, n, 1);
        
        bv_sym_ct *ct1, *ct2, *ct;
        ct1 = bv_sym_ctinit(ct1);
        ct2 = bv_sym_ctinit(ct2);
        ct = bv_sym_ctinit(ct);
        for( i = 1 ; i <= l1 ; i++) {
                fmpz_poly_set_str(ctnode, args[6+i]);
                ct1 = bv_sym_ctadd(ct1, ctnode);
        }
        for( i = 1 ; i <= l2 ; i++) {
                fmpz_poly_set_str(ctnode, args[6+i+l1]);
                ct2 = bv_sym_ctadd(ct2, ctnode);
        }
	fmpz_poly_t tmp;
	fmpz_poly_init(tmp);
	sk_node_t *list1, *list2;
	list1 = ct1->front;
	list2 = ct2->front;
	if ( l1 < l2 ) {
		while ( list1 != NULL ) {
			fmpz_poly_add(tmp, list1->sknode, list2->sknode);
			fmpz_poly_rem_basecase(tmp, tmp, fx);
			fmpz_poly_scalar_smod_fmpz(tmp, tmp, params->q);
			ct = bv_sym_ctadd(ct, tmp);
			list1 = list1->next;
			list2 = list2->next;
		}
		while(list2 != NULL) {
			fmpz_poly_set(tmp, list2->sknode);
			ct = bv_sym_ctadd(ct, tmp);
			list2 = list2->next;
		}
	}
	else {
		while ( list2 != NULL ) {
			fmpz_poly_add(tmp, list1->sknode, list2->sknode);
			fmpz_poly_rem_basecase(tmp, tmp, fx);
			fmpz_poly_scalar_smod_fmpz(tmp, tmp, params->q);
			ct = bv_sym_ctadd(ct, tmp);
			list1 = list1->next;
			list2 = list2->next;
		}
		while(list1 != NULL) {
			fmpz_poly_set(tmp, list1->sknode);
			ct = bv_sym_ctadd(ct, tmp);
			list1 = list1->next;
		}
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
        fmpz_poly_clear(tmp);
        fmpz_poly_clear(fx);
	return 0;
}