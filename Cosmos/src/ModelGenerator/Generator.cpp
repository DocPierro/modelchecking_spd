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
 * file Generator.cpp                                                          *
 * Created by Benoit Barbot on 21/01/2014.                                     *
 *******************************************************************************
 */


#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <exception>

#include "Generator.hpp"
#include "parameters.hpp"
#include "LhaParser/Lha-Reader.hpp"
#include "GspnParser/Gspn-model.hpp"
#include "GspnParser/Gspn-Writer.hpp"
#include "GspnParser/Gspn-Writer-Color.hpp"


using namespace std;

shared_ptr<GspnType> ParseGSPN() {

    // initialize an empty structure for the model.
    Gspn_Reader gReader(P);

    if (P.verbose > 0)cout << "Start Parsing " << P.PathGspn << endl;
    int parseresult;

    try {
        // Check the extension of the model file to call the correct parser
        if (!P.GMLinput
            && P.PathGspn.substr(P.PathGspn.length() - 4, 4) != "grml"
            && P.PathGspn.substr(P.PathGspn.length() - 3, 3) != "gml"
            && P.PathGspn.substr(P.PathGspn.length() - 4, 4) != "gspn"){
            if(P.verbose>0)cerr << "Input file not in GrML try to use convertor."<< endl;
            auto outspt = P.tmpPath + "/generatedspt";
            stringstream cmd;
            cmd << P.Path << "modelConvert --grml ";
            if(P.lightSimulator)cmd << "--light ";
            cmd << P.PathGspn << " " << outspt;
            if (P.verbose > 0)cout << cmd.str() << endl;
            if (system(cmd.str().c_str()) != 0) {
                cerr << "Fail to Convert from input language to GrML!" << endl;
                return nullptr;
            }
            P.PathGspn = outspt+".grml";
        }

        if (P.GMLinput
            || (P.PathGspn.compare(P.PathGspn.length() - 4, 4, "grml") == 0)
            || (P.PathGspn.compare(P.PathGspn.length() - 3, 3, "gml") == 0)) {
            parseresult = gReader.parse_gml_file(P);
        } else {
            parseresult = gReader.parse_file(P.PathGspn);
        }
        P.nbPlace = gReader.spn->pl;


        if(parseresult==1 || !gReader.spn)return nullptr;

        //The following code modify the internal representation of the
        //SPN according to options.

        //When using multimodel with simulink and a gspn add a synchronisation transition to the GSPN
        if(P.modelType == GSPN_Simulink){
            auto &spn = gReader.spn;
            transition trans(spn->transitionStruct.size(), "Synctrans", expr(0.0), true);
            trans.dist.name = "SYNC";
            spn->transitionStruct.push_back(trans);
            spn->TransList.insert(trans.name);
            spn->TransId["Synctrans"]=spn->tr;
            spn->tr++;
            for (size_t t = 0; t < spn->tr-1; t++)
                if(spn->transitionStruct[t].dist.name == "SYNC")
                for(auto outarc= spn->outArcsStruct.lower_bound(make_pair(t, 0));
                    outarc != spn->outArcsStruct.end() && outarc->first.first==t; ++outarc){
                    spn->outArcsStruct.insert(make_pair(make_pair(spn->tr-1, outarc->first.second) , outarc->second));
                }
        }

        //Set the isTraced flag for places, transitions and hybrid var
        if (P.tracedPlace.count("ALL") == 0 && P.tracedPlace.count("ALLCOLOR")==0 ) {
            P.nbPlace = 0;
            for (size_t i = 0; i < gReader.spn->pl; i++) {
                if (P.tracedPlace.count(gReader.spn->placeStruct[i].name)>0) {
                    gReader.spn->placeStruct[i].isTraced = true;
                    P.nbPlace++;
                } else {
                    gReader.spn->placeStruct[i].isTraced = false;
                }
            }
            for (size_t i = 0; i < gReader.spn->tr; i++) {
                if ( P.tracedPlace.count(gReader.spn->transitionStruct[i].name)>0 ) {
                    gReader.spn->transitionStruct[i].isTraced = true;
                } else {
                    gReader.spn->transitionStruct[i].isTraced = false;
                }
            }
            for (auto &v: gReader.spn->hybridVars) {
                if ( P.tracedPlace.count(v.name)>0 ) {
                    v.isTraced = true;
                } else {
                    v.isTraced = false;
                }
            }
        }

        //Apply Law of mass action for MASSACTION distribution:
        for (size_t t = 0; t < gReader.spn->tr; t++) {
            ProbabiliteDistribution *trDistr = &gReader.spn->transitionStruct[t].dist;
            if (trDistr->name.compare("MASSACTION") == 0) {
                gReader.spn->transitionStruct[t].markingDependant = true;
                for (size_t p = 0; p < gReader.spn->pl; p++) {
                    if (!gReader.spn->access(gReader.spn->inArcsStruct, t, p).isEmpty) {
                        expr exponent = gReader.spn->access(gReader.spn->inArcsStruct, t, p).exprVal;
                        /*if (gReader.spn->access(gReader.spn->inArcsStruct, t, p).isMarkDep) {
                         exponent = expr(gReader.spn->access(gReader.spn->inArcsStruct, t, p).stringVal);
                         } else {
                         exponent = expr((int) gReader.spn->access(gReader.spn->inArcsStruct, t, p).intVal);
                         }*/
                        expr pl = expr(PlaceName, gReader.spn->placeStruct[p].name);
                        expr mult = expr(Pow, pl, exponent);
                        expr dist = expr(Times, trDistr->Param[0], mult);

                        trDistr->Param[0] = dist;
                    }
                }
            }
        }

        if(P.is_domain_impl_set){
            //rewrite color variable guard
            for(auto  &tr : gReader.spn->transitionStruct){
                tr.guard.rewrite( [](expr &e){
                    if(e.t == ColorVarName){
                        e.t = UnParsed;
                        e.stringVal = " itVar_" +e.stringVal + " ";
                    }
                });
            }
        }

        //Print the internal representation
        { using namespace text_output;
            if(P.verbose >=3)cout << *(gReader.spn);}

    } catch (GrmlInputException& e) {
        cerr << "The following exception append during model import: " << e.what() << endl;
        return nullptr;
    }

    return gReader.spn;
}

