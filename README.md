# A statistical model checking approach for probabilistic process mining

This repository hosts the implementation of the methodology outlined in the paper titled "A statistical model checking approach for probabilistic process mining". The provided code facilitates an estimation of the weight function for mining a stochastic Petri net within the context of stochastic process discovery.

## Contents
**Scripts:** Contains the necessary scripts to execute an optimized search of weight parameters for stochastic Petri net mining.<br/>
&emsp; ***language/Eventlog*** contains the necessary to import and manipulate an eventlog.<br/>
&emsp; ***model/Petrinet*** contains the necessary to work with a stochastic and non-stochastic Petri net.<br/>
&emsp; ***cal/estimators*** contains a simple re-implementation of the ***Burke, A, Leemans, S.J.J and Wynn, M. T. (2021) - Stochastic Process Discovery By Weight Estimation*** estimators compatible with our object:<br/>
&emsp; ***Conformance/wasserstein*** contains the implementation of the Earth's mover distance and its restrictive version.<br/>
&emsp; ***Cosmos*** contains the Cosmos bin, include and lib.<br/>
&emsp; ***sim/ABC*** contains the implementation of the ABC-SMC.<br/>

**Data:** Includes a selection of event logs in ***.xes*** utilized for experimentation.<br/>
&emsp; ***rl_data/BPIC13_closed.xes*** -> [https://data.4tu.nl/datasets/1987a2a6-9f5b-4b14-8d26-ab7056b17929](https://data.4tu.nl/datasets/1987a2a6-9f5b-4b14-8d26-ab7056b17929)<br/>
&emsp; ***rl_data/BPIC13_incidents.xes*** -> [https://data.4tu.nl/datasets/1987a2a6-9f5b-4b14-8d26-ab7056b17929](https://data.4tu.nl/datasets/0fc5c579-e544-4fab-9143-fab1f5192432)<br/>
&emsp; ***rl_data/BPIC13_open.xes*** -> [https://data.4tu.nl/datasets/7aafbf5b-97ae-48ba-bd0a-4d973a68cd35](https://data.4tu.nl/datasets/7aafbf5b-97ae-48ba-bd0a-4d973a68cd35)<br/>
&emsp; ***rl_data/BPIC17_offerlog.xes*** -> [https://data.4tu.nl/datasets/cc497753-1175-41f6-a107-425787c54266](https://data.4tu.nl/datasets/cc497753-1175-41f6-a107-425787c54266)<br/>
&emsp; ***rl_data/BPIC20_dd.xes*** -> [https://data.4tu.nl/datasets/6a0a26d2-82d0-4018-b1cd-89afb0e8627f](https://data.4tu.nl/datasets/6a0a26d2-82d0-4018-b1cd-89afb0e8627f)<br/>
&emsp; ***rl_data/BPIC20_rfp.xes*** -> [https://data.4tu.nl/datasets/a6f651a7-5ce0-4bc6-8be1-a7747effa1cc](https://data.4tu.nl/datasets/a6f651a7-5ce0-4bc6-8be1-a7747effa1cc)<br/>

## Installation
This project requires Python 3.9 or higher. The required libraries and their versions are listed in the requirements file. Follow these steps to set up the codebase:

1. Clone this repository to your local machine:<br/>
&emsp; `git clone https://github.com/DocPierro/optimised_spd`<br/>
2. Navigate to the project directory:<br/>
&emsp; `cd optimised_spd`<br/>
3. Set up a virtual environment using Conda (optional but recommended):<br/>
&emsp; a. Create a new Conda environment:<br/>
&emsp; &emsp; `conda create -n myenv python=3.9`<br/>
&emsp; b. Activate the Conda environment:<br/>
&emsp; &emsp; `conda activate myenv`<br/>
4. Install the required libraries from the ***requirements.txt*** file:<br/>
&emsp; `pip install -r "requirements.txt"`<br/>
5. Compile and install Cosmos following the instructions at https://cosmos.lacl.fr/gettingstart.html
6. Modify the ***main.py*** script according to your needs.
7. Launch the script:<br/>
&emsp; `python main.py`

## How to use
Using the ***main.py*** script:
1. Import an event log from an ***.xes*** file:<br/>
&emsp; `ev = Eventlog(filename)`<br/>
2. Construct a control flow Petri net using discovery algorithms:<br/>
&emsp; a. Inductive miner:<br/>
&emsp; &emsp; `pn = ev.discover_pn_inductive()`<br/>
&emsp; b. Heuristics miner:<br/>
&emsp; &emsp; `pn = ev.discover_pn_heursitics()`<br/>
&emsp; c. Alpha miner:<br/>
&emsp; &emsp; `pn = ev.discover_pn_alpha()`<br/>
3. Specify the estimation parameters:<br/>
&emsp; ***N***: Specify the number of particles.<br/>
&emsp; ***threshold***: Specify the list of tolerance.<br/>
&emsp; ***njob***: Specify the number of job.<br/>
&emsp; ***verbose***: Specify the verbose level.<br/>
4. Launch:<br/>
&emsp; xp_abc(filename, miner)

## Contact

For any inquiries or assistance, please contact pierre.cry@centralesupelec.fr
