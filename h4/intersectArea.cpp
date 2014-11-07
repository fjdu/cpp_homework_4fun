#include <stdio.h>
#include <stdlib.h>

#define EPS 1e-10
#define PI 3.141592653589793

void inoutside_unit_circle(double x1, double y1, double x2, double y2, \
                           bool *is_outside, bool *is_inside, bool *contain_circ) {
    double r11 = x1*x1 + y1*y1;
    double r12 = x1*x1 + y2*y2;
    double r21 = x2*x2 + y1*y1;
    double r22 = x2*x2 + y2*y2;
    bool outside_1 = \
                  (r11 >  1.0) && \
                  (r12 >  1.0) && \
                  (r21 >  1.0) && \
                  (r22 >  1.0);
    *is_inside  = (r11 <= 1.0) && \
                  (r12 <= 1.0) && \
                  (r21 <= 1.0) && \
                  (r22 <= 1.0);
    if (outside_1 && (!(*is_inside))) {
        *contain_circ = \
            (x1 <= -1.0) && \
            (x2 >=  1.0) && \
            (y1 <= -1.0) && \
            (y2 >=  1.0);
        *is_outside = \
            (!(*contain_circ)) && \
            ((x1*x1 > 1.0) || (y1*y2 > 0.0)) && \
            ((x2*x2 > 1.0) || (y1*y2 > 0.0)) && \
            ((y1*y1 > 1.0) || (x1*x2 > 0.0)) && \
            ((y2*y2 > 1.0) || (x1*x2 > 0.0));
    } else {
        *contain_circ = false;
        *is_outside = false;
    }
}


double cArea(double x1, double y1, double x2, double y2) {
    if ((x2 <= x1) || (y2 <= y1)) {
        return 0.0;
    }
    bool is_out, is_in, is_contain;
    inoutside_unit_circle(x1, y1, x2, y2, &is_out, &is_in, &is_contain);
    if (is_in) {
        return (x2-x1) * (y2-y1);
    } else if (is_out) {
        return 0.0;
    } else if (is_contain) {
        return PI;
    } else if ((x2-x1)*(y2-y1) < EPS) {
        return 0.0;
    } else {
        double xmid = 0.5 * (x1 + x2);
        double ymid = 0.5 * (y1 + y2);
        return cArea(x1, y1, xmid, ymid) + \
               cArea(x1, ymid, xmid, y2) + \
               cArea(xmid, y1, x2, ymid) + \
               cArea(xmid, ymid, x2, y2);
    }
}

int main(int argc, char *argv[]){
    if (argc < 7) {
        return 1;
    }
    //
    double x1 = atof(argv[1]);
    double y1 = atof(argv[2]);
    double x2 = atof(argv[3]) - x1;
    double y2 = atof(argv[4]) - y1;
    double x3 = atof(argv[5]) - x1;
    double y3 = atof(argv[6]) - y1;
    //
    printf("%.4f\n", cArea(x2, y2, x3, y3));
    return 0;
}
