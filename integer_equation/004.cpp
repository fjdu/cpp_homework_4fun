#include <stdio.h>
#include <stdlib.h>

#define LONG long long


//LONG int_eq_count_nonzero(LONG *coeff, int ibg, int ied, LONG b) {
//    if (ied - ibg == 0) {
//        if (coeff[ibg] == 0) {
//            if (b == 0) {
//                // Let's agree that 0 * x = 0 has a single solution x = 1.
//                return 1;
//            } else {
//                return 0;
//            }
//        } else {
//            return (((b%coeff[ibg]) == 0) && (b >0)) ? 1 : 0;
//        }
//    } else if (ied - ibg > 0) {
//        if (coeff[ibg] > 0) {
//            LONG nsol = 0;
//            for (b-=coeff[ibg]; b>=0; b-=coeff[ibg]) {
//                nsol += int_eq_count_nonzero(coeff, ibg+1, ied, b);
//            }
//            return nsol;
//        } else if (coeff[ibg] == 0) {
//            return int_eq_count_nonzero(coeff, ibg+1, ied, b);
//        } else {
//            printf("Negative coefficient!\n");
//            printf("coeff[%d] = %llu\n", ibg, coeff[ibg]);
//            return -1;
//        }
//    } else {
//      return 0;
//    }
//}


LONG int_eq_count_divide(LONG *coeff, int ibg, int ied, LONG b) {
    if (ied > ibg + 1) {
        int imid = (ibg+ied) >> 1;
        LONG nsol = 0;
        for (LONG i=1; i<b; i++) {
            LONG nsol_R = int_eq_count_divide(coeff, imid+1, ied, b-i);
            if (nsol_R > 0) {
                nsol += int_eq_count_divide(coeff, ibg, imid, i) * nsol_R;
            }
        }
        nsol += int_eq_count_divide(coeff, ibg,   imid, b);
        nsol += int_eq_count_divide(coeff, imid+1, ied, b);
        return nsol;
    } else if (ied == ibg + 1) {
        LONG a1 = coeff[ibg];
        LONG a2 = coeff[ied];
        if (coeff[ibg] < coeff[ied]) {
            a1 = coeff[ied];
            a2 = coeff[ibg];
        }
#ifdef VERBOSE
        if (a2 < 1) {
            printf("Nonpositive coefficient!\n");
            printf("coeff[%d] = %llu, coeff[%d] = %llu\n", \
                ibg, coeff[ibg], ied, coeff[ied]);
            return -1;
        }
#endif
        if (a2 == 1) {
            return b/a1 + 1;
        }
        LONG nsol = 0;
        for (; b>=0; b-=a1) {
            nsol += !(b%a2);
        }
        return nsol;
    } else if (ied == ibg) {
        return !(b%coeff[ibg]);
    } else {
        printf("Wrong case!");
        printf("ibg = %d, ied = %d\n", ibg, ied);
        return -1;
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
            printf("coeffs[%d] = %llu\n", i, coeffs[i]);
            return -1;
        }
    }
    b = atol(argv[argc-1]);
    for (int i=0; i<ncoeff-1; i++) {
        printf("%llu * x%d + ", coeffs[i], i+1);
    }
    printf("%llu * x%d", coeffs[ncoeff-1], ncoeff);
    printf(" = %llu\n", b);
    printf("Number of nonnegative solutions:\n");
    printf("\t%llu\n", int_eq_count_divide(coeffs, 0, ncoeff-1, b));
    //printf("Number of positive solutions:\n");
    //printf("\t%d\n", int_eq_count_nonzero(coeffs, 0, ncoeff-1, b));
    return 0;
}
