from numpy.random import random
from subprocess import Popen, PIPE


def cmp_walk(nseg=1000, lenseg = 0.3):
    x1 = random(nseg) * lenseg
    y1 = random(nseg) * lenseg
    x2 = random(nseg) * lenseg
    y2 = random(nseg) * lenseg
    
    s = ''.join(['{:.4f} {:.4f} {:.4f} {:.4f} '.format(x1[i], y1[i], x2[i], y2[i]) \
                for i in range(nseg)])
    #print s
    cmd = './walkSee'
    
    p = Popen ([cmd], stdout=PIPE, stderr=PIPE, stdin=PIPE)
    sout1, serr = p.communicate(s + '\n')
    return sout1, s

ntot = 10

for i in xrange(ntot):
    p, s = cmp_walk(nseg=40, lenseg=0.95)
    print p, s
