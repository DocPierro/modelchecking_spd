/*******************************************************************************
 *									                                           *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *			                                    						       *
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
 *******************************************************************************
 */

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <time.h>
//#include <unistd.h>

using namespace std;

#include "generateLHA.hpp"
#include "generateSPN.hpp"
#include "readPlast.hpp"
#include "exPapier.hpp"


void combinePlast(vector<vector<vector<double > > > &OPlast,
				  vector<vector<vector<double > > > &NPlast,
				  vector<double> &Reward, double ron);

void combinePlast(vector<vector<vector<double > > > &OPlast,
				  vector<vector<vector<double > > > &NPlast,
				  vector<double> &Reward, double ron){
	double minreward = 0;
	size_t mini, minj, minx;
	for(size_t i =1; i<OPlast.size(); i++)
		for(size_t x =1; x<OPlast[0].size(); x++)
			for(size_t j=1; j<OPlast[0][0].size(); j++)
				if(NPlast[i][x][j]>0 && ron + NPlast[i][x][j]*Reward[j] < minreward){
					minreward = ron + NPlast[i][x][j]*Reward[j];
					mini = i;
					minx = x;
					minj = j;
				}
	
	cout << "Selected min:"<< mini << ","<< minx << "," << minj << "with min reward of" << minreward << endl;
	
	if(minreward == 0 )exit(EXIT_SUCCESS);
	
	for(size_t i =0; i<OPlast.size(); i++)
		for(size_t x =0; x<OPlast[0].size(); x++)
			for(size_t j =0; j<OPlast[0][0].size(); j++)
				if(i != mini || j != minj || x!= minx)
					NPlast[i][x][j] = OPlast[i][x][j];
	
}