bool ParseLHA(){
    GspnType emptyGSPN;
    emptyGSPN.tr =1;
    transition trans;
    trans.id = 0;
    trans.name = "DummyTrans";
    trans.type = Timed;
    trans.priority = expr(1);
    trans.weight = expr(1.0);
    trans.singleService = true;
    trans.markingDependant = false;
    trans.ageMemory = false;
    trans.nbServers = 1;

    emptyGSPN.transitionStruct.push_back(trans);
    emptyGSPN.TransId["DummyTrans"]=0;
    return ParseLHA(emptyGSPN);
}

bool ParseLHA(GspnType &spn){
    // Intialize an empty structure for the automaton
    Lha_Reader lReader(spn, P);
    auto &A = lReader.MyLha;

    int parseresult;

    //Copy name of transition and place required for synchronization.
    A.TransitionIndex = spn.TransId;
    A.PlaceIndex = spn.PlacesId;
    A.ConfidenceLevel = P.Level;

    if (P.verbose > 0)cout << "Start Parsing " << P.PathLha << endl;

    try {
        switch (P.generateLHA) {
            case CSL_LHA:
            {
                // If the automaton need to be generated from a formula
                // call the generating tool.
                P.PathLha = P.tmpPath + "/generatedlha.lha";
                stringstream cmd;
                cmd << "echo \"" << P.CSLformula << "\" | " << P.Path <<
                "automataGen >" << P.PathLha;
                if (P.verbose > 0)cout << cmd.str() << endl;
                if (system(cmd.str().c_str()) != 0) {
                    cerr << "Fail to Generate the Automaton!" << endl;
                    return false;
                }
                break;
            }
            case TimeLoop:
            case ActionLoop:
                generateLoopLHA(spn);
                break;
            case SamplingLoop:
                generateSamplingLHA(spn);
                break;
            case EmptyLoop:
                generateEmptyLHA();
                break;
            case NoGen:
                break;
        }


        //check the type of the LHA file
        //First check if it is not C++ code
        if (P.PathLha.compare(P.PathLha.length() - 3, 3, "cpp") != 0) {

            if (P.PathLha.compare(P.PathLha.length() - 4, 4, "grml") == 0) {
                //The LHA is in the GRML file format
                parseresult = lReader.parse_gml_file(P);
                if(! A.isDeterministic )P.lhaType= NOT_DET;
            } else {
                //The LHA is in the LHA file format
                parseresult = lReader.parse_file(P);
                if(! A.isDeterministic )P.lhaType= NOT_DET;
            }

            //Add external HASL formula to the lha.
            if (P.externalHASL.compare("") != 0)
                lReader.parse(P.externalHASL);

            //Set the isTraced flag for variables
            if (P.tracedPlace.count("ALL")==0 && P.tracedPlace.count("ALLCOLOR")==0) {
                for (size_t i = 0; i < A.NbVar; i++) {
                    if ( P.tracedPlace.count(A.Vars.label[i])>0){
                        A.Vars.isTraced[i] = true;
                    } else {
                        A.Vars.isTraced[i] = false;
                    }
                }
            }

            //If everythink work correctly, copy the HASL formula and generate the code
            if (!parseresult && A.NbLoc > 0) {
                P.HaslFormulasname = A.HASLname;
                P.HaslFormulas = vector<HaslFormulasTop*>(A.HASLtop);
                P.nbAlgebraic = A.Algebraic.size();
                P.nbQualitatif = A.FinalStateCond.size();

                //If the countTrans option is set then add HASL formula counting the occurance of each transition of the LHA.
                if (P.CountTrans) {
                    for (size_t tr = 0; tr < A.Edge.size(); tr++) {
                        P.nbAlgebraic++;
                        std::stringstream transname;
                        transname << "P_";
                        transname << A.LocLabel[A.Edge[tr].Source];
                        transname << "->";
                        transname << A.LocLabel[A.Edge[tr].Target];
                        P.HaslFormulasname.push_back(transname.str());
                        P.HaslFormulas.push_back(new HaslFormulasTop(A.Algebraic.size() + tr));
                    }
                }

                //some cleaning:
                A.SimplyUsedLinearForm = vector<bool>(A.LinearForm.size(),true);
                for( size_t i = 0; i< A.LhaFuncArg.size();++i)
                    if(A.LhaFuncType[i]!="Last")
                        A.SimplyUsedLinearForm[A.LhaFuncArg[i]] = false;


                //Generate the code for the LHA
                lReader.WriteFile(P);
                lReader.writeDotFile(P.tmpPath + "/templateLHA.dot");

            } else {
                return false;
            }

            //If the LHA is already C++ code just copy it to temporary
            //and add external HASL formula
        } else if (P.PathLha.compare(P.PathLha.length() - 3, 3, "cpp") == 0) {
            //The code for the LHA is provided by the user
            A.ConfidenceLevel = P.Level;
            //Add external HASL formula
            if (P.externalHASL.compare("") == 0) {
                P.HaslFormulasname.push_back("preComputedLHA");
                HaslFormulasTop *ht = new HaslFormulasTop((size_t) 0);
                P.HaslFormulas.push_back(ht);
                P.nbAlgebraic = 1;
            } else {
                parseresult = lReader.parse(P.externalHASL);
                if (!parseresult) {
                    P.HaslFormulasname = A.HASLname;
                    P.HaslFormulas = vector<HaslFormulasTop*>(A.HASLtop);
                    P.nbAlgebraic = A.Algebraic.size();
                } else
                    cerr << "Fail to parse extra Hasl Formula" << endl;
            }

            //Copy the code into the temporary directory
            if( P.PathLha != P.tmpPath + "/LHA.cpp"){
                string cmd = "cp " + P.PathLha + " " + P.tmpPath + "/LHA.cpp";
                if (P.verbose > 2)cout << cmd << endl;
                if (system(cmd.c_str()) != 0) {
                    cerr << "Fail to copy LHA to temporary" << endl;
                    return false;
                }
            }
        }
    } catch (GrmlInputException& e) {
        cerr << "The following exception append during LHA import: " << e.what() << endl;
        return false;
    }

    string cmd;

    if (P.RareEvent) {
        //If rareevent handling is require copy the lumping function and table of value to the temporary directory
        if (P.BoundedRE == 0)cmd = "cp muFile " + P.tmpPath + "/muFile";
        else cmd = "cp matrixFile " + P.tmpPath + "/matrixFile";
        if (system(cmd.c_str())) return false;
        cmd = "cp lumpingfun.cpp " + P.tmpPath + "/lumpingfun.cpp";
        if (system(cmd.c_str())) return false;

        //Rewrite part of probabilistic operator to apply Rare event handling
        //First case for Continuous bounded rare event.
        if (P.BoundedContinuous) {
            for (vector<HaslFormulasTop*>::iterator it = P.HaslFormulas.begin();
                 it != P.HaslFormulas.end(); ++it)
                if ((*it)->TypeOp == EXPECTANCY) {
                    (*it)->TypeOp = RE_Continuous;
                    (*it)->Value = P.continuousStep;
                    (*it)->Value2 = P.epsilon;

                }
        } else { // Second case Unbounded rare event.
            vector<HaslFormulasTop*> tmpRE;
            vector<string> tmpREName;
            for (vector<HaslFormulasTop*>::iterator it = P.HaslFormulas.begin();
                 it != P.HaslFormulas.end(); ++it)
                if ((*it)->TypeOp == EXPECTANCY) {
                    (*it)->TypeOp = RE_AVG;
                    HaslFormulasTop *HaslCopy = new HaslFormulasTop(**it);
                    HaslCopy->TypeOp = RE_Likelyhood;
                    tmpRE.push_back(HaslCopy);
                    tmpREName.push_back("Likelyhood_" + P.HaslFormulasname[it - P.HaslFormulas.begin() ]);
                }
            for (vector<HaslFormulasTop*>::iterator it = tmpRE.begin();
                 it != tmpRE.end(); ++it) {
                P.HaslFormulas.push_back(*it);
                P.HaslFormulasname.push_back(tmpREName[it - tmpRE.begin()]);
            }
        }
    }

    return true;

}

