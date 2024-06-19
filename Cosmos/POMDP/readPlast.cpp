//
//  readPlast.cpp
//  Cosmos
//
//  Created by Benoit Barbot on 06/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "readPlast.hpp"


double readPlastfun(int N,string PlastPath, vector< vector< vector<double> > >&Plast,double H){
    ifstream PlastFile(PlastPath.c_str(), ios::in);
    
    int dumbint;
    double dumbdouble;
    string line;
    
    if (PlastFile.is_open()) {
    
        /*PlastFile>> line;
        cout << line << endl;*/
        
		vector<pair<pair<int, int>, int> > bij;
		
		//cout << "Parse first line" << endl;
		getline(PlastFile,line);
		unsigned long pos = 0;
		int nbfield=0;
		while(pos != string::npos){
			pos = line.find("Mean[", pos+1);
			unsigned long pos2 = line.find("]", pos);
			if(pos != string::npos){
				string transname = line.substr(pos+5,pos2-pos-5);
				//cout<< transname;
				unsigned long posXlast = transname.find("_", 4);
				unsigned long posArrow = transname.find("->l", posXlast+1);
				unsigned long posXlast2 = transname.find("_",posArrow+4);
				
				int source = atoi(transname.substr(4,posXlast-4).c_str());
				int Xlast = atoi(transname.substr(posXlast+1,posArrow-posXlast-1).c_str());
				
				//cout << "\t nbfield: "<<nbfield;
				//cout << "\t source: "<< source;
				//cout << "\t Xlast: "<< Xlast;
				if (posXlast2!= string::npos) {
					int cible = atoi(transname.substr(posArrow+4,posXlast2-posArrow-4).c_str());
					int Xlast2= atoi(transname.substr(posXlast2+1,transname.length()-1-posXlast2).c_str());
					
					//cout << "\t cible: "<< cible;
					//cout << "\t Xlast2: "<< Xlast2;
					
					if (Xlast2==0) {
						bij.push_back(make_pair(make_pair(source, Xlast),cible));
					}else{
						bij.push_back(make_pair(make_pair(source, Xlast),0));
					}
				}else{
					bij.push_back(make_pair(make_pair(-1,0),0));
				}

				//cout << endl;
				nbfield++;
			}
			//if(line.find("\n",0)>=0)rendofline=true;
		}
		
		PlastFile >> dumbint >> dumbint;
		
        double reward;
		double conflow,confup;
        PlastFile >> reward;
        PlastFile >> dumbdouble >> conflow >> confup;
		
		int nbfield2 =1;
		
		for (int i=0; i < N ; i++)
            for (int Xlast=0; Xlast < Plast[i].size(); Xlast++) 
                for (int j=0; j<N; j++)
					Plast[i][Xlast][j]=0;		
		
		while (nbfield2 < nbfield) {
			double val;
			PlastFile >> val;
			PlastFile >> dumbdouble >> dumbdouble >> dumbdouble; 
			
			if(bij[nbfield2].first.first>=0){
				Plast[bij[nbfield2].first.first][bij[nbfield2].first.second][bij[nbfield2].second] += val;
				//cout << "lp"<<bij[nbfield2].first.first<< "_" << bij[nbfield2].first.second << "->" << bij[nbfield2].second << "\t "<< val<<endl;
			}
			nbfield2++;
		}
		
        for (int i=0; i < N ; i++) {
            for (int Xlast=0; Xlast < Plast[i].size(); Xlast++) {
                double outi = 0.0;
                for (int j=0; j<N; j++) {
                    outi += Plast[i][Xlast][j];
                }
                for (int j=0; j<N; j++) {
                    if(outi>0)
                        Plast[i][Xlast][j] /= outi;
                    //cout << "Plast("<<i<<","<< Xlast << ","<< j<<") = "<< Plast[i][Xlast][j]<< endl;
                }
            }
            
        }
		cout<< "Reward: ["<< conflow << " ; "<< confup << "], width:"<< (confup-conflow)<< endl;
		
		return reward;
        
    }else{
        cout << "Fail to open Plast file:"<< PlastPath << endl;
        exit(EXIT_FAILURE);
    }
    
}

void printPlast(  vector< vector< vector<double> > >& Plast ){
	for (int i= 0; i<Plast.size(); i++){ 
		cout <<setw(4)<< i<< "["<< setprecision(6);
		for (int j=0; j< Plast[0][0].size(); j++){
			cout << "->"<<setw(3)<<j<<"[";
			for (int Xlast=0; Xlast<Plast[0].size(); Xlast++){
				cout<< setw(8)<< Plast[i][Xlast][j]<<" ";
			}
			cout << "]";
		}
		cout << "]"<< endl;
	}
}

void fillMemory( vector< vector< vector<double> > >& Plast, int l){
	for (int i= 0; i<Plast.size(); i++) 
		for (int Xlast=l; Xlast<Plast[0].size(); Xlast++) 
			for (int j=0; j< Plast[0][0].size(); j++) 
				Plast[i][Xlast][j] = 1.0;
	
	
}


void readStrat(int N, string stratPath, vector< pair<int, vector<int> > >& strat){
	ifstream stratFile(stratPath.c_str(), ios::in);
	cout << "Opening:"<< stratPath<<endl;
	if (stratFile.is_open()) {
		while (stratFile.good()) {
			
			int source,obs;
			stratFile >> source >> obs;
			if(stratFile.eof())break;
			cout << source<< " "<<obs<<" ";
			strat.push_back(make_pair(obs, vector<int>(N,0)));
			for (int i=0; i<N; i++) {
				string valuestr;
				stratFile >> valuestr;
				if(valuestr.compare("-")==0){
					strat[source].second[i] = -1;
				}else{
					strat[source].second[i] = atoi(valuestr.c_str());
				}
				cout << strat[source].second[i] <<" ";
			}
			cout<< endl;
		}
	}else cout <<"fail to open"<< endl;
	cout<<"importing finished"<< endl;
}

void printStrat(vector< pair<int, vector<int> > >& strat){
	cout<< "Imported strategie"<< endl;
	for (int i= 0; i<strat.size(); i++){
		cout <<i<<" "<< strat[i].first<< " ";
		for (int action=0; action<strat[0].second.size(); action++){
			cout << strat[i].second[action]<< " ";
		}
		cout << endl;
	}
}
