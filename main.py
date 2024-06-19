import random
import time
import numpy as np
import pm4py
from matplotlib import pyplot as plt
from tqdm import tqdm

from cal.estimators import *
from cal.ML_estimator import *
from conformance.wasserstein import *
from language.Eventlog import *
from model.Petrinet import *
from sim.Cosmos import Cosmos
from sim.ABC import ABC

from pm4py.visualization.transition_system import visualizer as rg_visualizer


def xp_test(filename, miner):

    ev = Eventlog(filename)
    pn = ev.discover_pn_inductive()

    frequency_weights = frequency_estimator(ev, pn)
    abc = ABC("testg_abc", ev, pn)
    cosmos = Cosmos("testg_cosmos", ev, pn)
    mle = MaxLikelihoodEstimators(ev, pn)

def xp_estimators(filename, miner):

    ev = Eventlog(filename)
    pn = ev.discover_pn_inductive()

    # frequency_estimators
    weights_frequency = frequency_estimator(ev, pn)
    pn_language_frequency = pn.unfold_rg(ev, weights_frequency)
    print("frequency estimator:")
    print("\tloglh: " + str(log_likelihood(ev.get_language_pm4py(), pn_language_frequency)))
    print("\temd: " + str(emd_norm_lev(ev.get_language_pm4py(), pn_language_frequency)))
    pn.export_pnml(filename.replace("/", "_") + "_" + miner + "_frequency", weights_frequency)

    # lhpair_estimators
    weights_lhpair = lhpair_estimators(ev, pn)
    pn_language_lhpair = pn.unfold_rg(ev, weights_lhpair)
    print("lhpair estimator:")
    print("\tloglh: " + str(log_likelihood(ev.get_language_pm4py(), pn_language_lhpair)))
    print("\temd: " + str(emd_norm_lev(ev.get_language_pm4py(), pn_language_lhpair)))
    pn.export_pnml(filename.replace("/", "_") + "_" + miner + "_lhpair", weights_lhpair)

    # rhpair_estimators
    weights_rhpair = rhpair_estimators(ev, pn)
    pn_language_rhpair = pn.unfold_rg(ev, weights_rhpair)
    print("rhpair estimator:")
    print("\tloglh: " + str(log_likelihood(ev.get_language_pm4py(), pn_language_rhpair)))
    print("\temd: " + str(emd_norm_lev(ev.get_language_pm4py(), pn_language_rhpair)))
    pn.export_pnml(filename.replace("/", "_") + "_" + miner + "_rhpair", weights_rhpair)

    # pairscale_estimators
    weights_pairscale = pairscale_estimators(ev, pn)
    pn_language_pairscale = pn.unfold_rg(ev, weights_pairscale)
    print("pairscale estimator:")
    print("\tloglh: " + str(log_likelihood(ev.get_language_pm4py(), pn_language_pairscale)))
    print("\temd: " + str(emd_norm_lev(ev.get_language_pm4py(), pn_language_pairscale)))
    pn.export_pnml(filename.replace("/", "_") + "_" + miner + "_pairscale", weights_pairscale)

    # fork_estimators
    weights_fork = fork_estimators(ev, pn)
    pn_language_fork = pn.unfold_rg(ev, weights_fork)
    print("fork estimator:")
    print("\tloglh: " + str(log_likelihood(ev.get_language_pm4py(), pn_language_fork)))
    print("\temd: " + str(emd_norm_lev(ev.get_language_pm4py(), pn_language_fork)))
    pn.export_pnml(filename.replace("/", "_") + "_" + miner + "_fork", weights_fork)

def xp_abc(filename, miner):

    ev = Eventlog(filename)
    pn = ev.discover_pn_inductive()
    abc = ABC(filename.replace("/", "_"), ev, pn)

    N = 100
    threshold = [0.6, 0.5, 0.4, 0.3, 0.2]
    njob = 1

    result, sol, elapsed = abc.compute_smcABC(N=N, threshold=threshold, njob=njob, v=1)

    print(filename + " with " + miner)
    print("\tparticle: " + str(N))
    print("\tthreshold: " + str(threshold))
    print("\trEMD: " + str(emd_norm_lev(ev.get_language_pm4py(), pn.unfold_rg(ev,  sol[-1]["weights"]))))
    print("\tComputation time: " + str(elapsed) + "seconds (njob:" + str(njob) + ")")
    #pn.export_pnml(filename.replace("/", "_"), sol[-1]["weights"])