void generateMain() {

    string loc;

    loc = P.tmpPath + "/main.cpp";
    ofstream mF(loc.c_str(), ios::out | ios::trunc);

    mF << "#include \"BatchR.hpp\"" << endl;
    mF << "#include \"clientsim.hpp\"" << endl;
    /*mF << "#include \"Simulator.hpp\"" << endl;
     mF << "#include \"SimulatorRE.hpp\"" << endl;
     mF << "#include \"SimulatorBoundedRE.hpp\"" << endl;
     mF << "#include \"SimulatorContinuousBounded.hpp\"" << endl;*/
    mF << "#include <sys/types.h>" << endl;
    mF << "#include <unistd.h>" << endl;
    mF << "#include <signal.h>" << endl;
    if( P.modelType == GSPN_Simulink){
        mF << "#include \"MultiModel.hpp\"" << endl;
        mF << "#include \"SKModel.hpp\"" << endl;
        mF << "#include \"multimodel.cpp\"" << endl;
    }

    mF << "int main(int argc, char** argv) {" << endl;
    mF << "    signal(SIGHUP, signalHandler);" << endl;
    mF << "    signal(SIGINT, signalHandler);" << endl;
    mF << "    {const string tmppath(argv[1]);" << endl;
    mF << "    ifstream ifs(tmppath+\"/parameters.txt\");" << endl;
    mF << "    readParameters(ifs);}" << endl;
    if( P.computeStateSpace>0){
        mF << "stateSpace states;" << endl;
        mF << "states.exploreStateSpace();" << endl;
        mF << "states.buildTransitionMatrix();" << endl;
        if(P.computeStateSpace==1){
            mF << "states.outputPrism();" << endl;
            mF << "states.launchPrism(\""<< P.prismPath <<"\");" << endl;
            mF << "states.importPrism();" << endl;
            mF << "states.outputVect();" << endl;
            mF << "states.outputTmpLumpingFun();" << endl;
            mF << "double prResult = states.returnPrismResult();" << endl;
            mF << "BatchR dummR(1,0);" << endl;
            mF << "SimOutput sd;" << endl;
            mF << "sd.accept=true;" << endl;
            mF << "sd.quantR.push_back(prResult);" << endl;
            mF << "dummR.addSim(sd);" << endl;
        } else{
            //states.uniformizeMatrix();
            mF << "states.outputMat();" << endl;
            mF << "states.outputTmpLumpingFun();" << endl;
            mF << "BatchR dummR(0,0);" << endl;
        }
        mF << "dummR.outputR(cout);" << endl;
        mF << "cerr << \"Finish Exporting\" << endl;" << endl;
        mF << "exit(EXIT_SUCCESS);" << endl;
        mF << "}" << endl;
        return;
    }

    // Instantiate EventQueue
    const auto eqt = (P.is_domain_impl_set ? "EventsQueueSet" :"EventsQueue");

    // Instantiate DEDS
    if (P.BoundedRE > 0 || P.BoundedContinuous) {
        mF << "    SPN_BoundedRE N(false);" << endl;
    } else if (P.RareEvent) {
        mF << "    SPN_RE N("<< P.DoubleIS <<");" << endl;
    } else {
        mF << "    SPN_orig<" << eqt << "> N(0);" << endl;
        mF << "    fstream weight_file;" << endl;
        mF << "    weight_file.open (P.weight_file,ios::in);" << endl;
        mF << "    if (weight_file.is_open()) {" << endl;
        mF << "        string line;" << endl;
        mF << "        while (getline(weight_file, line)) {" << endl;
        mF << "            string name = line.substr(0,line.find(':'));" << endl;
        mF << "            double val = stod(line.substr(line.find(\":\")+1,line.size()));" << endl;
        mF << "            N.overrideParameter(name,val);" << endl;
        mF << "        }" << endl;
        mF << "    }" << endl;
    }

    if(P.modelType == GSPN_Simulink){
        mF << "    SKModel<" << eqt <<"> N2(N.tr);" << endl;
        mF << "    MultiModel<"<< eqt <<",typeof N, typeof N2> mm(N,N2);" << endl;
    }
    const auto model = ((P.modelType == GSPN_Simulink)? "mm" : "N");

    // Instantiade LHA
    if( P.lhaType == DET){
        mF << "    LHA_orig<typename decltype(N)::stateType> A;"<< endl;
    }else{
        mF << "    NLHA<typename decltype(N)::stateType> A;"<< endl;
    }

    // Instantiate Simulator
    if (P.BoundedContinuous){
        mF << "    SimulatorContinuousBounded<SPN_BoundedRE> sim(N,A,"<<
           P.BoundedRE << ", "<< P.epsilon << ", " << P.continuousStep << ");" << endl;
        mF << "    sim.initVectCo("<< P.horizon <<");" << endl;
    }else if (P.BoundedRE > 0) {
        mF << "    SimulatorBoundedRE<SPN_BoundedRE> sim(N,A,"<< P.BoundedRE <<");" << endl;
        mF << "    sim.initVect("<< (int)P.horizon <<");" << endl;
    } else if (P.RareEvent) {
        mF << "    SimulatorRE<SPN_RE> sim(N,A);" << endl;
        mF << "    sim.initVect();" << endl;
    } else {
        mF << "    Simulator<"<< eqt << ",typeof "<< model << "> sim("<< model <<",A);" << endl;
    }


    if( !P.dataraw.empty()) mF << "    sim.logValue(\"" << P.dataraw << "\");" <<endl;
    if( !P.datatrace.empty()){
        mF << "    sim.logTrace(\"" << P.datatrace << "\","<< P.sampleResol << ");" <<endl;
        mF << "    bool singleBatch = true;"<< endl;
    } else mF << "    bool singleBatch = false;"<< endl;
    if( !P.dotfile.empty()) mF << "    sim.dotFile = \"" << P.dotfile << "\";" << endl;

    mF << "    sim.SetBatchSize(" << P.Batch << ");" << endl;

    mF << "    setSimulator(sim,argc,argv,P);" << endl;

    mF << "    if((P.verbose>=4)|singleBatch)sim.RunBatch();" << endl;
    mF << "    else while( !cin.eof() && P.graceFullEnding <= 1 ){" << endl;
    mF << "        BatchR batchResult = sim.RunBatch(); //simulate a batch of trajectory" << endl;
    mF << "        batchResult.outputR(cout); // output the result on the standart output" << endl;
    mF << "    }" << endl;
    mF << "    return (EXIT_SUCCESS);" << endl;
    mF << "}" << endl << endl;

    mF.close();

}


