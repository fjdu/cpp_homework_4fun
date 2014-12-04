#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


#define X_0_WALL 1.0
#define YMIN_WALL 0.0
#define YMAX_WALL 1.0
#define NDIVL 10000
#define NDIVR 10000


#define SORT_INTERVALS


using namespace std;


// Method 1: chain-of-intervals

typedef struct interval {
    float vL, vR;
    struct interval *left, *right;
} interval;


typedef struct simple_interval {
    float vL, vR;
} simple_interval;


void go_to_head(interval **itv) {
    if ((*itv)->left != NULL) {
        (*itv) = (*itv)->left;
        go_to_head(itv);
    }
}


void cleanup_interval_rightward(interval **itv) {
    if ((*itv)->right == NULL) {
        return;
    }
    if ((*itv)->right->vL > (*itv)->vR) {
        //         (   )
        //  (   )
        return;
    } else if ((*itv)->right->vR >= (*itv)->vR) {
        //      (     )
        //  (      )
        //printf("000\n");
        (*itv)->vR = (*itv)->right->vR;
        interval *tmp;
        tmp = (*itv)->right->right;
        delete (*itv)->right;
        (*itv)->right = NULL;
        if (tmp != NULL) {
            tmp->left = (*itv);
            (*itv)->right = tmp;
        }
    } else {
        //     (    )
        //  (          )
        //printf("111\n");
        interval *tmp;
        tmp = (*itv)->right->right;
        delete (*itv)->right;
        (*itv)->right = NULL;
        if (tmp != NULL) {
            (*itv)->right = tmp;
            tmp->left = (*itv);
            cleanup_interval_rightward(itv);
        }
    }
}


void cleanup_interval_leftward(interval **itv) {
    if ((*itv)->left == NULL) {
        return;
    }
    if ((*itv)->left->vR < (*itv)->vL) {
        //  (  )
        //        (   )
        return;
    } else if ((*itv)->left->vL <= (*itv)->vL) {
        //  (     )
        //     (      )
        //printf("222\n");
        (*itv)->vL = (*itv)->left->vL;
        interval *tmp;
        tmp = (*itv)->left->left;
        delete (*itv)->left;
        (*itv)->left = NULL;
        if (tmp != NULL) {
            (*itv)->left = tmp;
            tmp->right = (*itv);
        }
    } else {
        //     (    )
        //  (          )
        //printf("333\n");
        interval *tmp;
        tmp = (*itv)->left->left;
        delete (*itv)->left;
        (*itv)->left = NULL;
        if (tmp != NULL) {
            (*itv)->left = tmp;
            tmp->right = (*itv);
            cleanup_interval_leftward(itv);
        }
    }
}


void add_to_interval_right(interval **itv, float xmin, float xmax) {
    if (xmax < (*itv)->vL) {
        //printf("OOO\n");
        interval *tmp;
        tmp = new interval;
        tmp->vL = xmin;
        tmp->vR = xmax;
        tmp->left = (*itv)->left;
        tmp->right = (*itv);
        (*itv)->left = tmp;
        tmp->left->right = tmp;
        return;
    } else if ((xmax >= (*itv)->vL) && (xmax <= (*itv)->vR)) {
        //printf("PPP\n");
        if (xmin < (*itv)->vL) {
            (*itv)->vL = xmin;
        }
    } else if (xmin < (*itv)->vL) {
        //printf("LLL\n");
        (*itv)->vL = xmin;
        (*itv)->vR = xmax;
        cleanup_interval_rightward(itv);
    } else if ((xmin >= (*itv)->vL) && (xmin <= (*itv)->vR)) {
        //printf("MMM\n");
        (*itv)->vR = xmax;
        cleanup_interval_rightward(itv);
    } else {
        //printf("NNN\n");
        if ((*itv)->right != NULL) {
            //printf("III, %f, %f, %f, %f\n", (*itv)->right->vL, (*itv)->right->vR, xmin, xmax);
            //if ((*itv)->right->right != NULL) {
            //  //printf("iii, %f, %f, %f, %f\n", (*itv)->right->right->vL, (*itv)->right->right->vR, xmin, xmax);
            //}
            add_to_interval_right(&((*itv)->right), xmin, xmax);
        } else {
            //printf("JJJ\n");
            (*itv)->right = new interval;
            (*itv)->right->vL = xmin;
            (*itv)->right->vR = xmax;
            (*itv)->right->left = *itv;
            (*itv)->right->right = NULL;
        }
    }
}


