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


LONG gcd(LONG u, LONG v)
//http://en.wikipedia.org/wiki/Binary_GCD_algorithm
{
    // simple cases (termination)
    if (u == v)
        return u;
 
    if (u == 0)
        return v;
 
    if (v == 0)
        return u;
 
    // look for factors of 2
    if (~u & 1) // u is even
    {
        if (v & 1) // v is odd
            return gcd(u >> 1, v);
        else // both u and v are even
            return gcd(u >> 1, v >> 1) << 1;
    }
 
    if (~v & 1) // u is odd, v is even
        return gcd(u, v >> 1);
 
    // reduce larger argument
    if (u > v)
        return gcd((u - v) >> 1, v);
 
    return gcd((v - u) >> 1, u);
}



//LONG gcd_ext(LONG u, LONG v, LONG *x0, LONG *y0) {
//    LONG a, b;
//    if (u >= v) {
//        a = u;
//        b = v;
//    } else {
//        a = v;
//        b = u;
//    }
//    LONG x = a/b;
//    LONG r = a - b * x;
//    if (r == 0) {
//        return b;
//    } else {
//        return gcd_ext(b, r, x0, y0);
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
        LONG a1, a2;
        if (coeff[ibg] <= coeff[ied]) {
            a1 = coeff[ied];
            a2 = coeff[ibg];
        } else {
            a1 = coeff[ibg];
            a2 = coeff[ied];
        }
#ifdef DEBUG
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
        //
        LONG g = gcd(a1, a2);
        if (b%g) {
            return 0;
        }
        LONG x0, y0;
        for (x0=0; x0<a2; x0++) {
            if (!((a1 * x0 - g)%a2)) {
                y0 = (a1 * x0 - g) / a2;
                break;
            }
        }
        if ((b*y0) % a1 == 0) {
            return b*x0/a2 - b*y0/a1 + 1;
        } else {
            return b*x0/a2 - b*y0/a1;
        }
        //
        //LONG nsol = 0;
        //for (; b>=0; b-=a1) {
        //    nsol += !(b%a2);
        //}
        //return nsol;
    } else if (ied == ibg) {
        return !(b%coeff[ibg]);
#ifdef DEBUG
    } else {
        printf("Wrong case!");
        printf("ibg = %d, ied = %d\n", ibg, ied);
        return -1;
#endif
    }
}


int main(int argc, char *argv[]){
    LONG *coeff, b;
    int ncoeff = argc - 2;
    coeff = new LONG[ncoeff];
    for (int i=0; i<ncoeff; i++) {
        coeff[i] = atol(argv[i+1]);
        if (coeff[i] <= 0) {
#ifdef VERBOSE
            printf("Nonpositive coefficient!\n");
            printf("coeff[%d] = %llu\n", i, coeff[i]);
#endif
            return -1;
        }
    }
    b = atol(argv[argc-1]);
    if (b <= 0) {
#ifdef VERBOSE
        printf("Nonpositive total!\n");
        printf("b = %llu\n", b);
#endif
        return -1;
    }
#ifdef VERBOSE
    for (int i=0; i<ncoeff-1; i++) {
        printf("%llu * x%d + ", coeff[i], i+1);
    }
    printf("%llu * x%d", coeff[ncoeff-1], ncoeff);
    printf(" = %llu\n", b);
    printf("Number of nonnegative solutions:\n");
#endif
    for (int i=0; i<ncoeff; i++) {
        for (int j=0; j<i; j++) {
            if ((coeff[j]!=1) && \
                ((coeff[j] < coeff[i]) || (coeff[i]==1))) {
                LONG tmp = coeff[i];
                coeff[i] = coeff[j];
                coeff[j] = tmp;
            }
        }
    }
    printf("\t%llu\n", int_eq_count_divide(coeff, 0, ncoeff-1, b));
    //printf("Number of positive solutions:\n");
    //printf("\t%d\n", int_eq_count_nonzero(coeff, 0, ncoeff-1, b));
    return 0;
}