def xp_cosmos(filename, miner):

    ev = Eventlog(filename)
    pn = ev.discover_pn_inductive()

    c = Cosmos("test_cosmos", ev, pn)
    sim1_nrs, sim2_nrs, sim3_nrs, sim4_nrs, sim5_nrs, sim6_nrs, sim7_nrs, sim8_nrs, sim9_nrs, sim10_nrs, sim11_nrs, sim12_nrs, sim13_nrs, sim14_nrs, sim15_nrs = ([] for _ in range(15))
    sim1_emds, sim2_emds, sim3_emds, sim4_emds, sim5_emds, sim6_emds, sim7_emds, sim8_emds, sim9_emds, sim10_emds, sim11_emds, sim12_emds, sim13_emds, sim14_emds, sim15_emds = ([] for _ in range(15))
    sim1_times, sim2_times, sim3_times, sim4_times, sim5_times, sim6_times, sim7_times, sim8_times, sim9_times, sim10_times, sim11_times, sim12_times, sim13_times, sim14_times, sim15_times = ([] for _ in range(15))

    for _ in tqdm(range(20)):

        weights = {transition.get_name(): random.random() for transition in pn.get_transitions()}
        pn_language = pn.unfold_rg(ev, weights)

        # 1. level 0.99 | width 0.1
        start_time = time.time()
        sim1_nr, sim1_pn_language = c.simulate(weights, 0.99, 0.1)
        if list(sim1_pn_language.values()) != [0] * len(sim1_pn_language):
            sim1_nrs.append(sim1_nr)
            sim1_emds.append(emd_norm_lev(pn_language, sim1_pn_language))
            sim1_times.append(time.time() - start_time)

        # 2. level 0.99 | width 0.05
        start_time = time.time()
        sim2_nr, sim2_pn_language = c.simulate(weights, 0.99, 0.05)
        if list(sim2_pn_language.values()) != [0] * len(sim2_pn_language):
            sim2_nrs.append(sim2_nr)
            sim2_emds.append(emd_norm_lev(pn_language, sim2_pn_language))
            sim2_times.append(time.time() - start_time)

        # 3. level 0.99 | width 0.01
        start_time = time.time()
        sim3_nr, sim3_pn_language = c.simulate(weights, 0.99, 0.01)
        if list(sim3_pn_language.values()) != [0] * len(sim3_pn_language):
            sim3_nrs.append(sim3_nr)
            sim3_emds.append(emd_norm_lev(pn_language, sim3_pn_language))
            sim3_times.append(time.time() - start_time)

        # 4. level 0.99 | width 0.005
        start_time = time.time()
        sim4_nr, sim4_pn_language = c.simulate(weights, 0.99, 0.005)
        if list(sim4_pn_language.values()) != [0] * len(sim4_pn_language):
            sim4_nrs.append(sim4_nr)
            sim4_emds.append(emd_norm_lev(pn_language, sim4_pn_language))
            sim4_times.append(time.time() - start_time)

        # 5. level 0.99 | width 0.001
        start_time = time.time()
        sim5_nr, sim5_pn_language = c.simulate(weights, 0.99, 0.001)
        if list(sim5_pn_language.values()) != [0] * len(sim5_pn_language):
            sim5_nrs.append(sim5_nr)
            sim5_emds.append(emd_norm_lev(pn_language, sim5_pn_language))
            sim5_times.append(time.time() - start_time)

        # 6. level 0.99 | width 0.0005
        start_time = time.time()
        sim6_nr, sim6_pn_language = c.simulate(weights, 0.99, 0.0005)
        if list(sim6_pn_language.values()) != [0] * len(sim6_pn_language):
            sim6_nrs.append(sim6_nr)
            sim6_emds.append(emd_norm_lev(pn_language, sim6_pn_language))
            sim6_times.append(time.time() - start_time)

        # 7. level 0.99 | width 0.0001
        start_time = time.time()
        sim7_nr, sim7_pn_language = c.simulate(weights, 0.99, 0.0001)
        if list(sim7_pn_language.values()) != [0] * len(sim7_pn_language):
            sim7_nrs.append(sim7_nr)
            sim7_emds.append(emd_norm_lev(pn_language, sim7_pn_language))
            sim7_times.append(time.time() - start_time)

    print("# level 0.99 | width 0.1 #")
    print("\t Average/Max/Min Number of Runs: " + str(sum(sim1_nrs) / len(sim1_nrs)) + " - " + str(max(sim1_nrs)) + " - " + str(min(sim1_nrs)))
    print("\t Average/Max/Min EMD between the unfolding language and the simulation language of the net: " + str(sum(sim1_emds) / len(sim1_emds)) + " - " + str(max(sim1_emds)) + " - " + str(min(sim1_emds)))
    print("\t Average/Max/Min Execution Time: " + str(sum(sim1_times) / len(sim1_times)) + " - " + str(max(sim1_times)) + " - " + str(min(sim1_times)))

    print("# level 0.99 | width 0.05 #")
    print("\t Average/Max/Min Number of Runs: " + str(sum(sim2_nrs) / len(sim2_nrs)) + " - " + str(max(sim2_nrs)) + " - " + str(min(sim2_nrs)))
    print("\t Average/Max/Min EMD between the unfolding language and the simulation language of the net: " + str(sum(sim2_emds) / len(sim2_emds)) + " - " + str(max(sim2_emds)) + " - " + str(min(sim2_emds)))
    print("\t Average/Max/Min Execution Time: " + str(sum(sim2_times) / len(sim2_times)) + " - " + str(max(sim2_times)) + " - " + str(min(sim2_times)))

    print("# level 0.99 | width 0.01 #")
    print("\t Average/Max/Min Number of Runs: " + str(sum(sim3_nrs) / len(sim3_nrs)) + " - " + str(max(sim3_nrs)) + " - " + str(min(sim3_nrs)))
    print("\t Average/Max/Min EMD between the unfolding language and the simulation language of the net: " + str(sum(sim3_emds) / len(sim3_emds)) + " - " + str(max(sim3_emds)) + " - " + str(min(sim3_emds)))
    print("\t Average/Max/Min Execution Time: " + str(sum(sim3_times) / len(sim3_times)) + " - " + str(max(sim3_times)) + " - " + str(min(sim3_times)))

    print("# level 0.99 | width 0.005 #")
    print("\t Average/Max/Min Number of Runs: " + str(sum(sim4_nrs) / len(sim4_nrs)) + " - " + str(max(sim4_nrs)) + " - " + str(min(sim4_nrs)))
    print("\t Average/Max/Min EMD between the unfolding language and the simulation language of the net: " + str(sum(sim4_emds) / len(sim4_emds)) + " - " + str(max(sim4_emds)) + " - " + str(min(sim4_emds)))
    print("\t Average/Max/Min Execution Time: " + str(sum(sim4_times) / len(sim4_times)) + " - " + str(max(sim4_times)) + " - " + str(min(sim4_times)))

    print("# level 0.99 | width 0.001 #")
    print("\t Average/Max/Min Number of Runs: " + str(sum(sim5_nrs) / len(sim5_nrs)) + " - " + str(max(sim5_nrs)) + " - " + str(min(sim5_nrs)))
    print("\t Average/Max/Min EMD between the unfolding language and the simulation language of the net: " + str(sum(sim5_emds) / len(sim5_emds)) + " - " + str(max(sim5_emds)) + " - " + str(min(sim5_emds)))
    print("\t Average/Max/Min Execution Time: " + str(sum(sim5_times) / len(sim5_times)) + " - " + str(max(sim5_times)) + " - " + str(min(sim5_times)))

    print("# level 0.99 | width 0.0005 #")
    print("\t Average/Max/Min Number of Runs: " + str(sum(sim6_nrs) / len(sim6_nrs)) + " - " + str(max(sim6_nrs)) + " - " + str(min(sim6_nrs)))
    print("\t Average/Max/Min EMD between the unfolding language and the simulation language of the net: " + str(sum(sim6_emds) / len(sim6_emds)) + " - " + str(max(sim6_emds)) + " - " + str(min(sim6_emds)))
    print("\t Average/Max/Min Execution Time: " + str(sum(sim6_times) / len(sim6_times)) + " - " + str(max(sim6_times)) + " - " + str(min(sim6_times)))

    print("# level 0.99 | width 0.0001 #")
    print("\t Average/Max/Min Number of Runs: " + str(sum(sim7_nrs) / len(sim7_nrs)) + " - " + str(max(sim7_nrs)) + " - " + str(min(sim7_nrs)))
    print("\t Average/Max/Min EMD between the unfolding language and the simulation language of the net: " + str(sum(sim7_emds) / len(sim7_emds)) + " - " + str(max(sim7_emds)) + " - " + str(min(sim7_emds)))
    print("\t Average/Max/Min Execution Time: " + str(sum(sim7_times) / len(sim7_times)) + " - " + str(max(sim7_times)) + " - " + str(min(sim7_times)))


if __name__ == '__main__':

    miner = "inductive"
    filename = "rl_data/BPIC13_closed.xes"

    xp_abc(filename, miner)
