#include "markingImpl.hpp"

const int reducePL_RE_ERKP = 0;
const int reducePL_RE_ERKPP = 1;
const int reducePL_RE_ERKP_MEKPP = 2;
const int reducePL_RE_MEK = 3;
const int reducePL_RE_MEKP = 4;
const int reducePL_RE_MEKPP = 5;
const int reducePL_RE_MEKP_RafP = 6;
const int reducePL_RE_MEK_RafP = 7;
const int reducePL_RE_Raf = 8;
const int reducePL_RE_RafP = 9;
const int reducePL_RE_Raf_RasGTP = 10;
void SPN::print_state(const vector<int> &vect){
	cerr << "RE_ERKP = " << vect[reducePL_RE_ERKP] << endl;
	cerr << "RE_ERKPP = " << vect[reducePL_RE_ERKPP] << endl;
	cerr << "RE_ERKP_MEKPP = " << vect[reducePL_RE_ERKP_MEKPP] << endl;
	cerr << "RE_MEK = " << vect[reducePL_RE_MEK] << endl;
	cerr << "RE_MEKP = " << vect[reducePL_RE_MEKP] << endl;
	cerr << "RE_MEKPP = " << vect[reducePL_RE_MEKPP] << endl;
	cerr << "RE_MEKP_RafP = " << vect[reducePL_RE_MEKP_RafP] << endl;
	cerr << "RE_MEK_RafP = " << vect[reducePL_RE_MEK_RafP] << endl;
	cerr << "RE_Raf = " << vect[reducePL_RE_Raf] << endl;
	cerr << "RE_RafP = " << vect[reducePL_RE_RafP] << endl;
	cerr << "RE_Raf_RasGTP = " << vect[reducePL_RE_Raf_RasGTP] << endl;
}

void SPN::lumpingFun(const abstractMarking &Marking,vector<int> &vect){
		vect[reducePL_RE_ERKP] = Marking.P->_PL_RE_ERKP + Marking.P->_PL_ERKP_Phase3 + Marking.P->_PL_RE_ERK + Marking.P->_PL_RE_ERK_MEKPF; //To Complete
		vect[reducePL_RE_ERKPP] = Marking.P->_PL_RE_ERKPP + Marking.P->_PL_ERKPP_Phase3;//To Complete
		vect[reducePL_RE_ERKP_MEKPP] = Marking.P->_PL_RE_ERKP_MEKPP;
		vect[reducePL_RE_MEK] = Marking.P->_PL_RE_MEK;
		vect[reducePL_RE_MEKP] = Marking.P->_PL_RE_MEKP + Marking.P->_PL_MEKP_Phase2;//To Complete
		vect[reducePL_RE_MEKPP] = Marking.P->_PL_RE_MEKPP + Marking.P->_PL_MEKPP_Phase2 + Marking.P->_PL_RE_ERK_MEKPF+ Marking.P->_PL_RE_ERKP_MEKPP;//To Complete
		vect[reducePL_RE_MEKP_RafP] = Marking.P->_PL_RE_MEKP_RafP;
		vect[reducePL_RE_MEK_RafP] = Marking.P->_PL_RE_MEK_RafP;
		vect[reducePL_RE_Raf] = Marking.P->_PL_RE_Raf;
		vect[reducePL_RE_RafP] = Marking.P->_PL_RE_RafP + Marking.P->_PL_RafP_Phase1 + Marking.P->_PL_RE_MEK_RafP + Marking.P->_PL_RE_MEKP_RafP;//To Complete
		vect[reducePL_RE_Raf_RasGTP] = Marking.P->_PL_RE_Raf_RasGTP;
}
