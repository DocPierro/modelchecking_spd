from itertools import chain

import numpy
import sympy


def jacobian(G, W):
    J = sympy.zeros(len(G), len(W))
    for x, fi in enumerate(G):
        for y, wi in enumerate(W):
            J[x, y] = sympy.diff(fi, wi)
    return J


def gradient_descent(ev, pn, lr=1, iter=100):

    G = sympy.Matrix([eq.subs({trace[0]: trace[3] for trace in ev.get_language()[:-1]}) for eq in pn.get_eqs()])
    W = sympy.symbols(["w_" + transition.get_name() for transition in pn.get_transitions()])
    J = jacobian(G, W)

    x, x_sub = numpy.full(len(W), 1), {wi: 1 for wi in W}
    F = [(1 / 2 * G.transpose().subs(x_sub) * G.subs(x_sub))[0]]

    for i in range(iter):
        x = numpy.subtract(x, list(chain.from_iterable(numpy.array(lr * J.transpose().subs(x_sub) * G.subs(x_sub)).astype(numpy.float64))))
        x_sub = {str(w): x[i] for i, w in enumerate(W)}
        F.append((1/2 * G.transpose().subs(x_sub) * G.subs(x_sub))[0])
        #if abs(F[i+1] - F[i]) <= lr:
            #break

    return x_sub, F
