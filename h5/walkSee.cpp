#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#define X_0_WALL 1.0
#define YMIN_WALL 0.0
#define YMAX_WALL 1.0
#define NDIVL 5000
#define NDIVR 5000

using namespace std;


void swap_seg(float *x1, float *y1, float *x2, float *y2, \
              int i1, int i2) {
    float tx1 = x1[i1], tx2 = x2[i1], ty1 = y1[i1], ty2 = y2[i1];
    x1[i1] = x1[i2];
    y1[i1] = y1[i2];
    x2[i1] = x2[i2];
    y2[i1] = y2[i2];
    x1[i2] = tx1;
    y1[i2] = ty1;
    x2[i2] = tx2;
    y2[i2] = ty2;
}


inline void swap_seg_idx(int *idx, int i1, int i2) {
    int tmp = idx[i1];
    idx[i1] = idx[i2];
    idx[i2] = tmp;
}


inline float get_intersection_y(float px, float py, float x, float y, float x0) {
    return (y-py) * (x0-px) / (x-px) + py;
}


inline float clip(float y, float ymin, float ymax) {
    if (y < ymin) {
        y = ymin;
    }
    if (y > ymax) {
        y = ymax;
    }
    return y;
}


float get_view_fraction(float px, float py, float x0, float ymin, float ymax, \
                        float x1, float y1, float x2, float y2) {
    float iy1 = get_intersection_y(px, py, x1, y1, x0);
    float iy2 = get_intersection_y(px, py, x2, y2, x0);
    iy1 = clip(iy1, ymin, ymax);
    iy2 = clip(iy2, ymin, ymax);
    return (iy1 >= iy2) ? (iy1 - iy2) : (iy2 - iy1);
}


float get_cmp_val(float *x1, float *y1, float *x2, float *y2, \
                  int i1, float *refx, float *refy, int nref) {
    float ftot1 = 0.0;
    for (int i=0; i<nref; i++) {
        ftot1 += get_view_fraction(refx[i], refy[i], \
                                   X_0_WALL, YMIN_WALL, YMAX_WALL, \
                                   x1[i1], y1[i1], x2[i1], y2[i1]);
    }
    return ftot1;
}


int partition(float *x1, float *y1, float *x2, float *y2, \
              int *idx, int i1, int i2, float *refx, float *refy, int nref) {
    float pivotval = get_cmp_val(x1, y1, x2, y2, idx[i2], refx, refy, nref);
    int storeidx = i1-1;
    for (int i=i1; i<=i2-1; i++) {
        float val = get_cmp_val(x1, y1, x2, y2, idx[i], refx, refy, nref);
        if (val > pivotval) {
            storeidx++;
            swap_seg_idx(idx, i, storeidx);
        }
    }
    swap_seg_idx(idx, storeidx+1, i2);
    return storeidx+1;
}


void sort_segments(float *x1, float *y1, float *x2, float *y2, \
                   int *idx, int i1, int i2, \
                   float *refx, float *refy, int nref) {
    if (i1 < i2) {
        int p = partition(x1, y1, x2, y2, idx, i1, i2, refx, refy, nref);
        sort_segments(x1, y1, x2, y2, idx, i1, p-1, refx, refy, nref);
        sort_segments(x1, y1, x2, y2, idx, p+1, i2, refx, refy, nref);
    }
}


inline bool intersect_2seg(float xL, float yL, float xR, float yR, \
                    float x1, float y1, float x2, float y2) {
    float u = ((x1 - xL) * (y2 - y1) - (y1 - yL) * (x2 - x1)) / ((xR - xL) * (y2 - y1) - (yR - yL) * (x2 - x1));
    float v = ((x1 - xL) * (yR - yL) - (y1 - yL) * (xR - xL)) / ((xR - xL) * (y2 - y1) - (yR - yL) * (x2 - x1));
    return (0.0 <= u) && (u <= 1.0) && \
           (0.0 <= v) && (v <= 1.0);
}


bool intersect(float xL, float yL, float xR, float yR, \
               float *x1, float *y1, float *x2, float *y2, int *idx, int nseg) {
    for (int i=0; i<nseg; i++) {
        int i0 = idx[i];
        if (intersect_2seg(xL, yL, xR, yR, x1[i0], y1[i0], x2[i0], y2[i0])) {
            return true;
        }
    }
    return false;
}


