#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"
#include <libsym.h>

char str[100000];

int main(int argc, char *args[])/*n t q d ct.txt sk.txt*/
{
        bv_sym_context_t *params;
        params = bv_sym_init(params, args[1], args[2], args[3], args[4], 10);
        long n = fmpz_get_ui(params->n);
        long i;
        fmpz_poly_t fx,m;
        fmpz_poly_init(fx);
        fmpz_poly_set_coeff_ui(fx, 0, 1);
        fmpz_poly_set_coeff_ui(fx, n, 1);
        fmpz_poly_init(m);
        
        FILE *fp1;
        
        if((fp1 = fopen(args[5], "r")) == NULL)
        {
                printf("file read error\n");
                exit(0);
        }
        fmpz_t len;
        
        fmpz_init(len);
        fgets(str, 10, fp1);
        fmpz_set_str(len, str, 10);
        long l = fmpz_get_ui(len);

	fmpz_poly_t tmp, ct, sk;
	fmpz_poly_init(tmp);
        fmpz_poly_init(ct);
        fmpz_poly_init(sk);
	fmpz_poly_zero(m);
        
        FILE *fp2;
        
        if((fp2 = fopen(args[6], "r")) == NULL)
        {
                printf("file read error\n");
                exit(0);
        }
	for( i = 1 ; i <= l ; i++) {
                fgets(str, 100000, fp1);
                fmpz_poly_set_str(ct, str);
                fgets(str, 100000, fp2);
                fmpz_poly_set_str(sk, str);
		fmpz_poly_mul(tmp, ct,  sk);
		fmpz_poly_add(m, m, tmp);
	}
        fclose(fp1);
        fclose(fp2);
	fmpz_poly_rem_basecase(m, m, fx);
	fmpz_poly_scalar_smod_fmpz(m, m, params->q);
	fmpz_poly_scalar_smod_fmpz(m, m, params->t);
        char *ms = fmpz_poly_get_str(m);
        printf("%s\n", ms);
        
        fmpz_clear(len);
        fmpz_poly_clear(sk);
        fmpz_poly_clear(m);
        fmpz_poly_clear(tmp);
        fmpz_poly_clear(ct);
        fmpz_poly_clear(fx);
        return 0;
}