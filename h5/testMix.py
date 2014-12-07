from numpy.random import seed, uniform, normal, shuffle
from numpy import concatenate
from subprocess import Popen, PIPE
import time

def one_run(ntot = 1000, p = 0.5, mu = 0.0, sig = 1.0, a = 0.0, b = 10.0, seed_val=None):
    if seed_val != None:
        seed(seed_val)
    nnorm = int(ntot * p)
    nunif = ntot - nnorm
    xnorm = normal(loc=mu, scale=sig, size=nnorm)
    xunif = uniform(low=a, high=b, size=nunif)
    xs = concatenate((xnorm, xunif))
    shuffle(xs)
    #print xnorm
    #print xunif
    #print xs
    s = ''.join(['{:.4f} '.format(_) for _ in xs])

    cmd = './mixNormUnif'
    t0 = time.time()
    p = Popen ([cmd], stdout=PIPE, stderr=PIPE, stdin=PIPE)
    sout1, serr = p.communicate(s + '\n')
    return sout1, s, time.time()-t0

nrun = 10

for i in xrange(nrun):
    p, s, t = one_run(ntot = 10000, 
                      p = 0.1, mu=12, sig = 1.0,
                      a = -10.0, b = 20.0,
                      seed_val=i*97+2878329)
    p = p.replace('\n', '')
    if p == '':
        with open('xxx_error', 'w') as f:
            f.write(s)
    print p, '\t', t, ' s'