void make_mask(bool *mask, int ndiv, float dy, \
               float xL, float yminL, float ymaxL, \
               float xR, float yminR, float ymaxR, \
               float *x1, float *y1, float *x2, float *y2, int nseg) {
    float yT, yB;
    int iT, iB;
    for (int i=0; i<nseg; i++) {
        if (y1[i] > y2[i]) {
            yT = ymaxR - (xR - xL) / (xR - x1[i]) * (ymaxR - y1[i]);
            yB = yminR - (xR - xL) / (xR - x2[i]) * (yminR - y2[i]);
        } else if (y1[i] < y2[i]) {
            yT = ymaxR - (xR - xL) / (xR - x2[i]) * (ymaxR - y2[i]);
            yB = yminR - (xR - xL) / (xR - x1[i]) * (yminR - y1[i]);
        } else {
            continue;
        }
        yB = clip(yB, yminL, ymaxL);
        yT = clip(yT, yminL, ymaxL);
        if (yT > yB) {
            iB = (yB - yminL) / dy;
            iT = (yT - yminL) / dy;
            if (iT > ndiv) {
                iT = ndiv;
            }
            for (int j=iB; j<=iT; j++) {
                mask[j] = true;
            }
        }
    }
}


int main(int argc, char *argv[]){
    string line = "";
    std::getline(std::cin, line, '\n');
    std::stringstream lineStream(line);
    float tmp;
    std::vector<float> values;
    while (lineStream >> tmp) {
        values.push_back(tmp);
    }
    if ((values.size() % 4) != 0) {
        return -1;
    }
    int nseg = values.size() / 4;
    float *x1, *y1, *x2, *y2;
    int *idx1, *idx2, *idx;
    x1 = new float[nseg];
    y1 = new float[nseg];
    x2 = new float[nseg];
    y2 = new float[nseg];
    idx1 = new int[nseg];
    idx2 = new int[nseg];
    for (int i=0; i<nseg; i++) {
        x1[i] = values[4*i];
        y1[i] = values[4*i+1];
        x2[i] = values[4*i+2];
        y2[i] = values[4*i+3];
        idx1[i] = i;
        idx2[i] = i;
        //printf("(%.4f, %.4f)  to  (%.4f, %.4f) \n", x1[i], y1[i], x2[i], y2[i]);
    }
    //
    float refx1[3] = {0.0, 0.0,  0.0};
    float refy1[3] = {0.0, 0.25, 0.5};
    float refx2[3] = {0.0, 0.0,  0.0};
    float refy2[3] = {0.5, 0.75, 1.0};
    sort_segments(x1, y1, x2, y2, idx1, 0, nseg-1, refx1, refy1, 3);
    sort_segments(x1, y1, x2, y2, idx2, 0, nseg-1, refx2, refy2, 3);
    //
    float dyL = 1.0 / ((float)NDIVL-1.0);
    float dyR = 1.0 / ((float)NDIVR-1.0);
    //
    bool *mask;
    mask = new bool[NDIVL];
    for (int i=0; i<NDIVL; i++) {
        mask[i] = false;
    }
    make_mask(mask, NDIVL, dyL, \
              0.0, 0.0, 1.0, \
              1.0, 0.0, 1.0, \
              x1, y1, x2, y2, nseg);
    //
    float frac_max = 0.0, frac_acc;
    float yL = 0.0;
    idx = idx1;
    for (int i=0; i<NDIVL; i++) {
        if (mask[i]) {
            continue;
        }
        float yR = 0.0;
        frac_acc = 0.0;
        if (yL > 0.5) {
            idx = idx2;
        }
        for (int j=0; j<NDIVR; j++) {
            if (intersect(0.0, yL, 1.0, yR, x1, y1, x2, y2, idx, nseg)) {
                frac_acc = 0.0;
            } else {
                frac_acc += dyR;
            }
            if ((1.0 - yR + frac_acc) <= frac_max) {
                break;
            }
            yR += dyR;
        }
        if (frac_acc > frac_max) {
            frac_max = frac_acc;
        }
        if (frac_max > 0.999) {
            break;
        }
        yL += dyL;
    }
    printf("%.3f\n", frac_max);
    //for (int i=0; i<nseg; i++) {
    //    printf("%5d, %5d: (%.4f, %.4f)  to  (%.4f, %.4f): %.8f \n", \
    //           i, idx[i], x1[idx[i]], y1[idx[i]], x2[idx[i]], y2[idx[i]], \
    //           get_view_fraction(refx[0], refy[0], X_0_WALL, YMIN_WALL, YMAX_WALL, \
    //                             x1[idx[i]], y1[idx[i]], x2[idx[i]], y2[idx[i]]));
    //}
    return 0;
}
