import numpy
from Levenshtein import distance as levenshtein_distance
from pm4py.algo.evaluation.earth_mover_distance.variants import pyemd


def emd(l1, l2):
    distance = numpy.zeros((len(l1), len(l2)))
    for x, t1 in enumerate(l1):
        for y, t2 in enumerate(l2):
            distance[x, y] = 0 if t1.get_seq() == t2.get_seq() else 1
    return pyemd.emd(numpy.array([t1.get_prob() for t1 in l1]), numpy.array([t2.get_prob() for t2 in l2]), distance)


def emd_levenshtein(l1, l2):
    distance = numpy.zeros((len(l1), len(l2)))
    for x, t1 in enumerate(l1):
        for y, t2 in enumerate(l2):
            distance[x, y] = levenshtein_distance(t1.get_seq(), t2.get_seq())
    return pyemd.emd(numpy.array([t1.get_prob() for t1 in l1]), numpy.array([t2.get_prob() for t2 in l2]), distance)


def emd_normalized_levenshtein(l1, l2):
    distance = numpy.zeros((len(l1), len(l2)))
    for x, t1 in enumerate(l1):
        for y, t2 in enumerate(l2):
            if len(t1.get_seq()) == 0 or len(t2.get_seq()) == 0:
                distance[x, y] = 1.
            else:
                distance[x, y] = float(levenshtein_distance(t1.get_seq(), t2.get_seq())) / float(max(len(t1.get_seq()), len(t2.get_seq())))
    return pyemd.emd(numpy.array([t1.get_prob() for t1 in l1]), numpy.array([t2.get_prob() for t2 in l2]), distance)


def emd_norm_lev(l1, l2):

        nc1, nc2 = 0, 0
        words = set(l1) & set(l2)
        for w in words:
            if w in l1:
                nc1 = nc1 + l1[w]
            if w in l2:
                nc2 = nc2 + l2[w]

        hist1 = numpy.zeros(len(words))
        hist2 = numpy.zeros(len(words))
        distance = numpy.zeros((len(words), len(words)))
        for x, xw in enumerate(words):
            for y, yw in enumerate(words):
                if xw in l1:
                    hist1[x] = l1[xw] / nc1
                if yw in l2:
                    hist2[y] = l2[yw] / nc2
                if len(xw) == 0 or len(yw) == 0:
                    distance[x, y] = 1.
                else:
                    maxl = float(max(len(xw), len(yw)))
                    distance[x, y] = float(levenshtein_distance(xw, yw)) / maxl

        return pyemd.emd(hist1, hist2, distance)


def log_likelihood(l1, l2):
    lh = 0
    for tt in l2:
        lh += l1[tt] * numpy.log(l2[tt])
    return -lh
