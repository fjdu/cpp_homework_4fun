#include <stdio.h>
#include <stdlib.h>

#define N_DIR 8

#define LONG long long


int get_n_neib(int *s, int nz, int nx, int ny, int iz, int ix, int iy) {
    const int dx[N_DIR] = {1, 2,  2,  1, -1, -2, -2, -1};
    const int dy[N_DIR] = {2, 1, -1, -2,  2,  1, -1, -2};
    int nn = 0, ix1, iy1, itmp, itmp2;
    itmp = nz * iz;
    for (int idir=0; idir<N_DIR; idir++) {
        ix1 = ix + dx[idir];
        iy1 = iy + dy[idir];
        itmp2 = itmp + ix1 * ny + iy1;
        if ((ix1 < 0)   || (iy1 < 0)   || \
            (ix1 >= nx) || (iy1 >= ny) || \
            (s[itmp2] != 0)) {
            continue;
        }
        nn += 1;
    }
    return nn;
}

void do_chess(int *s, int nz, int nx, int ny, int iz, int ix, int iy, \
              int nocc_this, LONG *n_success)
{
    // The current state is the istate-th element of s.
    // Assume the input chess state is not covered yet.
    const int dx[N_DIR] = {1, 2,  2,  1, -1, -2, -2, -1};
    const int dy[N_DIR] = {2, 1, -1, -2,  2,  1, -1, -2};
    int ix1, iy1, iz1, n_occ_next, idir;
    int itmp, itmp1, itmp2;
    iz1 = iz + 1;
    itmp = nz * iz;
    itmp1 = itmp + nz;
    n_occ_next = nocc_this + 1;
    for (idir=0; idir<N_DIR; idir++) {
        ix1 = ix + dx[idir];
        iy1 = iy + dy[idir];
        itmp2 = itmp + ix1 * ny + iy1;
        if ((ix1 < 0)   || (iy1 < 0)   || \
            (ix1 >= nx) || (iy1 >= ny) || \
            (s[itmp2] != 0)) {
            continue;
        }
        // Copy the state.
        for (int i=0; i<nz; i++) {
            s[itmp1 + i] = s[itmp + i];
        }
        s[itmp1 + ix1*ny + iy1] = 1;
        if (n_occ_next < nz) {
            //
            bool stop_try = false;
            if ((8*n_occ_next > nz) && (n_occ_next < nz-1)) {
                int iiy = itmp1;
                for (int i=0; i<nx; i++) {
                    for (int j=0; j<ny; j++) {
                        if (s[iiy + j] == 0) {
                            if (get_n_neib(s, nz, nx, ny, iz1, i, j) == 0) {
                                stop_try = true;
                                break;
                            }
                        }
                    }
                    iiy += ny;
                }
            }
            if (stop_try) {
                continue;
            }
            //
            do_chess(s, nz, nx, ny, iz1, ix1, iy1, n_occ_next, n_success);
        } else {
            *n_success += 1;
        }
    }
}


int main(int argc, char *argv[]) {
    int *s;
    LONG n_success;
    int nx, ny, nz, ix0, iy0, nxyz;
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
    nxyz = nx * ny * nz;
    //
    s = new int[nxyz];
    //
    for (int i=0; i<nxyz; i++) {
        s[i] = 0;
    }
    s[ix0*ny + iy0] = 1;
    n_success = 0;
    //
    do_chess(s, nz, nx, ny, 0, ix0, iy0, 1, &n_success);
    printf("%llu\n", n_success);
}
