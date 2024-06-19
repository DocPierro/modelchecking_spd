#include "markingImpl.hpp"


void REHandling::print_state(const vector<int> &vect){
	
}
bool REHandling::precondition(const abstractMarking &Marking){return true;}

void REHandling::lumpingFun(const abstractMarking &Marking,vector<int> &vect){
	
}

double REHandling::direct_computation(const abstractMarking &Marking){
	//return -1;
	return pow( (1-p)/p , (double)(Marking.P->_PL_P0 + Marking.P->_PL_P1) );
}