int main(int argc, char** argv) {
	/* 
	 * Most of the parameters are defined here.
	 */
	
    int Memory = 2; // Number of memory step for the strategy
    double H=1000; //Simulation horizon
	bool discounted = false; // Compute steady-state or discounted reward.
    
	double Prec=1e-6; // Threshold to stop the iteration
	int MaxIteration=10; //Max number of iteration
	double MaxReward = 7;

	//This is the command line to invoke cosmos.
	//This one use clang for the compilation of the model
	//If clang is not install command in comment can be used.
	string Cosmoscmd = "Cosmos generated.gspn generated.lha --njob 1 --output-data test --max-run 50000 --njob 12 --batch 100 --count-transition --width 0.01 -v 1  --tmp-status 2";
	
	//string Cosmoscmd = "../bin/Cosmos generated.gspn generated.lha -d test --max-run 50000 --batch 100 --count-transition --width 0.01 -v 1";

	
	
	double ron, rdet;
    string ExpFileName;
    string RewardTrace;
    string st;
    int n,m;
	
    
    vector< vector<double> > M;
    vector< double > RewardArray;
	
	if(argc<3){
		cout << "usage: "<<argv[0]<<" linear <n> <Reward> [strategy.pg]"<<endl;
		
		cout <<"<n> Is the number of sensors in the line it must be odd"<<endl;
		cout <<"If no strategy file are given this program compute iteratively";
		cout <<" by simulation approximated strategy."<< endl;
		cout <<"If a strategy file is given this programm compute an automaton";
		cout <<"Implementing this strategy."<< endl;
		cout <<"The output file are:"<<endl;
		cout <<"\t-generated.gspn\t is a Petri net simulating the drone"<<endl;
		cout <<"\t-generated.lha\t is an automaton implementing the last";
		cout <<"strategy computed by simulation"<<endl;
		cout <<"\t-generatedFromStrat.lha\t is an automaton implementing";
		cout <<" the strategy given as input"<<endl;
		cout <<"\t-linear<n>.txt\tcontain the progression of reward during the iteration";
		
		cout <<endl<<"Cosmos must by in the PATH."<<endl;
		cout <<"A typical invocation of cosmos on the output filed is:";
		cout <<"\tCosmos generated.gspn generated.lha"<<endl;
		
		
		
		exit(EXIT_FAILURE);
	}
	
    st=argv[1];
	if(st=="linear"){
		double p=0.3;
		double q=0.2;   
		
		
		std::istringstream st( argv[2]);
		st >> n;
		
		MaxReward = atoi(argv[3]);
		
		M = generateLinearMatrix(n,p,q);
		RewardArray = generateLinearReward(n,MaxReward);
//		RewardArray = vector<double>(n+1, MaxReward);
		ron=-1;
		rdet=10;
		ExpFileName="linear.txt";
		RewardTrace="linear";
		
    }
    else if(st=="grid"){
		
		
		double u=0.4;
		double r=0.4;
		double d=0.1;
		double l=0.05;
		
		std::istringstream st1( argv[3]);
		st1 >> n;
		std::istringstream st2( argv[4]);
		st2 >> m;
		
		M = generateGridMatrix(n,m,u,r,d,l);
		RewardArray = generateGridReward(n,m);
		ron=-5;
		rdet=100;
		
		ExpFileName="grid.txt";
		RewardTrace="grid";
		
		
    }
    
    else{
		M = generateMatrix(0);
		RewardArray = vector<double>(M.size(),4);
		ExpFileName="default.txt";
		RewardTrace="default";
		ron=-1;
		rdet=4;
		
    }
	
    RewardTrace=RewardTrace+argv[2]+".txt";
    ofstream RewardFile(RewardTrace.c_str(), ios::out | ios::trunc);
    ofstream ExpFile(ExpFileName.c_str(), ios::out | ios::app);
	
	
	// initial iteration
	int N = (int)M.size();
    vector< vector< vector<double> > > Plast(N,vector< vector<double> >(Memory, vector<double>(N,1.0)) );
	
	generateSPNfun(M);
	
	if (argc==5) {
		vector< pair<int, vector<int> > > ImportedStrat(0,make_pair(0,vector<int>(N,0)) );
		readStrat(N, argv[4], ImportedStrat);
		printStrat(ImportedStrat);
		generateLHAfromStrat(ron, N, ImportedStrat, RewardArray, H, M,discounted);
		exit(EXIT_SUCCESS);
	}
	 
	for (int i=0; i<N; i++){
		double out = 0.0;
		for (int j=0; j<N; j++)out += M[i][j];
		for (int j=0; j<N; j++)
			Plast[i][0][j]=M[i][j]/out;
	}
	
	double Reward,OldReward,maxReward=0;
	vector< vector< vector<double> > > OldPlast(N,vector< vector<double> >(Memory, vector<double>(N,1.0)) );
	int It=0;
	time_t start,end,maxRewardtime;
	double RelDiff=100; // actual relative difference = abs((Reward-OldReward)/OldReward);
	
	
	time(&start);
	while(RelDiff>Prec && It<MaxIteration){
		It++;    
		//printPlast(Plast);
		generateLHAfun(ron,N,OldPlast,Plast,RewardArray,H,M,discounted);
		OldPlast = Plast;
		system(Cosmoscmd.c_str());
		OldReward=Reward;
		system("head -n 1 test > test2");
		system("tail -n 1 test >> test2");
		Reward = readPlastfun(N, "test2", Plast,H);
		cout <<"Iteration:" <<It<< " reward: "<< Reward <<" RelDiff: "<<RelDiff<< endl;
		printPlast(Plast);
		
		combinePlast(OldPlast,Plast,RewardArray, ron);
		
		fillMemory(Plast,It+1);
		if(It==1){
			
		}else{
			RelDiff=fabs((Reward-OldReward)/OldReward);
		}
		time(&end);
		if(Reward>maxReward){
			maxReward=Reward;
			maxRewardtime=end;
		}
		RewardFile<<It<<" "<<Reward<< "\t time: "<< 
			difftime(end, start) << "s"<< endl;
	}
    st=argv[1];
	if(st=="linear"){
		ExpFile << "n="<<argv[2]<<" ";
    }
    else if(st=="grid"){
		ExpFile << "n="<<argv[3]<<" "<< "m="<<argv[4]<<" ";
    }
    ExpFile <<Reward<<endl;
    
	RewardFile<< "Max: "<< maxReward<<"\t time: "<< difftime(maxRewardtime,start)<<endl;
    RewardFile.close();
    ExpFile.close();
    return(EXIT_SUCCESS);
}
