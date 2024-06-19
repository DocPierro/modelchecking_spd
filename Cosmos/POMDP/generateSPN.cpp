
#include <fstream>
#include <iostream>

#include "generateSPN.hpp"

void generateSPNfun(vector< vector<double> > &M){
    
    cout << "genere SPN" << endl;
    
    ofstream SpnFile("generated.gspn", ios::out | ios::trunc);
    
    SpnFile << "NbPlaces = "<< M.size() << ";" << endl;
    
    int nbtr=0;
    for(int i =0; i<M.size(); i++){
        for(int j = 0; j< M.size()  ;j++){
            if(M[i][j] != 0) nbtr++;
        }
    }
    SpnFile << "NbTransitions = "<< nbtr << ";" << endl<<endl;
    
    SpnFile << "PlacesList = { V0 ";
    for(int i=1; i<M.size(); i++)SpnFile << ", V" << i << " ";
    SpnFile << "};" << endl;
    
    nbtr = 0;
    SpnFile << "TransitionsList = { ";
    for(int i =0; i<M.size(); i++){
        for(int j = 0; j< M.size()  ;j++){
            if(M[i][j] != 0){
                if (nbtr>0)SpnFile << " , ";
                SpnFile << "Tr" << i << "_" << j;
                nbtr++;
            }
        }
    }
    SpnFile << " };" << endl << endl;
    
    SpnFile << "Marking = {" << endl << "\t(V0 , 1);" << endl;
    for(int i=1; i<M.size(); i++)SpnFile << "\t(V" << i << " , 0);" << endl;
    SpnFile << "};" << endl << endl;

    nbtr = 0;
    SpnFile << "Transitions = { " << endl;
    for(int i =0; i<M.size(); i++){
        for(int j = 0; j< M.size()  ;j++){
            if(M[i][j] != 0){
                SpnFile <<"\t(Tr" << i << "_" << j;
                SpnFile <<" , EXPONENTIAL("<<M[i][j]<< "),1, 1,SINGLE);"<<endl;
                nbtr++;
            }
        }
    }
    SpnFile << "};" << endl << endl;

    nbtr = 0;
    SpnFile << "InArcs = { " << endl;
    for(int i =0; i<M.size(); i++){
        for(int j = 0; j< M.size()  ;j++){
            if(M[i][j] != 0){
                SpnFile <<"\t(V"<< i ;
                SpnFile << " ,Tr" << i << "_" << j;
                SpnFile <<" , 1);"<<endl;
                nbtr++;
            }
        }
    }
    SpnFile << "};" << endl << endl;

    nbtr = 0;
    SpnFile << "OutArcs = { " << endl;
    for(int i =0; i<M.size(); i++){
        for(int j = 0; j< M.size()  ;j++){
            if(M[i][j] != 0){
                SpnFile <<"\t(Tr" << i << "_" << j;
                SpnFile <<", V"<< j << " , 1);"<<endl;
                nbtr++;
            }
        }
    }
    SpnFile << "};" << endl << endl;

    
    SpnFile.close();
}