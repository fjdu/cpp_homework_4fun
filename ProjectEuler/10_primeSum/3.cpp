// Based on: Lucy_Hedgehog https://projecteuler.net/thread=10;page=5

#include <cmath>
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>

using namespace std;

#define LINT long long
#define SINT long long

LINT primes(SINT N) {
    std::map <SINT, LINT> S;
    std::vector <SINT> V;

    SINT r = (SINT)sqrt(N);
    assert(r*r <= N and (r+1)*(r+1) > N);

    SINT nv = r + N/r - 1;

    for (SINT i=0; i<r; i++) {
        V.push_back(N/(i+1));
    }
    for (SINT i=r; i<nv; i++) {
        V.push_back(V.back() - 1);
    }

    for (SINT i=0; i<nv; i++) {
        S[V[i]] = V[i] * (V[i] + 1) / 2 - 1;
    }

    for (SINT p=2; p<=r; p++) {
        if (S[p] > S[p-1]) {
            LINT sp = S[p-1];
            LINT p2 = p*p;
            for (SINT i=0; i<nv; i++) {
                if (V[i] < p2) break;
                S[V[i]] -= p * (S[V[i]/p] - sp);
            }
        }
    }

    return S[N];
}

int main() {
    SINT N = 10000000000;
    cout << primes(N) << endl;
}
