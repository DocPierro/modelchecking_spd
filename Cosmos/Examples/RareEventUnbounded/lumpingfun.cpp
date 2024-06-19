
#include "markingImpl.hpp"

const int reducePL_RE_Queue1 = 0;
const int reducePL_RE_Queue2 = 1;


void REHandling::print_state(const vector<int> &vect){
	cerr << "RE_Queue1 = " << vect[reducePL_RE_Queue1] << endl;
	cerr << "RE_Queue2 = " << vect[reducePL_RE_Queue2] << endl;
}

bool REHandling::precondition(const abstractMarking &Marking){return true;}

void REHandling::lumpingFun(const abstractMarking &Marking,vector<int> &vect){
	
	vect[reducePL_RE_Queue1] = Marking.P->_PL_RE_Queue1; //To Complete
	vect[reducePL_RE_Queue2] = Marking.P->_PL_RE_Queue2; //To Complete

	int reste =0;
    for (int i=vect.size()-1; i>0; i--) {
        if(vect[i]>r){
            reste += vect[i]-r ;
            vect[i]=r;
        } else if(vect[i]+reste>r){
            reste -= (r - vect[i]);
            vect[i]=r;
        } else {
            vect[i] += reste;
            reste =0;
        }
    }
    vect[0] += reste;

}


double REHandling::direct_computation(const abstractMarking &Marking){return -1.0;}


/*

void SPN::lumpingFun(vector<int>* vect){
    int reste =0;
    for (int i=vect->size()-1; i>0; i--) {
        if((*vect)[i]>r){
            reste += (*vect)[i]-r ;
            (*vect)[i]=r;
        } else if((*vect)[i]+reste>r){
            reste -= (r -(*vect)[i]);
            (*vect)[i]=r;
        } else {
            (*vect)[i] += reste;
            reste =0;
        }
    }
    (*vect)[0] += reste;
    
}
*/
