from cal.estimators import *
from language.Eventlog import Eventlog
from model.Petrinet import Petrinet

if __name__ == '__main__':
    log = Eventlog("../data/ex5.xes")
    net = Petrinet("../data/ex5.xes")

    weights_frequency = frequency_estimator(log, net)
    print(weights_frequency)

    weights_left_pair_frequency = lhpair_estimators(log, net)
    print(weights_left_pair_frequency)

    weights_right_pair_frequency = rhpair_estimators(log, net)
    print(weights_right_pair_frequency)

    weights_pairscale_frequency = pairscale_estimators(log, net)
    print(weights_pairscale_frequency)

    weights_fork = fork_estimators(log, net)
    print(weights_fork)

    weights_align = align_estimator(log, net)
    print(weights_align)
