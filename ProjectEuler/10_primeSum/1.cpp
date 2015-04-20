#include <stdio.h>
#include <math.h>

#define INT long long

class primes {
    public:

    INT *store;
    INT nprime;

    primes(INT npmax) {
        nprime = 0;
        store = new INT[npmax];
    }
    ~primes() {
        delete store;
    }

    void newcomer(INT n) {
        if (is_prime_Erato(n)) {
            store[nprime] = n;
            nprime += 1;
        }
    }

    bool is_prime_Erato(INT n) {
        for (INT i=0; i< nprime; i++) {
            if ((n % store[i] == 0)) {
                return false;
            } else if (store[i] * store[i] > n) {
                return true;
            }
        }
        return true;
    }
};

int main() {
    INT N = 2000000;
    INT np_est = (INT)(2 * N / log(N));
    primes ps(np_est);
    ps.newcomer(2);
    for (INT i=3; i<N; i+=2) {
        ps.newcomer(i);
    }
    INT sum = 0;
    for (INT i=0; i<ps.nprime; i++) {
        sum += ps.store[i];
    }
    printf("%20lld\n", sum);
}
