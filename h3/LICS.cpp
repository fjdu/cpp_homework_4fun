#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int lics(int *v1, int *v2, int n1, int n2) {
    int *L, c;
    L = new int[n1];
    for (int i=0; i<n1; i++) {
        L[i] = 0;
    }
    for (int i=0; i<n2; i++) {
        c = 0;
        for (int j=0; j<n1; j++) {
            if ((v2[i] == v1[j]) && (L[j] < (c+1))) {
                L[j] = c+1;
            }
            if ((v2[i] > v1[j]) && (c < L[j])) {
                c = L[j];
            }
        }
    }
    c = 0;
    for (int i=0; i<n1; i++) {
        c = max(c, L[i]);
    }
    delete L;
    return c;
}


int main(int argc, char *argv[]){
    std::vector<int> values[2];
    for (int j=0; j<2; j++) {
      string line = "";
      std::getline(std::cin, line, '\n');
      std::stringstream lineStream(line);
      int i;
      while (lineStream >> i) {
          values[j].push_back(i);
      }
    }
    //
    int n1 = values[0].size()-1;
    int n2 = values[1].size()-1;
    if ((n1 != values[0][0]) || (n2 != values[1][0])) {
        return 1;
    }
    int *v1, *v2;
    v1 = new int[n1];
    v2 = new int[n2];
    for (int i=0; i<n1; i++) {
        v1[i] = values[0][i+1];
        //printf("%d\n", v1[i]);
    }
    for (int i=0; i<n2; i++) {
        v2[i] = values[1][i+1];
        //printf("%d\n", v2[i]);
    }
    int lics_num;
    if (n1 <= n2) {
        lics_num = lics(v1, v2, n1, n2);
    } else {
        lics_num = lics(v2, v1, n2, n1);
    }
    printf("%d\n", lics_num);
    return 0;
}
