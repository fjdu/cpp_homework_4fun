#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>


#define STD_VAL 1.0
#define LOG_2PI 1.83787706641
#define NMAX_ITER 1000
#define SMALL_VAL 1e-10
#define ATOL 1e-6
#define RTOL 1e-6

using namespace std;


void initial_guess(std::vector<double> x, double *p, double *m, int *i0) {
    int c = 1, n = x.size();
    int i = *i0;
    *m = x[i];
    if (((x[i]-x[i-1]) < STD_VAL) || ((x[i+1]-x[i]) < STD_VAL)) {
        for (int j=i-1; j>=0; j--) {
            if (x[j] > (x[i] - STD_VAL)) {
                *m += x[j];
                c++;
            } else {
                break;
            }
        }
        for (int j=i+1; j<n; j++) {
            *i0 = j;
            if (x[j] < (x[i] + STD_VAL)) {
                *m += x[j];
                c++;
            } else {
                break;
            }
        }
    } else {
        *i0 = (*i0) + 1;
    }
    *m = round((*m)/(double)c);
    *p = (double)c / (double)n;
}


double one_iteration(std::vector<double> x, \
        double *p, double *m, double *a, double *b) {
    std::vector<double>::iterator it;
    double mnew = 0.0, munif_new = 0.0, sigunif_new = 0.0;
    double pnorm_t = 0.0, punif_t = 0.0;
    double ln_L = 0.0;
    for (it=x.begin(); it != x.end(); it++) {
        double v = *it;
        double pnorm = (*p) * exp(-0.5*((v-(*m))*(v-(*m)) + LOG_2PI));
        double punif = (1.0 - *p) / (*b - *a);
        double px = pnorm + punif;
        double pnorm_here = pnorm/px;
        ln_L += log(px);
        pnorm_t += pnorm_here;
        punif_t += (1.0 - pnorm_here);
        mnew += pnorm_here * v;
        munif_new += (1.0 - pnorm_here) * v;
        sigunif_new += (1.0 - pnorm_here) * v * v;
    }
    mnew /= pnorm_t;
    munif_new /= punif_t;
    sigunif_new = sigunif_new/punif_t - munif_new*munif_new;
    //
    *p = pnorm_t / (double)x.size();
    *m = mnew;
    double tmp = sqrt(3.0*sigunif_new);
    *a = munif_new - tmp;
    *b = munif_new + tmp;
    //printf("%.3f, %.3f, %.3f, %.3f\n", *p, mnew, munif_new, sigunif_new);
    return ln_L;
}


int main(int argc, char *argv[]){
    string line = "";
    std::getline(std::cin, line, '\n');
    std::stringstream lineStream(line);
    double tmp;
    std::vector<double> values;
    while (lineStream >> tmp) {
        values.push_back(tmp);
    }
    int n = values.size();
    std::sort(values.begin(), values.end());
    //
    double m, p, a, b;
    double m0, p0, a0, b0;
    double ln_L=-1e30, ln_L_max=-1e30;
    for (int idx=1; idx<n-1;) {
        initial_guess(values, &p, &m, &idx);
        a = values[0];
        b = values[n-1];
        //
        //printf("\t%4d: INIT %.3e, %.3f, %.3f, %.3f\n", 0, p, m, a, b);
        double p1=0.0, m1=0.0, a1=0.0, b1=0.0;
        for (int i=0; i<NMAX_ITER; i++) {
            tmp = one_iteration(values, &p, &m, &a, &b);
            //printf("\t%4d: ITER %.3e, %.3f, %.3f, %.3f\n", i, p, m, a, b);
            if ((p < SMALL_VAL) || \
                ((i > 10) && (tmp < ln_L)) || \
                (fabs(tmp-ln_L) < RTOL*tmp) || \
                ((fabs(p-p1) < ATOL + RTOL*p1) && \
                 (fabs(m-m1) < ATOL + RTOL*m1) && \
                 (fabs(a-a1) < ATOL + RTOL*a1) && \
                 (fabs(b-b1) < ATOL + RTOL*b1))) {
                break;
            }
            ln_L = tmp;
            p1 = p; m1 = m; a1 = a; b1 = b;
        }
        if (ln_L_max < ln_L) {
            ln_L_max = ln_L;
            m0 = m;
            p0 = p;
            a0 = a;
            b0 = b;
        }
    }
    if (n < 3) {
        m0 = 0.5*((*values.begin()) + (*(values.end()-1))) ;
    }
    printf("%d\n", (int)round(m0));
    //printf("%.3e, %.3f, %.3f, %.3f\n", p0, m0, a0, b0);
    return 0;
}
