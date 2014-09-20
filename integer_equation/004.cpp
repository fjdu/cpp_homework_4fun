#include <stdio.h>
#include <stdlib.h>


int int_eq_count(int *coeff, int ibg, int ied, int b) {
    if (ied - ibg == 0) {
        if (coeff[ibg] == 0) {
            if (b == 0) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return ((b%coeff[ibg]) == 0) ? 1 : 0;
        }
    } else if (ied - ibg > 0) {
        if (coeff[ibg] > 0) {
            int nsol = 0;
            for (; b>=0; b-=coeff[ibg]) {
                nsol += int_eq_count(coeff, ibg+1, ied, b);
            }
            return nsol;
        } else if (coeff[ibg] == 0) {
            return int_eq_count(coeff, ibg+1, ied, b);
        } else {
            printf("Negative coefficient!\n");
            printf("coeff[%d] = %d\n", ibg, coeff[ibg]);
            return -1;
        }
    } else {
      return 0;
    }
}


int int_eq_count_nonzero(int *coeff, int ibg, int ied, int b) {
    if (ied - ibg == 0) {
        if (coeff[ibg] == 0) {
            if (b == 0) {
                // Let's agree that 0 * x = 0 has a single solution x = 1.
                return 1;
            } else {
                return 0;
            }
        } else {
            return (((b%coeff[ibg]) == 0) && (b >0)) ? 1 : 0;
        }
    } else if (ied - ibg > 0) {
        if (coeff[ibg] > 0) {
            int nsol = 0;
            for (b-=coeff[ibg]; b>=0; b-=coeff[ibg]) {
                nsol += int_eq_count_nonzero(coeff, ibg+1, ied, b);
            }
            return nsol;
        } else if (coeff[ibg] == 0) {
            return int_eq_count_nonzero(coeff, ibg+1, ied, b);
        } else {
            printf("Negative coefficient!\n");
            printf("coeff[%d] = %d\n", ibg, coeff[ibg]);
            return -1;
        }
    } else {
      return 0;
    }
}


int main(int argc, char *argv[]){
    int *coeffs, b;
    int ncoeff = argc - 2;
    coeffs = new int[ncoeff];
    for (int i=0; i<ncoeff; i++) {
        coeffs[i] = atoi(argv[i+1]);
        if (coeffs[i] <= 0) {
            printf("Nonpositive coefficient!\n");
            printf("coeffs[%d] = %d\n", i, coeffs[i]);
            return -1;
        }
    }
    b = atoi(argv[argc-1]);
    for (int i=0; i<ncoeff-1; i++) {
        printf("%d * x%d + ", coeffs[i], i+1);
    }
    printf("%d * x%d", coeffs[ncoeff-1], ncoeff);
    printf(" = %d\n", b);
    printf("Number of nonnegative solutions:\n");
    printf("\t%d\n", int_eq_count(coeffs, 0, ncoeff-1, b));
    printf("Number of positive solutions:\n");
    printf("\t%d\n", int_eq_count_nonzero(coeffs, 0, ncoeff-1, b));
    return 0;
}
