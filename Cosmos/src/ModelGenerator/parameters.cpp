/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini Benoît Barbot & Hilal Djafri                       *
 * Website: http://www.lsv.ens-cachan.fr/Software/cosmos                       *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 * file parameters.cpp                                                         *
 * Created by Benoit Barbot on 08/02/12.                                       *
 *******************************************************************************
 */

#include "parameters.hpp"

#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>

using namespace std;

#define BUILD_VERSION "Cosmos 1.6"

#ifdef __APPLE__
#include <mach-o/dyld.h>
void findBinaryPath(parameters& P) {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0){
        printf("buffer too small; need size %u\n", size);
        exit(EXIT_FAILURE);
    }
    
    P.Path=path;
    std::string::size_type t = P.Path.find_last_of("/");
    P.Path = P.Path.substr(0, t);
    P.Path.append("/");
}
#elif __linux__
void findBinaryPath(parameters& P) {
    char path[1024];
    char link[512];
    sprintf(link, "/proc/%d/exe", getpid());
    int sz = readlink(link, path, 1024);
    if (sz >= 0) {
        path[sz] = 0;
        P.Path = path;
        std::string::size_type t = P.Path.find_last_of("/");
        P.Path = P.Path.substr(0, t);
        P.Path.append("/");
    }
}
#else
#error "Operating system not supported"
#endif


/**
 * Constructor for parameters, set all default values
 */
parameters::parameters() :
commandLine(""),
verbose(2),
interactive(false),
updatetime(100),
seed(0),
randomGen(MT19937),
Njob(1),

epsilon(0.000001),
continuousStep(1),
Level(0.99),
Width(0.001),
Batch(1000),
MaxRuns(2000000),
sequential(true),
relative(false),
chernoff(false),

comp_uuid("tmpuuid"),
tmpPath("tmp"),
tmpStatus((TmpStat)(TS_GEN | TS_BUILD | TS_RUN | TS_DESTROY)),
reuse(false),
modelType(GSPN),
lhaType(DET),
Path(""),
PathGspn(""),
PathLha(""),
result_name("Result"),
constants(),
weight_file("weights.txt"),
generateLHA(NoGen),
loopLHA(0.0),
loopTransientLHA(0.0),
CSLformula(""),
externalHASL(""),

localTesting(false),
RareEvent(false),
DoubleIS(false),
BoundedRE(0),
horizon(100),
BoundedContinuous(false),

CountTrans(false),
StringInSpnLHA(false),


GMLinput(false),
computeStateSpace(0),
lumpStateSpace(false),
alligatorMode(false),
guiGreatSpnMode(false),
unfold(""),
isTTY(true),
terminalWidth(80),
graceFullEnding(0),

gcccmd(CPP_COMPILER),
boostpath(BOOST_PATH),
gccflags("-Wno-return-type -std=gnu++11"),
boostlib(BOOST_LIB),
lightSimulator(false),

prismPath(""),
dataoutput(""),
dataraw(""),
datatrace(""),
sampleResol(0.0),
dataPDFCDF(""),
gnuplotDriver(""),
tracedPlace(),
dotfile(""),
magic_values(""),
use_magic_print(false),
is_domain_impl_set(false),

HaslFormulas(vector<HaslFormulasTop*>(0)),
HaslFormulasname(vector<string>(0)),
nbAlgebraic(0),
nbQualitatif(0),
nbPlace(0)
//prismPath = "/import/barbot/prism-4.0.1-linux64/bin/prism";
{
    tracedPlace.insert(pair<string, int>("ALL",0));
    isTTY = isatty(fileno(stdout));
    struct winsize ws;
    if(isTTY && ioctl(fileno(stdout), TIOCGWINSZ, &ws)>=0){
        if(ws.ws_col == 0){
            isTTY=false;
        }else{
            terminalWidth=ws.ws_col;
        }
    }
    
    const char *env = getenv("FROM_GUI");
    guiGreatSpnMode = (env != nullptr && 0 == strcmp(env, "1"))>0;
    if(guiGreatSpnMode){
        isTTY = true;
        terminalWidth =120;
        verbose=1;
    }
    
}

/**
 * Display the usage text
 */
