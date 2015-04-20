#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NSURF_DICE 6

int main(int argc, char *argv[]){
    int ngoal, n_throw_max, n_trial;
    if (argc < 3) {
        ngoal = 22;
        n_trial = 10000000;
    } else {
        ngoal = atoi(argv[1]);
        n_trial = atoi(argv[2]);
    }
    n_throw_max = ngoal + 1;
    int *rec, *nthrow;
    rec = new int[NSURF_DICE];
    nthrow = new int[n_throw_max];
    for (int i=0; i<NSURF_DICE; i++) {
        rec[i] = 0;
    }
    for (int i=0; i<n_throw_max; i++) {
        nthrow[i] = 0;
    }
    srand (time(NULL));
    for (int i=0; i<n_trial; i++) {
        int s = 0, s0 = 0;
        for (int j=0; j<n_throw_max; j++) {
            s += (1 + (rand() % NSURF_DICE));
            if (s0 <= ngoal && s > ngoal) {
                rec[s-ngoal-1] += 1;
                nthrow[j] += 1;
                break;
            } else {
                s0 = s;
            }
        }
    }
    printf("Statistics on the sum at termination:\n");
    double s = 0.0, p, p0=0;
    for (int i=0; i<NSURF_DICE; i++) {
        p = (double)rec[i]/(double)n_trial;
        s += p;
        printf("%4d: %8.5f, %8.5f\n", ngoal + i + 1, p, p0/p);
        p0 = p;
    }
    printf("Sum:  %8.5f\n", s);
    printf("Statistics on the number of throws:\n");
    s = 0.0;
    for (int i=0; i<n_throw_max; i++) {
        if (nthrow[i] == 0) {
            continue;
        }
        p = (double)nthrow[i] / (double)n_trial;
        s += p;
        printf("%4d: %13.10f\n", i+1, p);
    }
    printf("Sum:  %13.10f\n", s);
    return 0;
}
