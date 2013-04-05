#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"
#include <libsym.h>

char str[100000];

int main(int argc, char *args[])/*n t q d ct1.txt ct2.txt*/
{
        bv_sym_context_t *params;
        params = bv_sym_init(params, args[1], args[2], args[3], args[4], 10);
        long n = fmpz_get_ui(params->n), i;
        FILE *fp;
        
        if((fp = fopen(args[5], "r")) == NULL)
        {
                printf("file read error\n");
                exit(0);
        }
        
        fmpz_t len1;
        fmpz_init(len1);
        fgets(str, 10, fp);
        fmpz_set_str(len1 , str, 10);
        
        long l1 = fmpz_get_ui(len1);
        
        fmpz_poly_t ctnode;        
        fmpz_poly_init(ctnode);
        
        bv_sym_ct *ct1, *ct;
        ct1 = bv_sym_ctinit(ct1);
        ct = bv_sym_ctinit(ct);
        for( i = 1 ; i <= l1 ; i++) {
                fgets(str, 100000, fp);
                fmpz_poly_set_str(ctnode, str);
                ct1 = bv_sym_ctadd(ct1, ctnode);
        }
        fclose(fp);
        
        if((fp = fopen(args[6], "r")) == NULL)
        {
                printf("file read error\n");
                exit(0);
        }
        
        fmpz_t len2;
        fmpz_init(len2);
        fgets(str, 10, fp);
        fmpz_set_str(len2 , str, 10);
        
        long l2 = fmpz_get_ui(len2);        
        
        bv_sym_ct *ct2;
        ct2 = bv_sym_ctinit(ct2);
        for( i = 1 ; i <= l2 ; i++) {
                fgets(str, 100000, fp);
                fmpz_poly_set_str(ctnode, str);
                ct2 = bv_sym_ctadd(ct2, ctnode);
        }
        fclose(fp);
        
	fmpz_poly_t tmp;
	fmpz_poly_init(tmp);
	sk_node_t *list1, *list2;
	list1 = ct1->front;
	list2 = ct2->front;
	if ( l1 < l2 ) {
		while ( list1 != NULL ) {
			fmpz_poly_add(tmp, list1->sknode, list2->sknode);
			fmpz_poly_scalar_smod_fmpz(tmp, tmp, params->q);
			ct = bv_sym_ctadd(ct, tmp);
			list1 = list1->next;
			list2 = list2->next;
		}
		while(list2 != NULL) {
			ct = bv_sym_ctadd(ct, list2->sknode);
			list2 = list2->next;
		}
	}
	else {
		while ( list2 != NULL ) {
			fmpz_poly_add(tmp, list1->sknode, list2->sknode);
			fmpz_poly_scalar_smod_fmpz(tmp, tmp, params->q);
			ct = bv_sym_ctadd(ct, tmp);
			list1 = list1->next;
			list2 = list2->next;
		}
		while(list1 != NULL) {
			ct = bv_sym_ctadd(ct, list1->sknode);
			list1 = list1->next;
		}
	}
        sk_node_t *prt;
        prt = ct->front;
        printf("%d\n", ct->len);
        while(prt != NULL) {
                char *sum = fmpz_poly_get_str(prt->sknode);
                printf("%s\n", sum);
                prt = prt->next;
        }
	  
        fmpz_clear(len1);
        fmpz_clear(len2);
        fmpz_poly_clear(ctnode);
        fmpz_poly_clear(tmp);
	return 0;
}