void parameters::usage() {
    cout << "usage: Cosmos [-ghsr] [-vb arg] ";
    cout << "[--alligator-mode] ";
    cout << "[--level arg] [--width arg] [--batch arg] [--max-run arg] ";
    cout << "[--set-Horizon arg] [--njob arg] ";
    cout << "gspn_file lha_file" << endl;

    cout << "General options:" << endl;
    cout << "\t--version\tdisplay version number" << endl;
    cout << "\t-v,--verbose arg\tset the verbose level" << endl;
    cout << "\t-i,--interactive \tAsk the user to choose next transition" << endl;
    cout << "\t--update-time\t set the time in second beetween two updates of the display" << endl;
    cout << "\t-h,--help \tdisplay this message" << endl;
    cout << "\t--njob    \tset the number of parralel thread" << endl;
    cout << "\t--gppcmd  \tset the C++ compiler (default g++)" << endl;
    cout << "\t--gppflags\tset the C++ compiler flags (default -O3)" << endl;
    cout << "\t--reuse\ttry to load previous simulation and save simulations" << endl;
    cout << "\t--use-domainset\tuse the Set implementation on Places and Transitions" << endl;

    cout << "Option of simulation:" << endl;
    cout << "\t--level \tset the confidence level for the simulation (default=0.99)" << endl;
    cout << "\t--width \tset the width of the confidence interval (default=0.001)" << endl;
    cout << "\t--batch \tset the size of batch of simulation (default=1000)" << endl;
    cout << "\t--max-run \tset the maximal number of run (default=2000000)" << endl;
    cout << "\t--relative \tUse relative confidence interval instead of absolute one" << endl;
    cout << "\t--chernoff (level | width | nbrun)\tuse chernoff-hoeffding bound to compute the number of simulation" << endl;
    cout << "\t--seed \tSpecify the seed for the random generator, 0 allow to take a random value" << endl;
    cout << "\t--local-test \tUse local testing faster on big net" << endl;
    cout << "\t--const \toverride constant value of the model" << endl;
    cout << "\t--weight-param \tset the weight file for overriding Petri net transition weight" << endl;
    cout << "\t--random-generator (MT19937 | VDC | Kronecker) \tUse a pseudo random number generator" << endl;

    cout << "Rare event options:" << endl;
    cout << "\t-r \tUnbounded rare event mode" << endl;
    cout << "\t-b arg \tDiscrete bounded rare event mode, arg is the method to use to stop vector" << endl;
    cout << "\t--set-Horizon arg \tSet the horizon for bounded rare event mode arg must be an integer in discrete bounded case" << endl;
    cout << "\t-c \tContinuous bounded rare event mode." << endl;
    cout << "\t--epsilon arg \tError level of the fox glynn algorithme" << endl;
    cout << "\t--step-continuous arg \tSize of step of the continuous method" << endl;


    cout << "Miscellaneous options:" << endl;
    cout << "\t-g,--grmlinput \tforce use of grml file format for input file" << endl;
    cout << "\t--alligator-mode \toutput easy to parse result" << endl;
    cout << "\t--unfold arg \tUnfold the GSPN given as input" << endl;
    cout << "\t--output-model arg \tReturn the GSPN file given as input" << endl;
    cout << "\t--count-transition \tAdd a Hasl formula for wich count the number of time each transition occurs" << endl;
    cout << "\t--tmp-path arg \tPath to the temporary directory by default ./tmp/" << endl;
    cout << "\t--bin-path arg \tPath to the binary of cosmos (guess automatically)" << endl;
    cout << "\t--tmp-status arg \tDo not remove or do not rebuild tmp directory: 0 default->rebuild,destroy; 1->do not build;2->do not destroy;3-> do not build nor destroy" << endl;
    cout << "\t--debug-string \tAdd transition and place name to the compile file for debuging" << endl;
    cout << "\t-d,--outputdata \tOutput successive result in the blank separated file format" << endl;
    cout << "\t--output-result-name \tChange the name of the output file" << endl;
    cout << "\t--output-raw \tOutput the result of each trajectory in a file for debug purpose" << endl;
    cout << "\t--output-trace arg resol \tOutput the trace each trajectory in the file arg with a resolution of resol" << endl;
    cout << "\t--output-graph \tOutput the result of CDF or PDF formula in gnuplot file format" << endl;
    cout << "\t--output-dot \tOutput the Petri net in a dot file" << endl;
    cout << "\t--gnuplot-driver terminal\tRun gnuplot on the output datafile to produce graph use the given terminal for gnuplot" << endl;
    cout << "\t--trace-pt arg\tSpecify which place to trace in all the output file, arg is a comma separated list of places and transitions name" << endl;
    cout << "\t--HASL-expression \tAllow to define an HASL formula from the command line" << endl;
    cout << "\t--loop t1 [--transtient t2] \tGenerate an LHA that loop for t1 times unit and then t2 time unit. The --transient option alone do not do anything" << endl;
    cout << "\t--sampling t1 t2 \tGenerate an LHA that loop for t1 times unit and sample the average number of token each t2 time units" << endl;
    cout << "\t--formula f\t specify a CSL formula to use instead of an automata" << endl;
    cout << "\t--prism \tExport the state space and launch prism." << endl;
    cout << "\t-s,--state-space \tExport the state space." << endl;
    cout << "\t--lump-state-space \tLump the state space before exporting." << endl;
    cout << "\t--no-magic-print\tIgnore magic_print when using custom C code in models" << endl;
    

}