void add_to_interval_left(interval **itv, float xmin, float xmax) {
    if (xmin > (*itv)->vR) {
        interval *tmp;
        tmp = new interval;
        tmp->vL = xmin;
        tmp->vR = xmax;
        tmp->right = (*itv)->right;
        tmp->left = (*itv);
        (*itv)->right = tmp;
        tmp->right->left = tmp;
        return;
    } else if ((xmin >= (*itv)->vL) && (xmin <= (*itv)->vR)) {
        if (xmax > (*itv)->vR) {
            (*itv)->vR = xmax;
        }
    } else if (xmax > (*itv)->vR) {
        (*itv)->vL = xmin;
        (*itv)->vR = xmax;
        cleanup_interval_leftward(itv);
    } else if ((xmax >= (*itv)->vL) && (xmax <= (*itv)->vR)) {
        (*itv)->vL = xmin;
        cleanup_interval_leftward(itv);
    } else {
        if ((*itv)->left != NULL) {
            add_to_interval_left(&((*itv)->left), xmin, xmax);
        } else {
            (*itv)->left = new interval;
            (*itv)->left->vL = xmin;
            (*itv)->left->vR = xmax;
            (*itv)->left->right = *itv;
            (*itv)->left->left = NULL;
        }
    }
}


void add_to_interval(interval **itv, float xmin, float xmax) {
    if (xmax <= xmin) {
        return;
    }
    if ((*itv) == NULL) {
        (*itv) = new interval;
        (*itv)->vL = xmin;
        (*itv)->vR = xmax;
        (*itv)->left = NULL;
        (*itv)->right = NULL;
        return;
    }
    //printf("vL,vR;xmin,xmax: %.3f, %.3f; %.3f, %.3f\n", (*itv)->vL, (*itv)->vR, xmin, xmax);
    if ((xmin >= (*itv)->vL) && (xmin <= (*itv)->vR)) {
        //printf("000\n");
        if ((xmax >= (*itv)->vL) && (xmax <= (*itv)->vR)) {
            //  (       )
            //     ( )
            return;
        } else {
            //  (       )
            //     (       )
            (*itv)->vR = xmax;
            cleanup_interval_rightward(itv);
        }
    } else if (xmin > (*itv)->vR) {
        //  (       )
        //             (    )
        //printf("111\n");
        if ((*itv)->right != NULL) {
            //printf("QQQ\n");
            add_to_interval_right(&((*itv)->right), xmin, xmax);
        } else {
            (*itv)->right = new interval;
            (*itv)->right->left = (*itv);
            (*itv)->right->right = NULL;
            (*itv)->right->vL = xmin;
            (*itv)->right->vR = xmax;
        }
    } else if (xmax < (*itv)->vL) {
        //         (       )
        //  (   )
        //printf("222\n");
        if ((*itv)->left != NULL) {
            //printf("PPP\n");
            add_to_interval_left(&((*itv)->left), xmin, xmax);
        } else {
            (*itv)->left = new interval;
            (*itv)->left->right = (*itv);
            (*itv)->left->left = NULL;
            (*itv)->left->vL = xmin;
            (*itv)->left->vR = xmax;
        }
    } else if ((xmax >= (*itv)->vL) && (xmax <= (*itv)->vR)) {
        //         (       )
        //  (         )
        //printf("333\n");
        (*itv)->vL = xmin;
        cleanup_interval_leftward(itv);
    } else if (xmax > (*itv)->vR) {
        //         (       )
        //  (                    )
        //printf("444\n");
        (*itv)->vL = xmin;
        (*itv)->vR = xmax;
        cleanup_interval_rightward(itv);
        cleanup_interval_leftward(itv);
    }
}


void destroy_interval_right(interval **itv) {
    if ((*itv) == NULL) {
        return;
    }
    if ((*itv)->right != NULL) {
        destroy_interval_right(&((*itv)->right));
    }
    delete *itv;
    *itv = NULL;
    return;
}


