#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


#define INT int


INT calc_max_sum(INT *ns, int n1, int n2) {
    if (n1 >= n2) {
        return ns[n2];
    }
    if (ns[n1] >= 0) {
        INT r, s, t;
        s = ns[n1];
        int i, j;
        for (i=n1+1; i<=n2; i++) {
            if (ns[i] >= 0) {
                s += ns[i];
            } else {
                break;
            }
        }
        if ((i<=n2) && (ns[i] < 0)) {
            t = 0;
            for (j=i; j<=n2; j++) {
                if (ns[j] <= 0) {
                    t += ns[j];
                } else {
                    break;
                }
            }
            if ((j<=n2) && (ns[j] > 0)) {
                r = calc_max_sum(ns, j, n2);
                return max(max(s, s+t+r), r);
            } else {
                return s;
            }
        } else {
            return s;
        }
    } else {
        INT s = ns[n1];
        int i;
        for (i=n1+1; i<=n2; i++) {
            s = max(s, ns[i]);
            if (ns[i] > 0) {
                break;
            }
        }
        return max(s, calc_max_sum(ns, i, n2));
    }
}


int main(int argc, char *argv[]){
    //{
    //    int i;
    //    for (i=0; i<=2; i++) {if (i==2) break;};
    //    printf("%d\n", i);
    //    return 0;
    //}
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
    ns = new INT[n];
    for (int i=0; i<n; i++) {
        ns[i] = values[i];
    }
    if (n <= 0) {
        printf("%d\n", 0);
    } else {
        printf("%d\n", calc_max_sum(ns, 0, n-1));
    }
}