bool compileSource(const std::vector<std::string> &sources){
    string bcmd = P.gcccmd + " " + P.boostpath + " " + P.gccflags;

    bool first = true;
    string cmd;
    for(let file : sources){
        if(first){ first = false; cmd += " ";
        } else {
            cmd += "&";
        }

        cmd += "(" + bcmd + " -c -I" + P.Path + "../include " + (P.modelType== External || P.modelType==GSPN_Simulink ? (P.boostpath +" -I./ ") : "");
        cmd += " -o " + P.tmpPath + "/" + file +".o " + P.tmpPath + "/" + file +".cpp" + " )\\\n";
    }
    cmd += " & wait";
    if (P.verbose > 2)cout << cmd << endl;
    if (system(cmd.c_str())) return false;
    return true;
}


bool build() {
    /*const bool generateMain =
            P.RareEvent || P.computeStateSpace >0
            || P.is_domain_impl_set
            || P.modelType == External || P.modelType == GSPN_Simulink
            || P.lhaType == NOT_DET  || P.;*/

    const bool generateMain = true;

    string bcmd = P.gcccmd + " " + P.gccflags;

    if (P.verbose > 0) {
        cout << "Parsing OK.\n" << endl;
        cout << "Start building ... " << endl;
    }

    auto sources = vector<string>();

    //Compile the SPN
    if(P.modelType==GSPN || P.modelType == GSPN_Simulink )sources.push_back("spn");

    //Compile the LHA
    if(!P.lightSimulator)sources.push_back("LHA");

    //Compile main
    if(generateMain)sources.push_back("main");

    if(P.modelType == GSPN_Simulink){
        if(system(("cp " + P.tmpPath + "/../SKModel.cpp "+ P.tmpPath+"/SKModel.cpp").c_str()) !=0){
            cerr << "Fail to copy files to temporary directory" << endl;
            exit(EXIT_FAILURE);
        }

        sources.push_back("SKModel");
        //sources.push_back("multimodel");
    }

    compileSource(sources);

    //Link SPN and LHA with the library
    string cmd = bcmd + " -o " + P.tmpPath + "/ClientSim ";
    for(let file : sources)cmd += P.tmpPath + "/" + file + ".o ";
    if(P.lightSimulator){
        cmd += P.Path + "../lib/libClientSimLight.a ";
    } else {

        if(P.modelType == External || P.modelType == GSPN_Simulink)cmd += P.Path + "../lib/libClientSimSK.a ";
        if(generateMain){
            cmd += P.Path + "../lib/libClientSim.a ";
        } else {
            cmd += P.Path + "../lib/libClientSimMain.a ";
        }
        cmd += P.Path + "../lib/libClientSimBase.a ";
    };
    cmd += " " + P.boostlib + " ";


    if (P.verbose > 2)cout << cmd << endl;
    if (system(cmd.c_str())) return false;

    if (P.verbose > 0)cout << "Building OK.\n" << endl;

    return true;
}


