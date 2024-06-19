import os
import time
import random
import shutil
import multiprocessing

from datetime import datetime as dt
import matplotlib.pyplot as plt
import numpy as np
from colorama import Fore
from scipy.stats import multivariate_normal

from conformance.wasserstein import emd_normalized_levenshtein
from language.Eventlog import Trace
from sim.minimax_tilting_sampler import TruncatedMVN


class ABC:

    def __init__(self, name, ev, pn):

        self.name, self.ev, self.pn = "xp/" + name, ev, pn
        self.weight_dir = self.name + "/Weight"
        self.result_dir = self.name + "/Result"
        self.plot_dir = self.name + "/Plot"

        if os.path.exists(self.name):
            shutil.rmtree(self.name)
        os.mkdir(self.name)
        os.mkdir(self.weight_dir)
        os.mkdir(self.result_dir)
        os.mkdir(self.plot_dir)

        with open(self.name + "/gspn1.gspn", "w") as file:
            file.write(self.pn.gen_gspn())
        with open(self.name + "/lha1.lha", "w") as file:
            file.write(self.ev.gen_lha_m1(self.pn))

    def get_ev(self):
        return self.ev

    def get_pn(self):
        return self.pn

    ####################################################################################################

    def read_result(self, t, n):
        ev_language, pn_language, = self.ev.get_language(), []
        with open(self.result_dir + "/result_" + str(t) + "_" + str(n) + ".res", "r") as file:
            id = 0
            for line in file.readlines():
                if line[:16] == "Estimated value:":
                    pn_language.append(Trace(id, ev_language[id].get_seq(), ev_language[id].get_word(), -1, float(line[16:]), ev_language[id].get_mapping1(), ev_language[id].get_mapping3()))
                    id += 1
        return pn_language

    def write_weight(self, t, n, particle):
        with open(self.weight_dir + "/weights_" + str(t) + "_" + str(n) + ".txt", "w") as file:
            for weight in particle:
                file.write("w_" + weight + ":" + str(particle[weight]) + "\n")

    ####################################################################################################

    def print_particle(self, accepted, t, n, s, particle, ev_language, pn_language, dist_emd, v):
        if accepted:
            if v >= 3:
                print(Fore.GREEN + "####################################################################################################")
            if v >= 2:
                print(Fore.GREEN + "### ACCEPTED: [t = " + str(t) + "; n = " + str(n) + "; Simulation = " + str(s) + "] -> " + str(dist_emd) + " ###")
            if v >= 3:
                print(Fore.GREEN + "theta:" + str(particle))
                print("Log Language: " + str(ev_language))
                print("Net Language: " + str(pn_language))
                print("emd: " + str(dist_emd))
                print("####################################################################################################")
                print(Fore.RESET)
        else:
            if v >= 3:
                print(Fore.RED + "####################################################################################################")
            if v >= 2:
                print(Fore.RED + "### REJECTED: [t = " + str(t) + "; n = " + str(n) + "; Simulation = " + str(s) + "] -> " + str(dist_emd) + " ###")
            if v >= 3:
                print(Fore.RED + "w: " + str(particle))
                print("Log Language: " + str(ev_language))
                print("Net Language: " + str(pn_language))
                print("emd: " + str(dist_emd))
                print("####################################################################################################")
                print(Fore.RESET)

    def print_particles(self, particles, threshold, njob):
        print(Fore.BLUE + "####################################################################################################")
        print("ABC[t=" + str(particles["t"]) + ";N=" + str(len(particles["particles"])) + ";threshold=" + str(threshold) + ";njob=" + str(njob) + "] in " + dt.strftime(dt.utcfromtimestamp(particles["execution_time"]), '%H:%M:%S') + ":")
        for n, particle in enumerate(particles["particles"]):
            print("\t" + str(n+1) + ". " + str(particle))
        print(Fore.BLUE + "####################################################################################################")
        print(Fore.RESET)

    def plot_particles(self, particles, threshold, njob):
        weights_sol = {}
        for transition in self.pn.get_transitions():
            sample = []
            for n, particle in enumerate(particles["particles"]):
                sample.append(particle["theta"][transition.get_name()])
            fig = plt.figure()
            y, x, _ = plt.hist(sample, bins=100)
            plt.title("ABC[t=" + str(particles["t"]) + ";N=" + str(len(particles["particles"])) + ";threshold=" + str(threshold) + ";njob=" + str(njob) + "] in " + dt.strftime(dt.utcfromtimestamp(particles["execution_time"]), '%H:%M:%S'))
            plt.savefig(self.plot_dir + "/t" + str(particles["t"]) + "_" + transition.get_name())
            plt.close(fig)
            weights_sol[transition.get_name()] = x[np.where(y == y.max())][0]
        return weights_sol

    def export_result(self, result, threshold, njob):
        with open(self.name + "/result.res", "w") as file:
            for res in result:
                file.write("ABC[t=" + str(res["t"]) + ";N=" + str(len(res["particles"])) + ";threshold=" + str(threshold[res["t"]]) + ";njob=" + str(njob) + "] in " + dt.strftime(dt.utcfromtimestamp(res["execution_time"]), '%H:%M:%S') + ":\n")
                for n, particle in enumerate(res["particles"]):
                    file.write("\t" + str(n + 1) + ". " + str(particle) + "\n")

    ####################################################################################################

    def get_multivariate_normal_sample(self, mean, cov):

        # multivariate_normal
        #sample = np.random.multivariate_normal(mean, cov)

        # truncated multivariate normal
        tmvn = TruncatedMVN(np.array(mean), cov, np.zeros(len(mean)), np.ones(len(mean)))
        sample = [i for ii in tmvn.sample(1).tolist() for i in ii]

        # truncated multivariate normal by multiple sampling (1 by 1)
        #sample = []
        #while len(sample) < len(mean):
        #    while True:
        #        cur_sample = np.random.multivariate_normal(mean, cov)[len(sample)]
        #        if 0 <= cur_sample <= 1:
        #            sample.append(cur_sample)
        #            break

        # truncated multivariate normal by multiple sampling (all in 1)
        #while True:
        #    sample = np.random.multivariate_normal(mean, cov)
        #    if np.all(sample >= 0) and np.all(sample <= 1):
        #        break

        return sample

    ####################################################################################################

    def random_uniform_sim(self, param):

        s = 0
        n, threshold, fixed_w, v = param
        ev_language = self.ev.get_language()

        while True:

            s += 1
            particle = {"theta": ({transition.get_name(): (random.random()) if not (transition.get_name() in fixed_w) else fixed_w[transition.get_name()]
                                   for transition in self.pn.get_transitions()})}
            self.write_weight(0, n, particle["theta"])

            os.system("Cosmos/bin/Cosmos " + self.name + "/gspn1.gspn " + self.name + "/lha1.lha --tmp-path " + self.name + " --tmp-status reuse --width 0.1 --weight-param " + self.weight_dir + "/weights_0_" + str(n) + ".txt --output-result-name " + self.result_dir + "/result_0_" + str(n) + " -v 0")
            pn_language = self.read_result(0, n)

            dist_emd = emd_normalized_levenshtein(ev_language, pn_language)
            if dist_emd <= threshold:
                self.print_particle(True, 0, n, s, particle, ev_language, pn_language, dist_emd, v)
                particle["weight"] = 1
                return particle
            else:
                self.print_particle(False, 0, n, s, particle, ev_language, pn_language, dist_emd, v)

    def random_multivariate_sim(self, param):

        s = 0
        t, n, old_particles, threshold, fixed_w, v = param
        ev_language = self.ev.get_language()

        while True:

            s += 1
            old_particle = random.choices([old_particle["theta"] for old_particle in old_particles],[old_particle["weight"] for old_particle in old_particles], k=1)[0]
            mean = list(old_particle.values())
            cov = np.cov([[list((particle["theta"]).values())[d] for particle in old_particles] for d in range(len(mean))])
            sample = self.get_multivariate_normal_sample(mean, cov)
            new_particle = {"theta": ({transition.get_name(): sample[i] if not (transition.get_name() in fixed_w) else fixed_w[transition.get_name()]
                                       for i, transition in enumerate(self.pn.get_transitions())})}
            self.write_weight(t, n, new_particle["theta"])

            os.system("Cosmos/bin/Cosmos " + self.name + "/gspn1.gspn " + self.name + "/lha1.lha --tmp-path " + self.name + " --tmp-status reuse --width 0.1 --weight-param " + self.weight_dir + "/weights_" + str(t) + "_" + str(n) + ".txt --output-result-name " + self.result_dir + "/result_" + str(t) + "_" + str(n) + " -v 0")
            pn_language = self.read_result(t, n)

            dist_emd = emd_normalized_levenshtein(ev_language, pn_language)
            if dist_emd <= threshold:
                self.print_particle(True, t, n, s, new_particle, ev_language, pn_language, dist_emd, v)
                var = multivariate_normal(mean, cov, allow_singular=True)
                new_particle["weight"] = 1/np.sum([old_particle["weight"]*var.pdf(list(old_particle["theta"].values())) for old_particle in old_particles])
                return new_particle
            else:
                self.print_particle(False, t, n, s, new_particle, ev_language, pn_language, dist_emd, v)

    def compute_smcABC(self, N=100, threshold=None, fixed_w=None, njob=1, v=0):

        if threshold is None:
            threshold = [0.9, 0.8, 0.7, 0.6, 0.5]
        if fixed_w is None:
            fixed_w = {}

        start = time.time()
        T, result, sol = len(threshold), [], []
        os.system("Cosmos/bin/Cosmos " + self.name + "/gspn1.gspn " + self.name + "/lha1.lha --tmp-path " + self.name + " --tmp-status keep --max-run 0 -v 0")

        multiprocessing.set_start_method('spawn', force=True)
        with multiprocessing.get_context("spawn").Pool(njob) as pool:
            for t in range(T):
                if t == 0:
                    if njob > 1:
                        particles = pool.map(self.random_uniform_sim, zip(range(1, N+1), [threshold[t]] * N, [fixed_w] * N, [v] * N))
                    else:
                        particles = [self.random_uniform_sim([n+1, threshold[t], fixed_w, v]) for n in range(N)]
                else:
                    if njob > 1:
                        particles = pool.map(self.random_multivariate_sim, zip([t] * N, range(1, N+1), [result[t-1]["particles"]] * N, [threshold[t]] * N, [fixed_w] * N, [v] * N))
                    else:
                        particles = [self.random_multivariate_sim([t, n+1, result[t-1]["particles"], threshold[t], fixed_w, v]) for n in range(N)]

                    weights = [particle["weight"] for particle in particles]
                    for particle in particles:
                        particle["weight"] = particle["weight"]/np.sum(weights)

                elapsed_t = time.time() - start
                result.append({"t": t, "particles": particles, "execution_time": elapsed_t})
                sol.append({"t": t, "weights": self.plot_particles(result[t], threshold[t], njob)})
                if v >= 1:
                    self.print_particles(result[t], threshold[t], njob)

        self.export_result(result, threshold, njob)

        elapsed = (time.time() - start)
        return result, sol, elapsed