Poption parameters::parsersingleOpt(int i)const{
    switch (i) {
        case 'g':
            return CO_grml_input;
        case 'i':
            return CO_interactive;
        case 'h':
            return CO_help;
        case 's':
            return CO_state_space;
        case 'c':
            return CO_boundedcountiniouceRE;
        case 'r':
            return CO_rareevent;
        case 'b':
            return CO_boundedRE;
        case 'v':
            return CO_verbose;
        case 'p':
            return CO_prism;
        case 'd':
            return CO_output_data;
        default:
            return (Poption)i;
    }
}

/**
 * Parse the command line and set the parameter structure
 * with the option set by the user
 */
void parameters::parseCommandLine(int argc, char** argv) {
    
    //Find the path to the directory containing Cosmos binary.
    string st = argv[0];
    if (st == "./Cosmos"){Path = "";} //local directory
    else if(st.length()>6){Path=st.substr(0,st.length()-6);} //direct Path to Cosmos
    else findBinaryPath(*this); //Ask the system where Cosmos is (System dependant)
    
    
    commandLine = argv[0];
    for (int i = 1; i<argc; i++){
        commandLine += " ";
        commandLine += argv[i];
    }
    
    int c;

    while (1) {
        static struct option long_options[] ={
            /* Options for the simulator*/
            {"level",       required_argument, 0, CO_level},
            {"width",       required_argument, 0, CO_width},
            {"batch",       required_argument, 0, CO_batch},
            {"max-run",     required_argument, 0, CO_max_run},
            {"seed",        required_argument, 0, CO_seed},
            {"local-test",  no_argument      , 0, CO_local_test},
            {"sampling",    required_argument, 0, CO_sampling},
            {"loop",        required_argument, 0, CO_loop},
            {"transient",   required_argument, 0, CO_transient},
            {"formula",     required_argument, 0, CO_formula},
            {"chernoff",    required_argument, 0, CO_chernoff},
            {"relative",    no_argument      , 0, CO_relative},
            {"const",       required_argument, 0, CO_const},
            {"weight-param",required_argument, 0, CO_weight_param},
            {"reuse",       no_argument      , 0, CO_reuse},
            {"use-van-der-corput",no_argument, 0, CO_VDC},
            {"random-generator",required_argument, 0, CO_RandomGen},
            {"gracefull-ending", no_argument, 0, CO_gracefull_ending},

            /* Options for the rare event engine */
            {"rareevent",   no_argument      , 0, CO_rareevent},
            {"boundedcountiniousRE", no_argument, 0, CO_boundedcountiniouceRE},
            {"boundedRE",   required_argument, 0, CO_boundedRE},
            {"step-continuous", required_argument, 0, CO_step_continuous},
            {"epsilon",     required_argument, 0, CO_epsilon},
            {"set-Horizon", required_argument, 0, CO_set_Horizon},
            {"state-space", no_argument      , 0, CO_state_space},
            {"lump-state-space", no_argument , 0, CO_lump},
            {"prism",       no_argument      , 0, CO_prism},
            {"normalize-IS", no_argument     , 0, CO_normalize_IS},

            /* CosyVerif Options */
            {"gmlinput",    no_argument      , 0, CO_grml_input},
            {"grml-input",  no_argument      , 0, CO_grml_input},
            {"alligator-mode", no_argument   , 0, CO_alligator_mode},
            {"gui-greatSPN-mode",no_argument , 0, CO_gui_greatSPN_mode},

            /* Output function */
            {"verbose",     required_argument, 0, CO_verbose},
            {"interactive", no_argument      , 0, CO_interactive},
            {"update-time", required_argument, 0, CO_update_time},
            {"outputdata",  required_argument, 0, CO_output_data},
            {"output-data", required_argument, 0, CO_output_data},
            {"output-raw",  required_argument, 0, CO_output_raw},
            {"output-trace", required_argument, 0, CO_output_trace},
            {"output-PDFCDF", required_argument, 0, CO_output_graph},
            {"output-graph", required_argument, 0, CO_output_graph},
            {"output-dot", required_argument , 0, CO_output_dot},
            {"gnuplot-driver", required_argument, 0, CO_gnuplot_driver},
            {"trace-place", required_argument, 0, CO_trace_pt},
            {"trace-pt",    required_argument, 0, CO_trace_pt},
            {"output-result-name", required_argument, 0, CO_output_result_name},

            
            /* Miscellaneous options */
            {"not-gspn",    no_argument      , 0, CO_not_gspn},
            {"force-TTY",   no_argument      , 0, CO_force_TTY},
            {"unfold",      required_argument, 0, CO_unfold},
            {"output-model", required_argument,0, CO_output_model},
            {"HASL-formula", required_argument, 0, CO_HASL_formula},
            {"HASL-expression", required_argument, 0, CO_HASL_formula},
            {"njob",        required_argument, 0, CO_njob},
            {"gppcmd",      required_argument, 0, CO_gppcmd},
            {"gppflags",    required_argument, 0, CO_gppflags},
            {"light-simulator", no_argument  , 0, CO_light_simulator},
            {"help",        no_argument      , 0, CO_help},
            {"count-transition", no_argument , 0, CO_count_transition},
            {"debug-string", no_argument     , 0, CO_debug_string},
            {"tmp-path",    required_argument, 0, CO_tmp_path},
            {"tmp-status",  required_argument, 0, CO_tmp_status},
            {"bin-path",    required_argument, 0, CO_bin_path},
            {"prism-path",  required_argument, 0, CO_prism_path},
            {"magic-values", required_argument,0, CO_magic_values},
            {"no-magic-print", no_argument, 0, CO_use_magic_print},
            {"use-setdomain-impl", no_argument, 0, CO_domain_impl},
	        {"use-domainset", no_argument, 0, CO_domain_impl},
            {"version",     no_argument      , 0, CO_version},

            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "gihspcrb:v:d:",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (parsersingleOpt(c)) {
            case CO_help:
                usage();
                exit(EXIT_SUCCESS);
                break;

            case CO_verbose:verbose = atoi(optarg);
                if (verbose >= 4)StringInSpnLHA = true;
                break;

            case CO_interactive:
                interactive = true;
                StringInSpnLHA = true;
                break;

            case CO_update_time:updatetime = chrono::duration_cast<chrono::milliseconds>(chrono::duration<double> (atof(optarg)));
                break;

            case CO_grml_input:GMLinput = true;
                break;

            case CO_rareevent:
                RareEvent = true;
                StringInSpnLHA = true; // Need to know the name of place to find
                // place begining with "RE_"
                localTesting = false; //Need to unfire transition not implemented for local testing
                relative = true;
                break;

            case CO_relative:
                relative = true;
                break;

            case CO_normalize_IS:
                DoubleIS = true;
                break;
                
            case CO_gracefull_ending:
                graceFullEnding = 1;
                break;

            case CO_boundedRE:BoundedRE = atoi(optarg);
                StringInSpnLHA = true;
                RareEvent = true;
                relative = true;
                break;

            case CO_boundedcountiniouceRE:BoundedContinuous = true;
                RareEvent = true;
                break;

            case CO_set_Horizon :horizon = atof(optarg);
                break;

            case CO_state_space:computeStateSpace = 2;
                StringInSpnLHA = true;
                localTesting = false; //Need to unfire transition, not implemented for local testing
                break;
                
            case CO_lump: lumpStateSpace = true;
                break;

            case CO_prism:computeStateSpace = 1;
                StringInSpnLHA = true;
                localTesting = false; //Need to unfire transition, not implemented for local testing
                break;

            case CO_alligator_mode:alligatorMode = true;
                verbose = 0;
                break;
                
            case CO_force_TTY : isTTY = true; break;
                
            case CO_gui_greatSPN_mode:
                guiGreatSpnMode =true;
                isTTY = true;
                terminalWidth=120;
                break;

            case CO_unfold:unfold = optarg;
                break;
            case CO_output_model:outputModel =optarg;
                break;

            case CO_level:Level = atof(optarg);
                break;
            case CO_width:
            {
                double w = atof(optarg);
                if(w==0.0)sequential=false;
                Width = w;
            }
                break;
            case CO_batch: Batch = atol(optarg);
                break;
            case CO_max_run: MaxRuns = atol(optarg);
                break;
            case CO_chernoff:
                chernoff = true;
                sequential = false;
                if (strcmp(optarg, "level") == 0)Level = 0;
                else if (strcmp(optarg, "width") == 0)Width = 0;
                else if (strcmp(optarg, "nbrun") == 0)MaxRuns = (unsigned long)-1;
                else {
                    cerr << "Required one of (level | width | nbrun) to specify which ";
                    cerr << "parameter should be computed" << endl;
                    usage();
                    exit(EXIT_FAILURE);
                }
                break;
            case CO_local_test: localTesting = !localTesting;
                break;
            case CO_njob: Njob = atoi(optarg);
                break;
            case CO_epsilon: epsilon = atof(optarg);
                break;
            case CO_step_continuous: continuousStep = atoi(optarg);
                break;
            case CO_output_data: dataoutput = optarg;
                break;
            case CO_output_raw: dataraw = optarg;
                break;
            case CO_output_dot: dotfile = optarg;
                break;
            case CO_magic_values: magic_values = optarg;
                use_magic_print = true;
                break;
            case CO_use_magic_print: use_magic_print = false;
                break;
            case CO_domain_impl: is_domain_impl_set = true;
                break;
            case CO_output_trace:
                datatrace = optarg;
                StringInSpnLHA = true;
                if (optind == argc) {
                    usage();
                    exit(EXIT_FAILURE);
                }
                sampleResol = atof(argv[optind]);
                optind++;
                break;

            case CO_output_graph: dataPDFCDF = optarg;
                break;
            case CO_count_transition: CountTrans = true;
                break;
            case CO_debug_string: StringInSpnLHA = true;
                break;
            case CO_tmp_path: tmpPath = optarg;
                break;
            case CO_bin_path: Path = optarg;
                break;
            case CO_tmp_status:
            {
                int order = 0;
                if (strcmp(optarg, "full") == 0) order = 0;
                else if (strcmp(optarg, "keep") == 0)order = 2;
                else if (strcmp(optarg, "reuse") == 0)order = 3;
                else if (strcmp(optarg, "only-build") == 0)order = 5;
                else if (strcmp(optarg, "only-gen") == 0)order = 6;
                else { order = atoi(optarg);};
                switch (order) {
                    case 0:
                        tmpStatus = (TmpStat)( TS_GEN | TS_BUILD | TS_RUN | TS_DESTROY);
                        break;
                    case 1:
                        tmpStatus = (TmpStat)( TS_RUN | TS_DESTROY);
                        break;
                    case 2:
                        tmpStatus = (TmpStat)( TS_GEN | TS_BUILD | TS_RUN );
                        break;
                    case 3:
                        tmpStatus = (TmpStat)( TS_GEN | TS_RUN );
                        break;
                    case 4:
                        tmpStatus = (TmpStat)( TS_BUILD | TS_RUN );
                    case 5:
                        tmpStatus = (TmpStat)( TS_GEN | TS_BUILD  );
                        break;
                    case 6:
                        tmpStatus = (TmpStat)( TS_GEN );
                        break;

                    default:
                        tmpStatus = (TmpStat)atoi(optarg);
                        break;
                }
                break;
            }
            case CO_reuse:
                tmpStatus = (TmpStat)(tmpStatus & (TS_GEN | TS_BUILD | TS_RUN));
                reuse = true;
                break;
            case CO_gppcmd: gcccmd = optarg;
                break;
            case CO_gppflags: gccflags = optarg;
                break;
            case CO_light_simulator:
                lightSimulator = true;
                break;
            case CO_seed: seed = atoi(optarg);
                break;
            case CO_VDC: randomGen = VDC;
            break;
            case CO_RandomGen:
                if( strcmp(optarg,"VDC")==0) randomGen = VDC;
                if( strcmp(optarg,"Kronecker")==0) randomGen = VDC;
                break;
            case CO_HASL_formula: externalHASL = optarg;
                break;
            case CO_loop:
                if(optarg[0]!='#'){
                    loopLHA = atof(optarg);
                    generateLHA = TimeLoop;
                }else{
                    const auto st = string(optarg);
                    loopLHA = stod(st.substr(1,st.length()-1));
                    generateLHA = ActionLoop;
                }
                PathLha = "LOOP";
                break;
            case CO_sampling:
                loopLHA = atof(optarg);
                generateLHA = SamplingLoop;
                PathLha = "SAMPLING";
                if (optind == argc) {
                    usage();
                    exit(EXIT_FAILURE);
                }
                loopTransientLHA = atof(argv[optind]);
                optind++;
                break;
            case CO_transient: loopTransientLHA = atof(optarg);
                break;
            case CO_formula: CSLformula = optarg;
                generateLHA = CSL_LHA;
                break;

            case CO_gnuplot_driver: gnuplotDriver = optarg;
                break;

            case CO_trace_pt:
            {
                tracedPlace.clear();
                const string st = optarg;
                for(size_t i=0 ; i< st.length();){
                    auto j = st.find(',',i+1);
                    tracedPlace.insert(pair<string, int>(st.substr(i,j-i),i));
                    if(j == string::npos)break;
                    i=j+1;
                }
                break;
            }
            case CO_output_result_name: result_name = optarg;
                break;
            case CO_prism_path: prismPath = optarg;
                break;

            case CO_const: // const
            {
                string conststr = optarg;
                size_t index, index2;
                index2 = conststr.find('=', 0) + 1;
                for (index = 0; index != string::npos; index2 = conststr.find('=', index) + 1) {
                    string varname = conststr.substr(index, index2 - index - 1);
                    index = conststr.find(',', index2);
                    string varvalue;
                    if (index == string::npos) {
                        varvalue = conststr.substr(index2, conststr.length() - index2);
                    } else {
                        varvalue = conststr.substr(index2, index - index2);
                        index++;
                    }
                    
                    constants[varname] = stod(varvalue);
                }

                break;
            }

            case CO_weight_param: weight_file = optarg;
                break;

            case CO_not_gspn:
            {
                modelType = External;
                break;
            }
                
            case CO_version:
                cout << "Source Version:" << GIT_REVISION << endl;
                cout << BUILD_VERSION << " Build Date:" << __DATE__ " at " << __TIME__ << endl;
                cout << "Compiled with ";
#ifdef CMAKE_VERSION
                cout << "cmake " << CMAKE_VERSION ;
#else
#ifdef XCODE_VERSION
                cout << "Xcode " << XCODE_VERSION ;
#else
                cout << "automake" ;
#endif
#endif
                cout <<" : " << CPP_COMPILER << " " << BOOST_PATH << endl;
                cout << "Default flags: " << gccflags << endl;
                cout << "Library: " << BOOST_LIB << " " << endl;
                cout << "Binary Path: " << Path << endl;
                exit(0);

            default:
                usage();
                exit(EXIT_FAILURE);
        }

    }
    

    //Additionnal parameter handling.

    //MaxRuns =0 mean no run.
    if(MaxRuns==0)tmpStatus= (TmpStat)(tmpStatus & (~TS_RUN));

    if (lightSimulator && (tmpStatus & TS_RUN))StringInSpnLHA=true;
    
    if(interactive)Njob=1;

    //Batch must be smaller than maxRuns
    if (Batch != 0)Batch = min(Batch,MaxRuns);

    if(prismPath.empty())prismPath="prism";

    //If no LHA is required only set the path for the GSPN.
    //if (optind + 1 == argc && (loopLHA > 0.0 || !CSLformula.empty() || !unfold.empty() || lightSimulator)) {
    if (optind + 1 == argc) {
        PathGspn = argv[optind];
        if(generateLHA == NoGen) generateLHA = EmptyLoop;
    } else if (optind + 2 == argc) {
        PathGspn = argv[optind];
        PathLha = argv[optind + 1];
    } else {
        if (optind + 1 > argc) {
            cout << "Model file is required." << endl;
        } else {
            cout << "Unrecognize option:" << argv[optind + 2] << endl;
        }

        usage();
        exit(EXIT_FAILURE);
    }

    if( ! (tmpStatus & TS_GEN)){
        PathLha = tmpPath+"/LHA.cpp";
    }

}


