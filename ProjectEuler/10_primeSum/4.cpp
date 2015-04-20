// Based on: Lucy_Hedgehog https://projecteuler.net/thread=10;page=5

#include <math.h>
#include <stdio.h>
#include <assert.h>

#define LINT long long

inline LINT V2IDX(LINT v, LINT N, LINT Ndr, LINT nv) {
    return v >= Ndr ? (N/v - 1) : (nv - v);
}

LINT primesum(LINT N) {
    LINT *S;
    LINT *V;

    LINT r = (LINT)sqrt(N);
    LINT Ndr = N/r;

    assert(r*r <= N and (r+1)*(r+1) > N);

    LINT nv = r + Ndr - 1;

    V = new LINT[nv];
    S = new LINT[nv];

    for (LINT i=0; i<r; i++) {
        V[i] = N/(i+1);
    }
    for (LINT i=r; i<nv; i++) {
        V[i] = V[i-1] - 1;
    }

    for (LINT i=0; i<nv; i++) {
        S[i] = V[i] * (V[i] + 1) / 2 - 1;
    }

    for (LINT p=2; p<=r; p++) {
        if (S[nv-p] > S[nv-p+1]) {
            LINT sp = S[nv-p+1];
            LINT p2 = p*p;
            for (LINT i=0; i<nv; i++) {
                if (V[i] >= p2) {
                    S[i] -= p * (S[V2IDX(V[i]/p, N, Ndr, nv)] - sp);
                } else {
                    break;
                }
            }
        }
    }

    return S[0];
}

int main() {
    LINT N = 1000000000;
    printf("%lld\n", primesum(N));
}
