#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


#define INT int

//#define NAIVE

INT calc_max_sum_enum(INT *ns, int n1, int n2) {
    // Naive approach.
    INT s;
    s = ns[n1];
    for (int i=n1; i<=n2; i++) {
        INT t = ns[i];
        for (int j=i+1; j<=n2; j++) {
            t += ns[j];
            s = max(s, t);
        }
    }
    return s;
}


INT partial_sum_L(INT *ns, int n1, int n2) {
    INT s = ns[n2];
    INT t = s;
    for (int i=n2-1; i>=n1; i--) {
        s += ns[i];
        t = max(t, s);
    }
    return t;
}


INT partial_sum_R(INT *ns, int n1, int n2) {
    INT s = ns[n1];
    INT t = s;
    for (int i=n1+1; i<=n2; i++) {
        s += ns[i];
        t = max(t, s);
    }
    return t;
}


INT calc_max_sum_plus_minus(INT *ns, int n1, int n2) {
    if (n1 >= n2) {
        return ns[n2];
    }
    if (n1+1 == n2) {
        return max(max(ns[n1], ns[n2]), ns[n1]+ns[n2]);
    }
    int nmid;
    nmid = (n1 + n2) >> 1;
    INT s1 = calc_max_sum_plus_minus(ns, n1, nmid-1);
    INT s2 = calc_max_sum_plus_minus(ns, nmid+1, n2);
    INT sL = partial_sum_L(ns, n1, nmid-1);
    INT sR = partial_sum_R(ns, nmid+1, n2);
    s1 = max(s1, s2);
    s1 = max(s1, ns[nmid]);
    s1 = max(s1, sL+ns[nmid]);
    s1 = max(s1, sR+ns[nmid]);
    s1 = max(s1, sL+sR+ns[nmid]);
    return s1;
}


void reduce_plus_minus(INT *ns, int n, int *nseg) {
    int ibg = 0;
    int ied = n;
    if (ns[0] <= 0) {
        ibg = n + 1;
        for (int i=1; i<=n; i++) {
            if (ns[i] > 0) {
                ibg = i;
                break;
            }
        }
    }
    if (ns[n] <= 0) {
        ied = -1;
        for (int i=n-1; i>=0; i--) {
            if (ns[i] > 0) {
                ied = i;
                break;
            }
        }
    }
    if ((ibg > n) || (ied < 0)) {
        *nseg = -1;
        return;
    }
    *nseg = 0;
    ns[*nseg] = ns[ibg];
    for (int i=ibg+1; i<=ied; i++) {
        //printf("%d,%d\n", *nseg, i);
        if (ns[i] * ns[i-1] < 0) {
            *nseg += 1;
            ns[*nseg] = ns[i];
        } else {
            ns[*nseg] += ns[i];
        }
    }
    return;
}

INT get_max(INT *ns, int n) {
    INT s = ns[0];
    for (int i=1; i<=n; i++) {
        s = max(s, ns[i]);
    }
    return s;
}


int main(int argc, char *argv[]){
    INT *ns;
    string line = "";
    std::getline(std::cin, line, '\n');
    std::stringstream lineStream(line);
    int i;
    std::vector<int> values;
    while (lineStream >> i) {
        values.push_back(i);
    }
    int n = values.size();
    bool all_negative = true;
    ns = new INT[n];
    for (int i=0; i<n; i++) {
        ns[i] = values[i];
        if (ns[i] > 0) {
            all_negative = false;
        }
    }
    if (n <= 0) {
        printf("%d\n", 0);
    } else {
        if (all_negative) {
            printf("%d\n", get_max(ns, n-1));
            return 0;
        }
        int nseg;
        reduce_plus_minus(ns, n-1, &nseg);
#ifdef NAIVE
        printf("%d\n", calc_max_sum_enum(ns, 0, nseg));
#else
        if (nseg < 0) {
            printf("%d\n", 0);
            return 0;
        }
        printf("%d\n", calc_max_sum_plus_minus(ns, 0, nseg));
#endif
    }
}
