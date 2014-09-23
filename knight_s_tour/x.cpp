#include <stdio.h>
#include <stdlib.h>

#define N_DIR 8

#define LONG long long

//bool get_next_cell(int **s, int idir, int nx, int ny, int ix, int iy, int *ix1, int *iy1) {
//    const int dx[N_DIR] = {1, 2,  2,  1, -1, -2, -2, -1};
//    const int dy[N_DIR] = {2, 1, -1, -2,  2,  1, -1, -2};
//    //if (idir >= N_DIR) {
//    //    return false;
//    //}
//    *ix1 = ix + dx[idir];
//    *iy1 = iy + dy[idir];
//    bool is_bad = \
//        (*ix1 < 0)   || (*iy1 < 0)   || \
//        (*ix1 >= nx) || (*iy1 >= ny) || \
//        (s[*ix1][*iy1] != 0);
//    return !is_bad;
//}


void do_chess(int ***s, int nz, int nx, int ny, int iz, int ix, int iy, \
              int nocc_this, LONG *n_success)
{
    // The current state is the istate-th element of s.
    // Assume the input chess state is not covered yet.
    const int dx[N_DIR] = {1, 2,  2,  1, -1, -2, -2, -1};
    const int dy[N_DIR] = {2, 1, -1, -2,  2,  1, -1, -2};
    int ix1, iy1, iz1, n_occ_next, idir;
    iz1 = iz + 1;
    //printf("%d\n", iz1);
    n_occ_next = nocc_this + 1;
    for (idir=0; idir<N_DIR; idir++) {
        ix1 = ix + dx[idir];
        iy1 = iy + dy[idir];
        bool is_bad = \
            (ix1 < 0)   || (iy1 < 0)   || \
            (ix1 >= nx) || (iy1 >= ny) || \
            (s[iz][ix1][iy1] != 0);
        //return !is_bad;
        //if (!get_next_cell(s[iz], idir, nx, ny, ix, iy, &ix1, &iy1)) {
        if (is_bad) {
            continue;
        }
        // Copy the state.
        for (int i=0; i<nx; i++) {
            for (int j=0; j<ny; j++) {
                s[iz1][i][j] = s[iz][i][j];
            }
        }
        s[iz1][ix1][iy1] = 1;
        if (n_occ_next < nz) {
            //printf("%d, %d, %d, %d, %d, %d\n", ix, iy, ix1, iy1, idir, n_occ_next);
            do_chess(s, nz, nx, ny, iz1, ix1, iy1, n_occ_next, n_success);
        } else {
            //printf("n_success = %llu\n", *n_success);
            *n_success += 1;
        }
    }
}


int main(int argc, char *argv[]) {
    int ***s;
    LONG n_success;
    int nx, ny, nz, ix0, iy0;
    //
    if (argc < 5) {
        printf("Usage:\n");
        printf("\t%s nx ny ix iy\n", argv[0]);
        return 1;
    }
    //
    nx = atoi(argv[1]);
    ny = atoi(argv[2]);
    ix0 = atoi(argv[3]);
    iy0 = atoi(argv[4]);
    nz = nx * ny;
    //
    s = new int**[nz];
    for (int i=0; i<nz; i++) {
        s[i] = new int*[nx];
        for (int j=0; j<nx; j++) {
            s[i][j] = new int[ny];
        }
    }
    //
    for (int i=0; i<nz; i++) {
        for (int j=0; j<nx; j++) {
            for (int k=0; k<ny; k++) {
                s[i][j][k] = 0;
            }
        }
    }
    s[0][ix0][iy0] = 1;
    n_success = 0;
    //
    do_chess(s, nz, nx, ny, 0, ix0, iy0, 1, &n_success);
    printf("%llu\n", n_success);
}
