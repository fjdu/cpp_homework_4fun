from numpy.random import seed, random
from subprocess import Popen, PIPE
import time
from numpy import sin, cos

def cmp_walk(nseg=1000, lenseg = 0.3, seed_val=None):
    if seed_val != None:
        seed(seed_val)
    xc = random(nseg)
    yc = random(nseg)
    le = random(nseg) * lenseg
    an = random(nseg) * 6.28318530718
    cosan = cos(an)
    sinan = sin(an)
    x1 = xc - le * cosan
    x2 = xc + le * cosan
    y1 = yc - le * sinan
    y2 = yc + le * sinan
    x1[x1 > 1.0] = 1.0
    x2[x2 > 1.0] = 1.0
    x1[x1 < 0.0] = 0.0
    x2[x2 < 0.0] = 0.0
    y1[y1 > 1.0] = 1.0
    y2[y2 > 1.0] = 1.0
    y1[y1 < 0.0] = 0.0
    y2[y2 < 0.0] = 0.0
    
    s = ''.join(['{:.4f} {:.4f} {:.4f} {:.4f} '.format(x1[i], y1[i], x2[i], y2[i]) \
                for i in range(nseg)])
    #cmd = './walkSee'
    cmd = './walkSee_LH'
    
    t0 = time.time()
    p = Popen ([cmd], stdout=PIPE, stderr=PIPE, stdin=PIPE)
    sout1, serr = p.communicate(s + '\n')
    return sout1, s, time.time()-t0

ntot = 10

for i in xrange(ntot):
    p, s, t = cmp_walk(nseg=3001, lenseg=0.002, seed_val=97*i+23456)
    p = p.replace('\n', '')
    if p == '':
        with open('xxx', 'w') as f:
            f.write(s)
    print p, '\t', t, ' s'
