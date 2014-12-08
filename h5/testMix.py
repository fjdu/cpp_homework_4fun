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

nrun = 500

n_range = (50, 10000)
p_range = (0.05, 0.99)
mu_range = (-50, 50)
a_range = (-20, 20)
b_a_range = (0.5, 30)

nerr = 0

for i in xrange(nrun):
    seed(i*17+75491)
    n = int(uniform(low=n_range[0], high=n_range[1]))
    prob = uniform(low=p_range[0], high=p_range[1])
    mu = round(uniform(low=mu_range[0], high=mu_range[1]))
    a = uniform(low=a_range[0], high=a_range[1])
    b = a + uniform(low=b_a_range[0], high=b_a_range[1])
    p, s, t = one_run(ntot = n, 
                      p = prob,
                      mu = mu,
                      sig = 1.0,
                      a = a,
                      b = b,
                      seed_val=i*97+2878329)
    p = p.replace('\n', '')
    if p == '':
        with open('xxx_error', 'w') as f:
            f.write(s)
    p = float(p)
    if p != mu:
        nerr = nerr + 1
    print 'mu = {0:6.1f} ({1:6.1f})'.format(p, mu), ', p = {0:4.2f}, a = {1:6.2f}, b = {2:6.2f}, n = {3:6d}'.format(prob, a, b, n), '    time = {0:.4f} s'.format(t), '' if p == mu else '  X'

print 'Your score: {0:d} ({1:d} error out of {2:d})'.format((nrun-nerr)*10/nrun, nerr, nrun)
