#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
        clock_t start1, finish1, start4, finish4, start5, finish5, start6, finish6, start7, finish7;
        double  dur1,dur4,dur5,dur6,dur7;
        start1=clock();
        system("./keygen 1024 2 144115188076060673 2 > sk.txt");
        finish1=clock();
        dur1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
        printf( "keygen: %f seconds\n", dur1);
         
         start4=clock();
         system("./enc 1024 2 144115188076060673 2 \"3  1 0 1\" sk.txt > ct.txt");
         finish4=clock();
         dur4 = (double)(finish4 - start4) / CLOCKS_PER_SEC;
         printf( "encrypt: %f seconds\n", dur4);

        start5=clock();
        system("./mul 1024 2 144115188076060673 2 ct.txt ct.txt > nct.txt");
        finish5=clock();
        dur5 = (double)(finish5 - start5) / CLOCKS_PER_SEC;
        printf( "mul: %f seconds\n", dur5);

        start6=clock();
        system("./add 1024 2 144115188076060673 2 ct.txt ct.txt > nct1.txt");
        finish6=clock();
        dur6 = (double)(finish6 - start6) / CLOCKS_PER_SEC;
        printf( "add: %f seconds\n", dur6);

        start7=clock();
        system("./dec 1024 2 144115188076060673 2 nct.txt sk.txt");
        finish7=clock();
        dur7 = (double)(finish7 - start7) / CLOCKS_PER_SEC;
        printf( "decrypt: %f seconds\n", dur7);
        return 0;
}