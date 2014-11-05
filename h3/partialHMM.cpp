#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Matrix615.h"
#include "HMM615.h"

int main(int argc, char** argv) {
    double p  = atof(argv[1]);
    double a1 = atof(argv[2]);
    double a2 = atof(argv[3]);
    double b1 = atof(argv[4]);
    double b2 = atof(argv[5]);
    std::string tok=argv[6];
    std::vector<int> toss;

    for(int i=0; i<tok.size(); ++i)
    {
        if(tok[i]=='H') toss.push_back(0);
        else if(tok[i]=='T') toss.push_back(1);
        else if(tok[i]=='.') toss.push_back(-1);
    }

    int T = toss.size();
    HMM615 hmm(2, 2, T);
    hmm.trans.data[0][0] = a1; hmm.trans.data[0][1] = 1-a1;
    hmm.trans.data[1][0] = a2; hmm.trans.data[1][1] = 1-a2;
    hmm.emis.data[0][0] = b1; hmm.emis.data[0][1] = 1-b1;
    hmm.emis.data[1][0] = b2; hmm.emis.data[1][1] = 1-b2;

    hmm.pis[0] = p; hmm.pis[1] = 1-p;

    hmm.outs = toss;

    hmm.forwardBackward();
    //hmm.viterbi();
    
    for(int i=0; i<T; ++i)
    {
        if(toss[i]==0) std::cout << 'H';
        else if(toss[i]==1) std::cout << 'T';
        else
        {
            if((b1*hmm.gammas.data[i][0]+b2*hmm.gammas.data[i][1])>0.5) std::cout << 'H';
            else std::cout << 'T';
        }
    }
    std::cout << std::endl;
    return 0;
}
