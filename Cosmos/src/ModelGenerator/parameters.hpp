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
 * file parameters.hpp                                                         *
 * Created by Benoit Barbot on 08/02/12.                                       *
 *******************************************************************************
 */

#ifndef Cosmos_parameters_h
#define Cosmos_parameters_h

#include <chrono>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "HaslFormula.hpp"
#include "SimpleSerializer.hpp"
#include "expressionStruct.hpp"

#define let const auto &

enum LHAGenType : int {
  NoGen,
  EmptyLoop,
  TimeLoop,
  ActionLoop,
  SamplingLoop,
  CSL_LHA
};

enum Poption {
  CO_level,
  CO_width,
  CO_batch,
  CO_max_run,
  CO_seed,
  CO_VDC,
  CO_RandomGen,
  CO_local_test,
  CO_sampling,
  CO_loop,
  CO_transient,
  CO_formula,
  CO_chernoff,
  CO_relative,
  CO_const,
  CO_weight_param,
  CO_rareevent,
  CO_boundedcountiniouceRE,
  CO_boundedRE,
  CO_step_continuous,
  CO_epsilon,
  CO_set_Horizon,
  CO_state_space,
  CO_lump,
  CO_prism,
  CO_normalize_IS,
  CO_grml_input,
  CO_alligator_mode,
  CO_gui_greatSPN_mode,
  CO_unfold,
  CO_output_model,
  CO_HASL_formula,
  CO_njob,
  CO_gppcmd,
  CO_gppflags,
  CO_light_simulator,
  CO_verbose,
  CO_interactive,
  CO_update_time,
  CO_output_data,
  CO_output_raw,
  CO_output_trace,
  CO_output_graph,
  CO_output_dot,
  CO_gnuplot_driver,
  CO_trace_pt,
  CO_output_result_name,
  CO_help,
  CO_count_transition,
  CO_debug_string,
  CO_tmp_path,
  CO_tmp_status,
  CO_bin_path,
  CO_prism_path,
  CO_magic_values,
  CO_version,
  CO_reuse,
  CO_use_magic_print,
  CO_domain_impl,
  CO_force_TTY,
  CO_not_gspn,
  CO_gracefull_ending,
};

enum RandomGenerator : int { MT19937, VDC, Kronecker };

enum TmpStat : int {
  TS_GEN = 0x01,
  TS_BUILD = 0x02,
  TS_RUN = 0x04,
  TS_DESTROY = 0x08
};

enum ModelType : int { GSPN, GSPN_Simulink, External, CTMC };

enum LHAType : int { DET, NOT_DET };

struct parameters {
  std::string commandLine;
  int verbose;
  bool interactive;
  std::chrono::milliseconds updatetime;
  unsigned long seed;
  int randomGen;
  int Njob;

  double epsilon;
  int continuousStep;
  double Level;
  double Width;
  unsigned long int Batch;
  unsigned long int MaxRuns;
  bool sequential;
  bool relative;
  bool chernoff;

  std::string comp_uuid;
  std::string tmpPath;
  TmpStat tmpStatus; // 0 create and destroy tmp (default),
                     // 1 do not build but destroy,
                     // 2 do not destroy,
                     // 3 do not build nor destoy
                     // 4 do not generate but build and do not destroy
  bool reuse;

  ModelType modelType;
  LHAType lhaType;

  std::string Path;
  std::string PathGspn;
  std::string PathLha;
  std::string result_name;
  std::map<std::string, expr> constants;
  std::string weight_file;

  LHAGenType generateLHA;
  double loopLHA;
  double loopTransientLHA;
  std::string CSLformula;
  std::string externalHASL;
  bool localTesting;
  bool RareEvent;
  bool DoubleIS;
  int BoundedRE;
  double horizon;
  bool BoundedContinuous;
  bool CountTrans;
  bool StringInSpnLHA;

  bool GMLinput;
  int computeStateSpace;
  bool lumpStateSpace;

  bool alligatorMode;
  bool guiGreatSpnMode;
  std::string unfold;
  std::string outputModel;
  bool isTTY;
  int terminalWidth;
  int graceFullEnding;

  std::string gcccmd;
  std::string boostpath;
  std::string gccflags;
  std::string boostlib;
  bool lightSimulator;

  // void parseCommandLine2(int argc, char** argv);
  std::string prismPath;
  std::string dataoutput;
  std::string dataraw;
  std::string datatrace;
  double sampleResol;
  std::string dataPDFCDF;
  std::string gnuplotDriver;
  std::map<std::string, int> tracedPlace;
  std::string dotfile;
  std::string magic_values;
  bool use_magic_print;
  bool is_domain_impl_set;

  std::vector<HaslFormulasTop *> HaslFormulas;
  std::vector<std::string> HaslFormulasname;
  size_t nbAlgebraic;
  size_t nbQualitatif;
  size_t nbPlace;

  parameters();
  void View();
  void usage();
  void parseCommandLine(int argc, char **argv);
  Poption parsersingleOpt(int i) const;

public:
  template <class Archive> void serialize(Archive &ar) {
    ar &verbose &interactive &seed &randomGen &Njob;
    ar &epsilon &continuousStep &Level &Width &Batch &MaxRuns &sequential
        &relative &chernoff;

    ar &comp_uuid &tmpPath &(int &)tmpStatus &reuse &graceFullEnding;
    ar &(int &)modelType &(int &)lhaType;

    ar &Path &PathGspn &PathLha &weight_file; //&constant

    ar &(int &)generateLHA &loopLHA &loopTransientLHA &CSLformula &externalHASL;
    ar &localTesting &RareEvent &DoubleIS &BoundedRE &horizon
        &BoundedContinuous;
    ar &CountTrans &StringInSpnLHA &GMLinput &computeStateSpace &lumpStateSpace;

    ar &prismPath &dataoutput &dataraw &datatrace &sampleResol &dataPDFCDF
        &gnuplotDriver;
    ar &tracedPlace &dotfile &magic_values &use_magic_print &is_domain_impl_set &result_name;
  }
};

/**
 * Retrive the real absolute path of the executable of Cosmos
 * This is usefull for finding the library containing all the
 * code for the simulator.
 * Thoses functions fill the variable P.Path
 * This code is system dependant.
 * @param P is a structure of parameters
 */
void findBinaryPath(parameters &P);

extern parameters P;

#endif
