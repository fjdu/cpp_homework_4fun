#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

using namespace std;

#define NOT_YET_CALCULATED -100


int count_align_enum(char *s1, char*s2, int ibg1, int ied1, int ibg2, int ied2, int **t) {
    if ((ibg1 > ied1) || (ibg2 > ied2)) {
        return 0;
    }
    if (t[ibg1][ibg2] != NOT_YET_CALCULATED) {
        return t[ibg1][ibg2];
    }
    int n, c, c1, s=0;
    for (int i=ied1; i>=ibg1; i--) {
        for (int j=ied2; j>=ibg2; j--) {
            n = max(ied1-i, ied2-j);
            c = 0;
            for (int k=0; k<=n; k++) {
                if (s1[i+k] == s2[j+k]) {
                    c += 1;
                } else {
                    c1 = max(count_align_enum(s1, s2, i+k+1, ied1, j+k, ied2, t), \
                             count_align_enum(s1, s2, i+k, ied1, j+k+1, ied2, t));
                    c = max(max(c, c1), c + c1 - 1);
                    break;
                }
            }
            if (c > s) {
                s = c;
            }
            t[i][j] = c;
        }
    }
    return s;
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        return 1;
    }
    //printf("%s\n%s\n", argv[1], argv[2]);
    int n1 = (int)strlen(argv[1]), n2 = (int)strlen(argv[2]);
    int **t;
    t = new int*[n1];
    for (int i=0; i<n1; i++) {
        t[i] = new int[n2];
        for (int j=0; j<n2; j++) {
            t[i][j] = NOT_YET_CALCULATED;
        }
    }
    printf("%d\n", count_align_enum(argv[1], argv[2], 0, n1-1, 0, n2-1, t));
}
