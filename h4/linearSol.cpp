#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

void solve(int *b, int n) {
    // c: result
    for (int i=n-2; i>=0; i--) {
        b[i+1] -= b[i];
    }
    int s = b[n-1];
    for (int i=n-2; i>=0; i--) {
        b[i] -= s;
        s += b[i];
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
    int *b;
    b = new int[n];
    for (int i=0; i<n; i++) {
        b[i] = values[i];
    }
    solve(b, n);
    int m = b[0];
    for (int i=1; i<n; i++) {
        if (m < b[i]) {
            m = b[i];
        }
    }
    printf("%d\n", m);
    return 0;
}
