#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


#define INT int
#define MODBASE 10000
#define NMAXIN 1024

INT calc_last_4digit(INT a, INT n) {
    if (n == 1) {
        return a % MODBASE;
    }
    if (n == 0) {
        return 1;
    }
    a = a % MODBASE;
    if (n%2 == 0) {
        INT b = (a*a) % MODBASE;
        if (a != b) {
            return calc_last_4digit(b, n>>1);
        } else {
            return a;
        }
    } else {
        return (a*calc_last_4digit(a, n-1)) % MODBASE;
    }
}


INT calc_last_4digit_list(INT *ns, int n1, int n2) {
    INT r = ns[n1]%MODBASE;
    for (int i=n1+1; i<=n2; i++) {
        r = calc_last_4digit(r, ns[i]);
    }
    return r;
}


int main(int argc, char *argv[]){
    //int s = 1;
    //for (int i=1; i<1000000; i++) {
    //    s = (s*2)%MODBASE;
    //    if (s == 9376) {
    //        printf("%d\n", i);
    //        break;
    //    }
    //}
    //s = 1;
    //for (int i=1; i<1000000; i++) {
    //    s = (s*3)%MODBASE;
    //    if (s == 1) {
    //        printf("%d\n", i);
    //        break;
    //    }
    //}
    //return 0;
    //for (int k=1000; k<MODBASE; k++) {
    //  if ((k*(k-1))%MODBASE == 0) {
    //      printf("%d\n", k);
    //  }
    //}
    //return 0;
    INT *ns;
    // http://stackoverflow.com/questions/1145831/get-list-of-numbers-from-stdin-and-tokenize-them
    string line = "";
    std::getline(std::cin, line, '\n');
    std::stringstream lineStream(line);
    int i;
    std::vector<int> values;
    while (lineStream >> i) {
        values.push_back(i);
    }
    int n = values.size();
    ns = new int[n];
    for (int i=0; i<n; i++) {
        ns[i] = values[i];
    }
    if (n==1) {
        printf("%d\n", ns[0]%MODBASE);
    } else {
        printf("%d\n", calc_last_4digit_list(ns, 0, n-1));
    }
}
