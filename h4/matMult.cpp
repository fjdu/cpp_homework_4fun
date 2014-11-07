#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

#define MODBASE 10000

inline int get_idx(int i, int j) {
    // i: row index, starting from 0
    // j: column index, starting from 0
    // Return the index in the first row
    return (j >= i) ? (j-i) : (i-j);
}


void mul_col(int *a, int *b, int *c, int n) {
    // a: the first row of the input matrix
    // b: any column vector with size n
    // c: output
    for (int i=0; i<n; i++) {
        c[i] = 0;
        for (int j=0; j<n; j++) {
            int idx = get_idx(i, j);
            c[i] += ((a[idx]%MODBASE) * (b[j]%MODBASE)) % MODBASE;
        }
    }
}


void mul_mat(int *a, int *b, int *c, int n, int k) {
    if (k < 2) {
        return;
    }
    mul_col(a, b, c, n);
    for (int i=0; i<n; i++) {
        b[i] = c[i];
    }
    mul_mat(a, b, c, n, k-1);
}


void initialize(int *a, int *b, int n, int j) {
    for (int i=0; i<n; i++) {
        b[i]= a[get_idx(i, j)];
    }
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
    int k = values[0];
    int ii = values[1];
    int jj = values[2];
    int n = values.size() - 3;
    if ((ii <= 0) || (jj <= 0) || (ii > n) || (jj > n) || (k < 0)) {
        return 1;
    }
    //printf("%d, %d, %d, %d\n", k, ii, jj, n);
    int *a, *b, *c;
    a = new int[n];
    b = new int[n];
    c = new int[n];
    for (i=0; i<n; i++) {
        a[i] = values[3+i] % MODBASE;
        //printf("%d: %d\n", i, a[i]);
    }
    //for (i=0; i<n; i++) {
    //    for (int j=0; j<n; j++) {
    //        printf("%d ", get_idx(i, j) + 1);
    //    }
    //    printf("\n");
    //}
    //
    if (k == 0) {
        printf("1\n");
    } else {
        initialize(a, b, n, jj-1);
        mul_mat(a, b, c, n, k);
        printf("%d\n", b[ii-1]%MODBASE);
    }
    return 0;
}
