#include "markingImpl.hpp"

const int reducePL_RE_P0 = 0;
const int reducePL_RE_AP = 1;

void REHandling::print_state(const vector<int> &vect){
	cerr << "RE_P0 = " << vect[reducePL_RE_P0] << endl;
	cerr << "RE_AP = " << vect[reducePL_RE_AP] << endl;
}
bool REHandling::precondition(const abstractMarking &Marking){return true;}

void REHandling::lumpingFun(const abstractMarking &Marking,vector<int> &vect){
	vect[reducePL_RE_P0] = Marking.P->_PL_RE_P0; //To Complete
	vect[reducePL_RE_AP] = 1000-Marking.P->_PL_RE_P0; ; //To Complete
}

double REHandling::direct_computation(const abstractMarking &Marking){
	//return -1;
	return pow( (1-p)/p , (double)Marking.P->_PL_RE_P0 );
}

