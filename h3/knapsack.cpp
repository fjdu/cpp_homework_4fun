#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

#define NOT_YET_CALCULATED -999
#define NO_SOLUTION -9999

#define EXACT_MATCH

int ks(int *v, int *w, int *s, int n, int W, int WL, int iL) {
    if ((WL <= 0) || (iL < 0)) {
        return 0;
    }
    if (iL == 0) {
#ifdef EXACT_MATCH
        if (w[0] == WL) {
            return v[0];
        } else {
            return NO_SOLUTION;
        }
#else
        if (w[0] <= WL) {
            return v[0];
        } else {
            return 0;
        }
#endif
    }
    int idx = iL*(W+1) + WL;
    if (s[idx] != NOT_YET_CALCULATED) {
        return s[idx];
    }
    if (WL >= w[iL]) {
        int r = ks(v, w, s, n, W, WL-w[iL], iL-1);
        int r1;
        if (r != NO_SOLUTION) {
            r1 = r + v[iL];
        } else {
            r1 = NO_SOLUTION;
        }
        int r2 = ks(v, w, s, n, W, WL, iL-1);
        s[idx] = max(r1, r2);
    } else {
        s[idx] = ks(v, w, s, n, W, WL, iL-1);
    }
    //printf("%d: %d, %d\n", iL, WL, s[idx]);
    return s[idx];
}


int gcd(int u, int v)
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


int main(int argc, char *argv[]){
    string line = "";
    std::getline(std::cin, line, '\n');
    std::stringstream lineStream(line);
    int i;
    std::vector<int> values;
    while (lineStream >> i) {
        values.push_back(i);
    }
    //
    int n0 = values.size();
    int W = values[0], n = values[1];
    //if (n*2 != (n0-2)) {
    //    return 1;
    //}
    int *v, *w;
    v = new int[n];
    w = new int[n];
    int g = W;
    for (i=0; i<n; i++) {
        v[i] = values[2+2*i];
        w[i] = values[2+2*i+1];
        if (g > 1) {
            g = gcd(g, w[i]);
        }
        //printf("%d: %d, %d\n", i+1, v[i], w[i]);
    }
    if (g > 1) {
        W /= g;
        for (i=0; i<n; i++) {
            w[i] /= g;
        }
    }
    //
    int *s;
    s = new int[n*(W+1)];
    for (i=0; i<n*(W+1); i++) {
        s[i] = NOT_YET_CALCULATED;
    }
    printf("%d\n", g*max(0, ks(v, w, s, n, W, W, n-1)));
    return 0;
}
