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


void mat_mult(int **A, int **B, int **C, int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            C[i][j] = 0;
            for (int k=0; k<n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
            if (C[i][j] > MODBASE) {
                C[i][j] = C[i][j] % MODBASE;
            }
        }
    }
}


void matExp(int **A, int n, int k) {
    if (k <= 1) {
        return;
    }
    int **C;
    C = new int*[n];
    for (int i=0; i<n; i++) {
        C[i] = new int[n];
    }
    if (k & 1) {
        int **Atmp;
        Atmp = new int*[n];
        for (int i=0; i<n; i++) {
            Atmp[i] = new int[n];
            for (int j=0; j<n; j++) {
                Atmp[i][j] = A[i][j];
            }
        }
        matExp(A, n, k-1);
        mat_mult(A, Atmp, C, n);
        delete Atmp;
    } else {
        matExp(A, n, k>>1);
        mat_mult(A, A, C, n);
    }
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            A[i][j] = C[i][j];
        }
    }
    delete C;
}


int bigMod(int a, int b, int m) {
    if (a == 0 || b == 0) {
        return 0;
    }
    if (a == 1) {
        return b;
    }
    if (b == 1) {
        return a;
    }
    int c = (b >> 1);
    if ((c << 1) == b) {
        return  (bigMod(a, c, m) << 1) % m;
    } else {
        return ((bigMod(a, c, m) << 1) + a) % m;
    }
}

void mul_col(int *a, int *b, int *c, int n) {
    // a: the first row of the input matrix
    // b: any column vector with size n
    // c: output
    for (int i=0; i<n; i++) {
        c[i] = 0;
        for (int j=0; j<n; j++) {
            c[i] += (a[get_idx(i, j)] * b[j]);
            if (c[i] >= MODBASE) {
                c[i] = (c[i] % MODBASE);
            }
        }
    }
}


void mul_mat(int *a, int *b, int *c, int n, int k) {
    for (int i=k; i>1; i--) {
        mul_col(a, b, c, n);
        for (int j=0; j<n; j++) {
            b[j] = c[j];
        }
    }
}


void initialize(int *a, int *b, int n, int j) {
    for (int i=0; i<n; i++) {
        b[i]= a[get_idx(i, j)];
    }
}


int log2(int k) {
    int a = 0;
    while (k > 1) {
        a++;
        k = k >> 1;
    }
    return a;
}


int main(int argc, char *argv[]){
    //printf("%d\n", bigMod(64545, 58971, 144));
    //return 0;
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
    if (k == 0) {
        printf("1\n");
    }
    //
    if ((k > log2(k) * n) && (n < 1000)) {
        int **A;
        A = new int*[n];
        for (int i=0; i<n; i++) {
            A[i] = new int[n];
            for (int j=0; j<n; j++) {
                A[i][j] = values[3+get_idx(i, j)] % MODBASE;
                //printf("%d ", A[i][j]);
            }
            //printf("\n");
        }
        matExp(A, n, k);
        printf("%d\n", A[ii-1][jj-1]%MODBASE);
        //for (int i=0; i<n; i++) {
        //    for (int j=0; j<n; j++) {
        //        printf("%6d ", A[i][j]);
        //    }
        //    printf("\n");
        //}
    } else {
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
        initialize(a, b, n, jj-1);
        mul_mat(a, b, c, n, k);
        printf("%d\n", b[ii-1]%MODBASE);
    }
    return 0;
}
