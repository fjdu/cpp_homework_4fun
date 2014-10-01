#include <stdio.h>
#include <stdlib.h>

#define INT int
#define MODBASE 10000

INT calc_last_4digit(INT a, INT n) {
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


INT calc_last_4digit_list(INT *ns, int n1, int n2) {
    if (n1+1 == n2) {
        return calc_last_4digit(ns[n1], ns[n2]);
    } else {
        return calc_last_4digit(calc_last_4digit_list(ns, n1, n2-1), ns[n2]);
    }
}


int main(int argc, char *argv[]){
    INT *ns;
    int n = argc - 1;
    if (n < 2) {
        printf("At least two arguments needed!\n");
        return 1;
    }
    ns = new INT[n];
    for (int i=0; i<n; i++) {
        ns[i] = atoi(argv[i+1]);
    }
    printf("%d\n", calc_last_4digit_list(ns, 0, n-1));
}
