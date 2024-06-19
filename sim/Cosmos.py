import os
import shutil

class Cosmos:

    def __init__(self, name, ev, pn):

        self.name, self.ev, self.pn = "xp/" + name, ev, pn
        self.weight_dir = self.name + "/Weight"
        self.result_dir = self.name + "/Result"

        if os.path.exists(self.name):
            shutil.rmtree(self.name)
        os.mkdir(self.name)
        os.mkdir(self.weight_dir)
        os.mkdir(self.result_dir)

        with open(self.name + "/gspn1.gspn", "w") as file:
            file.write(self.pn.gen_gspn())
        with open(self.name + "/lha1.lha", "w") as file:
            file.write(self.ev.gen_lha_m1(pn))

        os.system("Cosmos/bin/Cosmos " + self.name + "/gspn1.gspn " + self.name + "/lha1.lha --tmp-path " + self.name + " --tmp-status keep --max-run 0 -v 0")

    def simulate(self, weights, level=0.99, width=0.001):

        ev_language = self.ev.get_language()

        with open(self.weight_dir + "/weights.txt", "w") as file:
            for weight in weights:
                file.write("w_" + weight + ":" + str(weights[weight]) + "\n")

        os.system("Cosmos/bin/Cosmos "
                  + self.name + "/gspn1.gspn "
                  + self.name + "/lha1.lha "
                  + "--tmp-path " + self.name + " --tmp-status reuse "
                  + "--weight-param " + self.weight_dir + "/weights.txt "
                  + "--output-result-name " + self.result_dir + "/result "
                  + "--level " + str(level) + " "
                  + "--width " + str(width) + " "
                  + "-v 0")

        with open(self.result_dir + "/result.res", "r") as file:
            nr, pn_language = 0, {}
            id = 0
            for line in file.readlines():
                if line[:16] == "Estimated value:":
                    pn_language[ev_language[id].get_seq()] = float(line[16:])
                    id += 1
                if line[:12] == "Total paths:":
                    nr = int(line[12:])

        return nr, pn_language
