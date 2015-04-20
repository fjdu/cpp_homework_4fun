// https://projecteuler.net/problem=512

LINT f(SINT n) {
}


LINT g(SINT n) {
    LINT sum = 0;
    for (SINT i=1; i<=n; i++) {
        sum += f(i);
    }
    return sum;
}