void generateSamplingLHA(GspnType &spn) {
    //bool allcolor = false;
    //if (P.tracedPlace == "ALLCOLOR")allcolor= true;
    P.sampleResol = P.loopTransientLHA;
    size_t nbsample = static_cast<size_t> (ceil((P.loopLHA / P.sampleResol)));

    P.PathLha = P.tmpPath + "/samplelha.lha";
    ofstream lhastr(P.PathLha.c_str(), ios::out | ios::trunc);

    //lhastr << "NbVariables = "<<1+gReader.spn->tr + P.nbPlace <<";\nNbLocations = 3;\n";
    lhastr << "const double T=" << P.loopLHA << ";\n";
    lhastr << "const double invT=" << P.sampleResol << ";\n";
    lhastr << "const double invT2=" << 1 / P.sampleResol << ";\n";

    lhastr << "VariablesList = {time,time2, DISC counter";
    for (const auto &itt : spn.placeStruct)if (itt.isTraced) {
        lhastr << ", PLVARACC_" << itt.name;
        lhastr << ", DISC PLVAR_" << itt.name << "[" << nbsample << "]";
        //if(allcolor && itt.colorDom != UNCOLORED_DOMAIN){
        //	gReader.iterateDom("", "_", "","","","" ,gReader.spn->colDoms[itt.colorDom], 0, [&] (const string &str,const string&){
        //		lhastr << ", PLVAR_" + itt.name + str;
        //	});

        //}
    }
    lhastr << "} ;\nLocationsList = {l0,";
    //for (size_t i = 0; i < nbsample ; ++i ) lhastr << "l" << i << ", ";
    lhastr << "l2 };\n";

    for (const auto &itt : spn.placeStruct) {
        if (itt.isTraced)for (size_t i = 0; i < nbsample; ++i) {
            lhastr << "MeanToken_" << itt.name << "$GRAPH$" << (double) i * P.sampleResol << "$" << (double) (i + 1) * P.sampleResol << "$= AVG(Last( PLVAR_" << itt.name << "[" << i << "]));\n";
            /*if(allcolor && itt.colorDom != UNCOLORED_DOMAIN){
             gReader.iterateDom("", "_", "","","","" ,gReader.spn->colDoms[itt.colorDom], 0, [&] (const string &str,const string&){
             lhastr << "MeanToken_" << itt.name << str << "= AVG(Last( PLVAR_" << itt.name<< str <<"));\n";
             });
             }*/
        }
    }
    lhastr << P.externalHASL << endl;
    lhastr << "InitialLocations={l0};\nFinalLocations={l2};\n";
    lhastr << "Locations={" << endl;
    //for (size_t i = 0; i < nbsample ; ++i ) {
    lhastr << "(l" << 0 << ", TRUE , (time:1,time2:1";
    for (const auto &itt : spn.placeStruct)
        if (itt.isTraced) {
            lhastr << ", PLVARACC_" << itt.name << ": " << itt.name << "* invT2 ";
            /*if(allcolor && itt.colorDom != UNCOLORED_DOMAIN){
             gReader.iterateDom("", "_", "","","","," ,gReader.spn->colDoms[itt.colorDom], 0, [&] (const string &str,const string &str2){
             lhastr << ", PLVAR_" << itt.name << str << ": " << itt.name << "[" << str2 <<"]* invT ";
             });
             }*/
        }
    lhastr << "));" << endl;
    //}
    lhastr << "(l2, TRUE , (time:1,time2:1));};\n";
    lhastr << "Edges={";
    //for (size_t i = 0; i < nbsample ; ++i ) {
    lhastr << "((l0,l0),ALL,time<= invT ,#);";
    lhastr << "((l0,l0),#,time=invT ,{time=0,counter=counter+1";
    for (const auto &itt : spn.placeStruct)if (itt.isTraced) {
        lhastr << ", PLVARACC_" << itt.name << " = 0.0 ";
        lhastr << ", PLVAR_" << itt.name << "[" << "counter" << "]=PLVARACC_" << itt.name;
    }
    lhastr << "});" << endl;
    lhastr << "((l0,l2),# , time2 >= " << P.loopLHA + P.sampleResol * 0.001 << ",#);";

    //}

    lhastr << "};";
    lhastr.close();

}

