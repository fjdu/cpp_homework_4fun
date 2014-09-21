#include <stdio.h>
#include <stdlib.h>

#define LONG long long


LONG int_eq_count(LONG *coeff, int ibg, int ied, LONG b) {
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
            LONG nsol = 0;
            for (; b>=0; b-=coeff[ibg]) {
                nsol += int_eq_count(coeff, ibg+1, ied, b);
            }
            return nsol;
        } else if (coeff[ibg] == 0) {
            return int_eq_count(coeff, ibg+1, ied, b);
        } else {
            printf("Negative coefficient!\n");
            printf("coeff[%d] = %lu\n", ibg, coeff[ibg]);
            return -1;
        }
    } else {
      return 0;
    }
}


LONG int_eq_count_nonzero(LONG *coeff, int ibg, int ied, LONG b) {
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
            LONG nsol = 0;
            for (b-=coeff[ibg]; b>=0; b-=coeff[ibg]) {
                nsol += int_eq_count_nonzero(coeff, ibg+1, ied, b);
            }
            return nsol;
        } else if (coeff[ibg] == 0) {
            return int_eq_count_nonzero(coeff, ibg+1, ied, b);
        } else {
            printf("Negative coefficient!\n");
            printf("coeff[%d] = %lu\n", ibg, coeff[ibg]);
            return -1;
        }
    } else {
      return 0;
    }
}


LONG int_eq_count_2(LONG *coeff, int ibg, int ied, LONG b) {
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
    } else if (ied - ibg == 1) {
        if (coeff[ibg] == 1) {
            return b/coeff[ied]+1;
        } else if (coeff[ied] == 1) {
            return b/coeff[ibg]+1;
        } else if (coeff[ibg] > 0) {
            LONG nsol = 0;
            for (; b>=0; b-=coeff[ibg]) {
                nsol += int_eq_count_2(coeff, ibg+1, ied, b);
            }
            return nsol;
        } else if (coeff[ibg] == 0) {
            return int_eq_count_2(coeff, ibg+1, ied, b);
        } else {
            printf("Negative coefficient!\n");
            printf("coeff[%d] = %lu\n", ibg, coeff[ibg]);
            return -1;
        }
    } else {
      printf("Too many coefficients!\n");
      return -1;
    }
}


LONG int_eq_count_divide(LONG *coeff, int ibg, int ied, LONG b) {
    if (ied - ibg > 1) {
        int imid = (ibg+ied)/2;
        LONG nsol = 0;
        for (LONG i=1; i<b; i++) {
            nsol += int_eq_count_divide(coeff, ibg, imid, i) * \
                    int_eq_count_divide(coeff, imid+1, ied, b-i);
        }
        nsol += int_eq_count_divide(coeff, ibg,    imid, b);
        nsol += int_eq_count_divide(coeff, imid+1, ied,  b);
        return nsol;
    } else {
        return int_eq_count_2(coeff, ibg, ied, b);
    }
}


int main(int argc, char *argv[]){
    LONG *coeffs, b;
    int ncoeff = argc - 2;
    coeffs = new LONG[ncoeff];
    for (int i=0; i<ncoeff; i++) {
        coeffs[i] = atol(argv[i+1]);
        if (coeffs[i] <= 0) {
            printf("Nonpositive coefficient!\n");
            printf("coeffs[%d] = %lu\n", i, coeffs[i]);
            return -1;
        }
    }
    b = atol(argv[argc-1]);
    for (int i=0; i<ncoeff-1; i++) {
        printf("%lu * x%d + ", coeffs[i], i+1);
    }
    printf("%lu * x%d", coeffs[ncoeff-1], ncoeff);
    printf(" = %lu\n", b);
    printf("Number of nonnegative solutions:\n");
    printf("\t%lu\n", int_eq_count_divide(coeffs, 0, ncoeff-1, b));
    //printf("Number of positive solutions:\n");
    //printf("\t%d\n", int_eq_count_nonzero(coeffs, 0, ncoeff-1, b));
    return 0;
}
