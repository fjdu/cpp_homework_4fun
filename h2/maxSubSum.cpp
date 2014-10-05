#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

#define INT int

//#define NAIVE

INT max_sum_Kadane(INT *ns, int n1, int n2) {
    INT s=0, t=0;
    for (int i=n1; i<=n2; i++) {
        s = max(0, s + ns[i]);
        t = max(s, t);
    }
    return t;
}


INT get_max(INT *ns, int n) {
    INT s = ns[0];
    for (int i=1; i<=n; i++) {
        s = max(s, ns[i]);
    }
    return s;
}


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
#ifdef NAIVE
        printf("%d\n", calc_max_sum_enum(ns, 0, n-1));
#else
        printf("%d\n", max_sum_Kadane(ns, 0, n-1));
#endif
    }
}
