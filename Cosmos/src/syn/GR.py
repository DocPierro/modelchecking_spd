import math
import numpy as np
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import GPy
from scipy.special import erf
import os
import random

kernel = GPy.kern.RBF(input_dim=1, variance=1., lengthscale=1.)

def sample(xin):
    return random.random()

def normpdf(x):
    return np.exp(-x*x/2)/np.sqrt(2*math.pi)

def normcdf(x):
    return (1+ erf(x/np.sqrt(2)))/2

def findMax(m,fmin):
    n = 1000
    X2 = np.linspace(minv,maxv,num=n).reshape((n,1))
    mu,s2 = m.predict(X2)

    t = np.argmax((fmin-mu) * normcdf( (fmin-mu)/np.sqrt(s2) ) + np.sqrt(s2)*normpdf( (fmin-mu)/np.sqrt(s2) ))
    t2 = minv + ( maxv - minv)*t/n

    return t2

def optstep(X,Y):
    m = GPy.models.GPRegression(X,Y,kernel)
    m.optimize_restarts(num_restarts = 10)
    nx = findMax(m,min(Y))
    X2 = np.vstack((X,nx))
    Y2 = np.vstack((Y,sample(nx)))
    return X2,Y2,m


minv = 300
maxv = 1100

X = np.array([[minv],
       [ maxv]])

Y = np.array([[sample(minv)],
       [ sample(maxv)]])

print X

for c in range(0,1):
    X,Y,m=optstep(X,Y)
    m.plot()
    display(m)
    plt.savefig(format('gaussfig%i'%c))
