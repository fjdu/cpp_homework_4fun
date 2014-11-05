#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "Matrix615.h"

using namespace std;

void initialize(Matrix615<int>&, vector<int>, vector<int>, int, int);
int worker(Matrix615<int>&, vector<int>, vector<int>, int, int);

int main()
{
    /*
    string line = "";
    getline(cin, line, '\n');
    stringstream lineStream(line);
    int W, n;
    lineStream >> W;
    lineStream >> n;
    vector<int> value(n), weight(n);
    for(int i=0; i<n; ++i)
    {
        lineStream >> value[i];
        lineStream >> weight[i];
    }
    cout << W << n << weight[1] << endl;
    */
    int n, W, tok;
    cin >> W;
    cin >> n;
    vector<int> value(n), weight(n);
    for(int i=0; i<n; ++i)
    {
        cin >> tok;
        value[i] = tok;
        cin >> tok;
        weight[i] = tok;
    }
    /*
    for(int i=0; i<n; ++i)
    {
        cout << value[i] << ' ' << weight[i] << endl;
    }
    */
    Matrix615<int> OUT(n+1, W+1, -1);

    initialize(OUT, weight, value, n, W);

    int maxValue = worker(OUT, weight, value, n, W);

    cout << maxValue << endl;

    return 0;
}

void initialize(Matrix615<int>& OUT, vector<int> weight, vector<int> value, int n, int W)
{
    for(int i=0; i<n+1; ++i) OUT.data[i][0]=0;
    for(int i=0; i<W+1; ++i) OUT.data[0][i]=0;
    for(int i=1; i<W+1; ++i)
    {
        if(weight[0]==i) OUT.data[1][i]=value[0];
        else OUT.data[1][i]=0;
    }
    OUT.data[1][0]=0;
}

int worker(Matrix615<int>& OUT, vector<int> weight, vector<int> value, int a, int b)
{
    if(OUT.data[a][b]==-1)
    {
        int noPut=worker(OUT, weight, value, a-1, b);
        int Put=worker(OUT, weight, value, a-1, b-weight[a-1])+value[a-1];
        if((noPut>=Put) || weight[a-1]>b) OUT.data[a][b]=noPut;
        else if(OUT.data[a-1][b-weight[a-1]]==0)
        {
            if(b==weight[a-1]) OUT.data[a][b]=value[a-1];
            else OUT.data[a][b]=noPut;
        }
        else OUT.data[a][b]=Put;

    }
    return OUT.data[a][b];
}
