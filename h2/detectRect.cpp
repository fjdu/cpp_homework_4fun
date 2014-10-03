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

void sort_segments(INT *nx, int *ibg, int *ied, int n1, int n2) {
    if (n1 >= n2) {
        return;
    }
    int imid = (n1+n2) >> 1;
    int pvtx = nx[ibg[imid]], pvty = nx[ied[imid]];
    swap(&ibg[imid], &ibg[n2]);
    swap(&ied[imid], &ied[n2]);
    int idx = n1-1;
    for (int i=n1; i<n2; i++) {
        if ((nx[ibg[i]] < pvtx) || \
            (nx[ibg[i]] == pvtx) && (nx[ied[i]] < pvty)) {
            idx++;
            swap(&ibg[i], &ibg[idx]);
            swap(&ied[i], &ied[idx]);
        }
    }
    idx++;
    swap(&ibg[idx], &ibg[n2]);
    swap(&ied[idx], &ied[n2]);
    sort_segments(nx, ibg, ied, n1, idx-1);
    sort_segments(nx, ibg, ied, idx+1, n2);
}


bool has_no_common_point(INT *mx, int n, int bg1, int ed1, int bg2, int ed2, int *imc1, int *imc2) {
    if ((bg1 > ed1) || (bg2 > ed2) || (ed1 > n) || (ed2 > n)) {
        return true;
    }
    if ((mx[ed1] < mx[bg2]) || (mx[ed2] < mx[bg1])) {
        return true;
    }
    if (mx[bg1] < mx[bg2]) {
        return has_no_common_point(mx, n, bg1+1, ed1, bg2, ed2, imc1, imc2);
    } else if (mx[bg1] > mx[bg2]) {
        return has_no_common_point(mx, n, bg1, ed1, bg2+1, ed2, imc1, imc2);
    } else {
        *imc1 = bg1;
        *imc2 = bg2;
        return false;
    }
}


bool has_rect(INT *mx, int n, int *ibg, int *ied, int nseg) {
    for (int i=0; i<nseg; i++) {
        for (int j=i+1; j<=nseg; j++) {
            if (mx[ibg[j]] > mx[ied[i]]) {
                // Asumming the segments are sorted.
                break;
            }
            int imatch1, imatch2;
            if (has_no_common_point(mx, n, ibg[i], ied[i], ibg[j], ied[j], \
                                    &imatch1, &imatch2)) {
                continue;
            }
            if (has_no_common_point(mx, n, imatch1+1, ied[i], imatch2+1, ied[j], \
                                    &imatch1, &imatch2)) {
                continue;
            } else {
                return true;
            }
        }
    }
    return false;
}


void get_line_segments(INT *nx, INT *ny, int n, int *ibg, int *ied, int *nseg) {
    // Assuming nx and ny are sorted, with x as the first direction, and y as
    // the second direction.
    *nseg = -1;
    if ((nx[0] != nx[1]) && (ny[0] == ny[1])) {
        (*nseg)++;
        ibg[*nseg] = 0;
    }
    for (int i=1; i<n; i++) {
        if (((nx[i-1] == nx[i]) || (ny[i-1] != ny[i])) && \
            ((nx[i] != nx[i+1]) && (ny[i] == ny[i+1]))) {
            (*nseg)++;
            ibg[*nseg] = i;
            continue;
        }
        if (((nx[i+1] == nx[i]) || (ny[i+1] != ny[i])) && \
            ((nx[i] != nx[i-1]) && (ny[i] == ny[i-1]))) {
            ied[*nseg] = i;
        }
    }
    if ((nx[n] != nx[n-1]) && (ny[n] == ny[n-1])) {
        ied[*nseg] = n;
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
    int n = values.size();
    if (n < 8) {
        printf("No\n");
        return 1;
    }
    //
    int np = n/2;
    INT *mx, *my;
    int *ibg, *ied;
    mx = new INT[np];
    my = new INT[np];
    ibg = new int[np];
    ied = new int[np/2];
    for (int i=0; i<np; i++) {
        mx[i] = values[2*i];
        my[i] = values[2*i+1];
    }
    //
    int nseg;
    bool has_re;
    sorted(my, mx, ibg, 0, np-1);
    get_line_segments(mx, my, np-1, ibg, ied, &nseg);
    if (nseg < 1) {
        printf("No\n");
        return 0;
    }
    if (nseg * nseg < np) {
        sort_segments(mx, ibg, ied, 0, nseg);
        has_re = has_rect(mx, np-1, ibg, ied, nseg);
        //for (i=0; i<=np-1; i++) {
        //    printf("%d,%d\n", mx[i], my[i]);
        //}
        //printf("nseg=%d\n", nseg+1);
    } else {
        sorted(mx, my, ibg, 0, np-1);
        get_line_segments(my, mx, np-1, ibg, ied, &nseg);
        sort_segments(my, ibg, ied, 0, nseg);
        has_re = has_rect(my, np-1, ibg, ied, nseg);
    }
    //
    if (has_re) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }
    return 0;
}
