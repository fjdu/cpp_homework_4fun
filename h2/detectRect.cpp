#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

#define INT int

void swap(INT *x, INT*y) {
    INT t=*x;
    *x = *y;
    *y = t;
}

void sorted(INT *nx, INT *ny, int *ind, int n1, int n2) {
    if (n1 >= n2) {
        return;
    }
    int imid = (n1+n2)>>1;
    //printf("%d,%d,%d\n", imid, n1, n2);
    INT pvtx = nx[imid], pvty = ny[imid];
    swap(&nx[imid], &nx[n2]);
    swap(&ny[imid], &ny[n2]);
    swap(&ind[imid], &ind[n2]);
    int idx = n1-1;
    for (int i=n1; i<n2; i++) {
        if ((nx[i] < pvtx) || \
            ((nx[i] == pvtx) && (ny[i] < pvty))) {
            idx++;
            swap(&nx[i], &nx[idx]);
            swap(&ny[i], &ny[idx]);
            swap(&ind[i], &ind[idx]);
        }
    }
    idx++;
    swap(&nx[idx], &nx[n2]);
    swap(&ny[idx], &ny[n2]);
    swap(&ind[idx], &ind[n2]);
    sorted(nx, ny, ind, n1, idx-1);
    sorted(nx, ny, ind, idx+1, n2);
}

bool has_rect(INT *mx, INT *my, INT *nx, INT *ny, int *ind, int n1, int n2) {
    if ((n1+3) > n2) {
        return false;
    }
    int i0 = ind[n1];
    for (int i=n1+1; i<=n2; i++) {
        if (mx[i] == mx[n1]) {
            continue;
        }
        if (my[i] != my[n1]) {
            return has_rect(mx, my, nx, ny, ind, i, n2);
        }
        int j0 = ind[i];
        for (int j=i0; j<=n2; j++) {
            if (ny[j] == ny[i0]) {
                continue;
            }
            if (nx[j] != nx[i0]) {
                return has_rect(mx, my, nx, ny, ind, n1+1, n2);
            }
            for (int k=j0; k<=n2; k++) {
                if ((nx[k] != nx[j0]) || (ny[k] > ny[j])) {
                    break;
                } else if (ny[j] == ny[k]) {
                    return true;
                }
            }
        }
    }
    return false;
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
    int n = values.size();
    if (n < 8) {
        printf("No\n");
        return 1;
    }
    int np = n/2;
    INT *nx, *ny, *mx, *my;
    int *ind;
    nx = new INT[np];
    ny = new INT[np];
    mx = new INT[np];
    my = new INT[np];
    ind = new int[np];
    for (int i=0; i<np; i++) {
        mx[i] = values[2*i];
        my[i] = values[2*i+1];
    }
    sorted(my, mx, ind, 0, np-1);
    for (int i=0; i<np; i++) {
        nx[i] = mx[i];
        ny[i] = my[i];
        ind[i] = i;
    }
    sorted(nx, ny, ind, 0, np-1);
    //
    //for (int i=0; i<np; i++) {
    //    printf("%d, %d\n", nx[i], ny[i]);
    //}
    //printf("\n%d,%d,%d\n\n", 0>>1, 6>>1, (0>>1)+(6>>1));
    //for (int i=0; i<np; i++) {
    //    printf("%d, %d\n", mx[i], my[i]);
    //}
    //
    //printf("%p\n", (void *)nx[0]);
    //printf("%p\n", (void *)nx);
    //printf("%p\n", (void *)&nx);
    //printf("%p\n", (void *)&nx[0]);
    //printf("%p\n", (void *)nx[1]);
    //printf("%p\n", (void *)&nx[1]);
    bool has_re = has_rect(mx, my, nx, ny, ind, 0, np-1);
    if (has_re) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }
    return 0;
}
