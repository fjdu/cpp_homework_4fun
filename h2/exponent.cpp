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
        return calc_last_4digit((a*a) % MODBASE, n>>1);
    } else {
        return (a*calc_last_4digit(a, n-1)) % MODBASE;
    }
}


INT calc_last_4digit_list(INT *ns, int n1, int n2) {
    if (n1+1 == n2) {
        return calc_last_4digit(ns[n1], ns[n2]);
    } else {
        return calc_last_4digit(calc_last_4digit_list(ns, n1, n2-1), ns[n2]);
    }
}


int main(int argc, char *argv[]){
    INT *ns;
    //
    //char s_in[NMAXIN];
    //int n = argc - 1;
    //if (n < 2) {
    //    printf("At least two arguments needed!\n");
    //    return 1;
    //}
    //ns = new INT[n];
    //for (int i=0; i<n; i++) {
    //    ns[i] = atoi(argv[i+1]);
    //}
    //
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
    printf("%d\n", calc_last_4digit_list(ns, 0, n-1));
}