void destroy_interval_left(interval **itv) {
    if ((*itv) == NULL) {
        return;
    }
    if ((*itv)->left != NULL) {
        destroy_interval_left(&((*itv)->left));
    }
    delete *itv;
    *itv = NULL;
    return;
}


void destroy_interval(interval **itv) {
    if (*itv == NULL) {
        return;
    }
    destroy_interval_right(&((*itv)->right));
    destroy_interval_left(&((*itv)->left));
    delete *itv;
    *itv = NULL;
}


float sumup_interval_leftward(interval *itv) {
    if (itv == NULL) {
        return 0.0;
    } else {
        //printf("\tL: vL,vR = %.3f, %.3f\n", itv->vL, itv->vR);
        return (itv->vR - itv->vL) + \
               sumup_interval_leftward(itv->left);
    }
}


float sumup_interval_rightward(interval *itv) {
    if (itv == NULL) {
        return 0.0;
    } else {
        //printf("\tR: vL,vR = %.3f, %.3f\n", itv->vL, itv->vR);
        return (itv->vR - itv->vL) + \
               sumup_interval_rightward(itv->right);
    }
}


float sumup_interval(interval *itv) {
    if (itv == NULL) {
        return 0.0;
    } else {
        //printf("\tC: vL,vR = %.3f, %.3f\n", itv->vL, itv->vR);
        return (itv->vR - itv->vL) + \
               sumup_interval_leftward(itv->left) + \
               sumup_interval_rightward(itv->right);
    }
}


// Method 2: ray-crossing


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


void get_view_interval(float px, float py, float x0, float ymin, float ymax, \
                       float x1, float y1, float x2, float y2, \
                       float *itvvL, float *itvvR) {
    float iy1 = get_intersection_y(px, py, x1, y1, x0);
    float iy2 = get_intersection_y(px, py, x2, y2, x0);
    iy1 = clip(iy1, ymin, ymax);
    iy2 = clip(iy2, ymin, ymax);
    if (iy1 <= iy2) {
        *itvvL = iy1;
        *itvvR = iy2;
    } else {
        *itvvL = iy2;
        *itvvR = iy1;
    }
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
    // Quicksort
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
    // Mask out the completely blocked region
    float yT, yB;
    int iT, iB;
    interval *itv_mask;
    itv_mask = NULL;
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
        if (!((yB != yB) || (yT != yT))) {
            add_to_interval(&itv_mask, yB, yT);
        }
    }
    if (itv_mask != NULL) {
        go_to_head(&itv_mask);
    }
    while (itv_mask != NULL) {
        yB = itv_mask->vL;
        yT = itv_mask->vR;
        iB = (yB - yminL) / dy;
        iT = (yT - yminL) / dy;
        if (iB < 0) {
            iB = 0;
        }
        if (iT > ndiv-1) {
            iT = ndiv-1;
        }
        for (int j=iB; j<=iT; j++) {
            mask[j] = true;
        }
        itv_mask = itv_mask->right;
    };
    destroy_interval(&itv_mask);
}


void swap_interval(simple_interval *itvs, int i1, int i2) {
    float t1 = itvs[i1].vL, t2 = itvs[i1].vR;
    itvs[i1].vL = itvs[i2].vL;
    itvs[i1].vR = itvs[i2].vR;
    itvs[i2].vL = t1;
    itvs[i2].vR = t2;
}


int partition_intervals(simple_interval *itvs, int i1, int i2) {
    float piv1=itvs[i2].vL, piv2=itvs[i2].vR;
    int storeidx = i1-1;
    for (int i=i1; i<=i2-1; i++) {
        if ((itvs[i].vL < piv1) || \
            ((itvs[i].vL == piv1) && (itvs[i].vR <= piv2))) {
            storeidx++;
            swap_interval(itvs, i, storeidx);
        }
    }
    swap_interval(itvs, storeidx+1, i2);
    return storeidx+1;
}


void sort_intervals(simple_interval *itvs, int i1, int i2) {
    if (i1 < i2) {
        int p = partition_intervals(itvs, i1, i2);
        sort_intervals(itvs, i1, p-1);
        sort_intervals(itvs, p+1, i2);
    }
}


