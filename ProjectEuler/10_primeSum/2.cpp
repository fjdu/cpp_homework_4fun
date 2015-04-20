#include <stdio.h>
#include <math.h>

#define INT long long
#define INTS int

class primes {
    public:

    bool *store;
    INTS nmax;

    primes(INTS nmax_) {
        nmax = nmax_;
        store = new bool[nmax];
        store[0] = false;
        store[1] = true;
        for (INTS i=3; i<nmax; i+=2) {
            store[i-1] = true;
            store[i] = false;
        }
    }
    ~primes() {
        delete store;
    }

    void EratoShieve() {
       INTS sqrtn = (INTS)sqrt(nmax)+1;
       for (INTS i=0; i<=sqrtn; i++) {
         if (store[i]) {
            INTS inc = (i+1)*2;
            for (INTS j=(i+1)*(i+1)-1; j<nmax; j+=inc) {
                store[j] = false;
            }
         }
       }
    }
};

int main() {
    INTS N = 1000000000;
    primes ps(N);
    ps.EratoShieve();
    INT sum = 0;
    for (INTS i=0; i<ps.nmax; i++) {
        if (ps.store[i]) {
            sum += (i+1);
        }
    }
    printf("%20lld\n", sum);
}