void generateEmptyLHA(){
    P.PathLha = P.tmpPath + "/emptylha.lha";
    ofstream lhastr(P.PathLha.c_str(), ios::out | ios::trunc);

    lhastr << "VariablesList = {time};" <<endl;
    lhastr << "LocationsList = {l0, l1};" <<endl;
    lhastr << "PROB;" << endl;
    lhastr << "InitialLocations={l0};" << endl;
    lhastr << "FinalLocations={l1};" << endl;
    lhastr << "Locations={ (l0, TRUE); (l1, TRUE); };" << endl;
    lhastr << "Edges={ ((l0,l0),ALL, # ,#);} ;" << endl;

    lhastr.close();

}

void generateLoopLHA(GspnType &spn) {
    //If the automaton need to be generated to mesure simple perfomance indices generate it
    //An automaton is produce with two loop the first make time elapse until transient time
    //elapse and then compute the mean number of token in each place and the throughput
    //of each transition
    bool allcolor = false;
    if (P.tracedPlace.count("ALLCOLOR")>0.0)allcolor = true;


    P.PathLha = P.tmpPath + "/looplha.lha";
    ofstream lhastr(P.PathLha.c_str(), ios::out | ios::trunc);

    if(P.generateLHA ==TimeLoop){
        lhastr << "const double T=" << P.loopLHA << ";\n";
        lhastr << "const double invT=" << 1 / P.loopLHA << ";\n";
    } else {
        lhastr << "const double TDiscr=" << P.loopLHA << ";\n";
        lhastr << "const double invT= 1.0 ;\n";
    }
    lhastr << "const double Ttrans=" << P.loopTransientLHA << ";\n";
    lhastr << "VariablesList = {time,DISC countT";
    for (let itt : spn.transitionStruct)
        if (itt.isTraced)lhastr << ", " << itt.name;

    for (let itt : spn.placeStruct) {
        if (itt.isTraced) {
            lhastr << ", PLVAR_" << itt.name;
            if (allcolor && itt.colorDom != UNCOLORED_DOMAIN) {
                spn.iterateDom("", "_", "", "", "", "", spn.colDoms[itt.colorDom], 0, [&] (const string &str, const string&) {
                    lhastr << ", PLVAR_" + itt.name + str;
                });
            }
        }
    }
    lhastr << "} ;\nLocationsList = {l0, l1,l2};\n";

    auto nbHASL = 0;
    for (let itt : spn.transitionStruct)
        if (itt.isTraced){
            nbHASL++;
            lhastr << "Throughput_" << itt.name << "= AVG(Last(" << itt.name << "));\n";
        }
    for (let itt : spn.placeStruct)
        if (itt.isTraced) {
            nbHASL++;
            lhastr << "MeanToken_" << itt.name << "= AVG(Last( PLVAR_" << itt.name << "));\n";
            if (allcolor && itt.colorDom != UNCOLORED_DOMAIN) {
                spn.iterateDom("", "_", "", "", "", "", spn.colDoms[itt.colorDom], 0, [&] (const string &str, const string&) {
                    lhastr << "MeanToken_" << itt.name << str << "= AVG(Last( PLVAR_" << itt.name << str << "));\n";
                });
            }
        }
    lhastr << P.externalHASL << endl;
    if(P.externalHASL.empty() && nbHASL==0)
        lhastr << "PROB;" << endl;

    const auto stopcond = (P.generateLHA == TimeLoop ? "time<=T," : "countT<=TDiscr -1,");

    lhastr << "InitialLocations={l0};\nFinalLocations={l2};\n";
    lhastr << "Locations={\n(l0, TRUE, (time:1));\n(l1, TRUE, (time:1 ";
    for (let itt : spn.placeStruct)
        if (itt.isTraced) {
            lhastr << ", PLVAR_" << itt.name << ": " << itt.name << "* invT ";
            if (allcolor && itt.colorDom != UNCOLORED_DOMAIN) {
                spn.iterateDom("", "_", "", "", "", ",", spn.colDoms[itt.colorDom], 0, [&] (const string &str, const string & str2) {
                    lhastr << ", PLVAR_" << itt.name << str << ": " << itt.name << "[" << str2 << "]* invT ";
                });
            }
        }

    lhastr << "));\n(l2, TRUE);\n};\n";
    lhastr << "Edges={\n((l0,l0),ALL,time<= Ttrans ,#);\n((l0,l1),#,time=Ttrans ,{time=0});\n";
    size_t nbplntr = 0;
    for (let itt : spn.transitionStruct) {
        if (itt.isTraced) {
            lhastr << "((l1,l1),{" << itt.name << "}," << stopcond;
            if(P.loopLHA>0.0){
                lhastr << "{" << itt.name << " = " << itt.name << " + invT, countT = countT+1 });\n";
            }else{
                lhastr << "{" << itt.name << " = " << itt.name << " + 1, countT = countT+1 });\n";
            }
        } else nbplntr++;
    }
    if (nbplntr > 0) {
        lhastr << "((l1,l1),{";
        nbplntr = 0;
        for (let itt : spn.transitionStruct)
            if (!itt.isTraced) {
                if (nbplntr > 0)lhastr << ",";
                lhastr << itt.name;
                nbplntr++;
            }
        lhastr << "}," << stopcond;
        lhastr << " # );" << endl;
    }
    if (P.generateLHA == TimeLoop){
        lhastr << "((l1,l2),#,time=T ,#);\n};";
    }else lhastr << "((l1,l2),ALL,countT=TDiscr ,#);\n};";
    lhastr.close();
}
