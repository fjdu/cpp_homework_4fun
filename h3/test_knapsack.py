from numpy.random import randint
from subprocess import Popen, PIPE


def cmp_knapsack( \
    n = 10,
    Wmax = 200,
    Wmin = 1,
    vmin = 1,
    vmax = 60,
    fmin = 2,
    fmax = 10
):
    w = randint(Wmin, Wmax, size=n)
    v = randint(vmin, vmax, size=n)
    W = w.sum() / randint(fmin, fmax)
    
    arg =  '{:} '.format(W) + '{:} '.format(n) +  \
        ' '.join(['{:}'.format(_[0]) + ' {:}'.format(_[1]) for _ in zip(list(v), list(w))])
    
    cmd1 = './knapsack'
    cmd2 = './knapsack_LH'
    
    p = Popen ([cmd1], stdout=PIPE, stderr=PIPE, stdin=PIPE)
    sout1, serr = p.communicate(arg + '\n')
    
    p = Popen ([cmd2], stdout=PIPE, stderr=PIPE, stdin=PIPE)
    sout2, serr = p.communicate(arg + '\n')
    
    return sout1.replace('\n', ''), sout2.replace('\n', ''), arg, v, w



nnzero = 0
ntot = 10000

for i in xrange(ntot):
    r1, r2, arg, v, w = cmp_knapsack(n=randint(1,10))
    if r1.strip() != '0':
        nnzero += 1
    #print '{:12s} {:12s}'.format(r1, r2)
    if (r1 != r2):
        print 'Error (mine vs LH): {:12s} {:12s}'.format(r1, r2)
        print '\t', arg
        for i in xrange(len(v)):
            print '\t\t', v[i], w[i]

print float(nnzero) / float(ntot)