float sumup_simpleintervals(simple_interval *itvs, int nseg) {
    float s = itvs[0].vR - itvs[0].vL;
    float rightmost = itvs[0].vR;
    for (int i=1; i<nseg; i++) {
        if (itvs[i].vL >= rightmost) {
            s += (itvs[i].vR - itvs[i].vL);
            rightmost = itvs[i].vR;
        } else if (itvs[i].vR > rightmost) {
            s += (itvs[i].vR - rightmost);
            rightmost = itvs[i].vR;
        }
        //printf("%5d: %.5f, %.5f\n", i, itvs[i].vL, itvs[i].vR);
    }
    return s;
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
    float ave_len=0.0;
    int ave_nseg_cross;
    for (int i=0; i<nseg; i++) {
        x1[i] = values[4*i];
        y1[i] = values[4*i+1];
        x2[i] = values[4*i+2];
        y2[i] = values[4*i+3];
        idx1[i] = i;
        idx2[i] = i;
        ave_len += (y2[i] >= y1[i]) ? (y2[i] - y1[i]) : (y1[i] - y2[i]);
        //printf("%5d of %5d: (%.4f, %.4f) to (%.4f, %.4f) \n", \
        //       i+1, nseg, x1[i], y1[i], x2[i], y2[i]);
    }
    ave_len /= (float)nseg;
    ave_nseg_cross = 1.0/ave_len + 1;
    //
    bool *mask;
    //
    float dyL = 1.0 / ((float)NDIVL-1.0);
    float dyR = 1.0 / ((float)NDIVR-1.0);
    //
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
#ifndef SORT_INTERVALS
    /*
    Ray-crossing
    */
    if (nseg > (ave_nseg_cross*NDIVR)) {
        float refx1[3] = {0.0, 0.0,  0.0};
        float refy1[3] = {0.0, 0.25, 0.5};
        float refx2[3] = {0.0, 0.0,  0.0};
        float refy2[3] = {0.5, 0.75, 1.0};
        sort_segments(x1, y1, x2, y2, idx1, 0, nseg-1, refx1, refy1, 3);
        sort_segments(x1, y1, x2, y2, idx2, 0, nseg-1, refx2, refy2, 3);
        //
        idx = idx1;
        for (int i=0; i<NDIVL; i++) {
            if (!mask[i]) {
                if (yL > 0.5) {
                    idx = idx2;
                }
                float yR = 0.0;
                frac_acc = 0.0;
                for (int j=0; j<NDIVR; j++) {
                    if (!(intersect(0.0, yL, 1.0, yR, x1, y1, x2, y2, idx, nseg))) {
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
            }
            yL += dyL;
        }
    /*
    Chain of intervals
    */
    } else {
        for (int i=0; i<NDIVL; i++) {
            if (!mask[i]) {
                interval *itv;
                float itvvL, itvvR;
                itv = NULL;
                for (int j=0; j<nseg; j++) {
                    get_view_interval(0.0, yL, 1.0, 0.0, 1.0, \
                                      x1[j], y1[j], x2[j], y2[j], \
                                      &itvvL, &itvvR);
                    add_to_interval(&itv, itvvL, itvvR);
                    if ((j%1000 == 0) && (j > 0)) {
                        if (sumup_interval(itv) >= 0.999) {
                            break;
                        }
                    }
                }
                frac_acc = 1.0 - sumup_interval(itv);
                if (frac_acc > frac_max) {
                    frac_max = frac_acc;
                }
                destroy_interval(&itv);
            }
            yL += dyL;
        }
    }
#else
    /*
    Sort the intervals
    */
    simple_interval *itvs;
    itvs = new simple_interval[nseg];
    for (int i=0; i<NDIVL; i++) {
        if (!mask[i]) {
            float itvvL, itvvR;
            for (int j=0; j<nseg; j++) {
                get_view_interval(0.0, yL, 1.0, 0.0, 1.0, \
                                  x1[j], y1[j], x2[j], y2[j], \
                                  &itvvL, &itvvR);
                itvs[j].vL = itvvL;
                itvs[j].vR = itvvR;
            }
            sort_intervals(itvs, 0, nseg-1);
            frac_acc = 1.0 - sumup_simpleintervals(itvs, nseg);
            if (frac_acc > frac_max) {
                frac_max = frac_acc;
            }
        }
        yL += dyL;
    }
#endif
    /*
    The end
    */
    printf("%.3f\n", frac_max);
    return 0;
}
