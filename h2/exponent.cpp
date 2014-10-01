#include <stdio.h>
#include <stdlib.h>

#define LONG int
#define MODBASE 10000

LONG calc_last_4digit(LONG a, LONG n) {
    if (n == 1) {
        return a % MODBASE;
    }
    if (n == 0) {
        return 1;
    }
    a = a % MODBASE;
    if (n%2 == 0) {
        return calc_last_4digit((a*a) % MODBASE, n>>1);
    } else {
        return (a*calc_last_4digit(a, n-1)) % MODBASE;
    }
}


LONG calc_last_4digit_list(LONG *ns, int n1, int n2) {
    if (n1+1 == n2) {
        return calc_last_4digit(ns[n1], ns[n2]);
    } else {
        return calc_last_4digit(calc_last_4digit_list(ns, n1, n2-1), ns[n2]);
    }
}


int main(int argc, char *argv[]){
    LONG *ns;
    int n = argc - 1;
    if (n < 2) {
        printf("At least two arguments needed!\n");
        return 1;
    }
    ns = new LONG[n];
    for (int i=0; i<n; i++) {
        ns[i] = atol(argv[i+1]);
    }
    printf("%d\n", calc_last_4digit_list(ns, 0, n-1));
}
