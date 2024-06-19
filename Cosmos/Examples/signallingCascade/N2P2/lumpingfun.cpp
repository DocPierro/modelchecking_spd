#include "markingImpl.hpp"

const int reducePL_RE_ERK = 0;
const int reducePL_RE_ERKP = 1;
const int reducePL_RE_ERKPP = 2;
const int reducePL_RE_ERKPP_Phase3 = 3;
const int reducePL_RE_ERKP_MEKPP = 4;
const int reducePL_RE_ERKP_Phase3 = 5;
const int reducePL_RE_ERK_MEKPF = 6;
const int reducePL_RE_MEK = 7;
const int reducePL_RE_MEKP = 8;
const int reducePL_RE_MEKPP = 9;
const int reducePL_RE_MEKPP_Phase2 = 10;
const int reducePL_RE_MEKP_Phase2 = 11;
const int reducePL_RE_MEKP_RafP = 12;
const int reducePL_RE_MEK_RafP = 13;
const int reducePL_RE_Raf = 14;
const int reducePL_RE_RafP = 15;
const int reducePL_RE_RafP_Phase1 = 16;
const int reducePL_RE_Raf_RasGTP = 17;

const int N1=3;
const int N2=1;
const int N3=0;


void SPN::print_state(const vector<int> &vect){
	cerr << "RE_ERK = " << vect[reducePL_RE_ERK] << endl;
	cerr << "RE_ERKP = " << vect[reducePL_RE_ERKP] << endl;
	cerr << "RE_ERKPP = " << vect[reducePL_RE_ERKPP] << endl;
	cerr << "RE_ERKPP_Phase3 = " << vect[reducePL_RE_ERKPP_Phase3] << endl;
	cerr << "RE_ERKP_MEKPP = " << vect[reducePL_RE_ERKP_MEKPP] << endl;
	cerr << "RE_ERKP_Phase3 = " << vect[reducePL_RE_ERKP_Phase3] << endl;
	cerr << "RE_ERK_MEKPF = " << vect[reducePL_RE_ERK_MEKPF] << endl;
	cerr << "RE_MEK = " << vect[reducePL_RE_MEK] << endl;
	cerr << "RE_MEKP = " << vect[reducePL_RE_MEKP] << endl;
	cerr << "RE_MEKPP = " << vect[reducePL_RE_MEKPP] << endl;
	cerr << "RE_MEKPP_Phase2 = " << vect[reducePL_RE_MEKPP_Phase2] << endl;
	cerr << "RE_MEKP_Phase2 = " << vect[reducePL_RE_MEKP_Phase2] << endl;
	cerr << "RE_MEKP_RafP = " << vect[reducePL_RE_MEKP_RafP] << endl;
	cerr << "RE_MEK_RafP = " << vect[reducePL_RE_MEK_RafP] << endl;
	cerr << "RE_Raf = " << vect[reducePL_RE_Raf] << endl;
	cerr << "RE_RafP = " << vect[reducePL_RE_RafP] << endl;
	cerr << "RE_RafP_Phase1 = " << vect[reducePL_RE_RafP_Phase1] << endl;
	cerr << "RE_Raf_RasGTP = " << vect[reducePL_RE_Raf_RasGTP] << endl;
}


int tock;

int remaining3(int t){
	//cerr << "remaining tock:" << tock << " t: " << t << " remain: ";
	int t2 = fmin(ceil(t*((double)N3/ (double)N)),tock);
	tock -= t2;
	//cerr << t2 << endl;
	return t2;
}

int remaining2(int t){
	//cerr << "remaining tock:" << tock << " t: " << t << " remain: ";
	int t2 = fmin(ceil(t*((double)N2/ (double)N)),tock);
	tock -= t2;
	//cerr << t2 << endl;
	return t2;
}

int remaining1(int t){
	//cerr << "remaining tock:" << tock << " t: " << t << " remain: ";
	int t2 = fmin(ceil(t*((double)N1/ (double)N)),tock);
	tock -= t2;
	//cerr << t2 << endl;
	return t2;
}
bool SPN::precondition(const abstractMarking &Marking){
	return (Marking.P->_PL_RafP >=L);
}

void SPN::lumpingFun(const abstractMarking &Marking,vector<int> &vect){
	tock = 3*N3;
	vect[reducePL_RE_ERKPP] = remaining3(Marking.P->_PL_ERKPP); //To Complete
	vect[reducePL_RE_ERKPP_Phase3] = remaining3(Marking.P->_PL_ERKPP_Phase3); //To Complete
	vect[reducePL_RE_ERKP_MEKPP] = remaining3(Marking.P->_PL_ERKP_MEKPP); //To Complete
	vect[reducePL_RE_ERKP] = remaining3(Marking.P->_PL_ERKP); //To Complete
	vect[reducePL_RE_ERKP_Phase3] = remaining3(Marking.P->_PL_ERKP_Phase3); //To Complete
	vect[reducePL_RE_ERK_MEKPF] = remaining3(Marking.P->_PL_ERK_MEKPF); //To Complete
	vect[reducePL_RE_ERK] = tock; //remaining(Marking.P->_PL_ERK); //To Complete
	 
	/*
	vect[reducePL_RE_ERKPP_Phase3] = Marking.P->_PL_ERKPP_Phase3; //To Complete
	
	vect[reducePL_RE_ERKP_MEKPP] = fmin(2,Marking.P->_PL_ERKP_MEKPP); //To Complete
	vect[reducePL_RE_ERKPP] = Marking.P->_PL_ERKPP +fmax(0,Marking.P->_PL_ERKP_MEKPP-2); //To Complete
	
	
	vect[reducePL_RE_ERKP_Phase3] = Marking.P->_PL_ERKP_Phase3; //To Complete
	
	vect[reducePL_RE_ERK_MEKPF] = fmin(2-vect[reducePL_RE_ERKP_MEKPP] ,Marking.P->_PL_ERK_MEKPF); //To Complete
	vect[reducePL_RE_ERKP] = Marking.P->_PL_ERKP + fmax(0,Marking.P->_PL_ERK_MEKPF-2+vect[reducePL_RE_ERKP_MEKPP]); //To Complete
	
	vect[reducePL_RE_ERK] = Marking.P->_PL_ERK; //To Complete*/
	
	
	
	tock = fmax(0,2*N2 - vect[reducePL_RE_ERK_MEKPF] - vect[reducePL_RE_ERKP_MEKPP]);
	vect[reducePL_RE_MEKPP] = remaining2(Marking.P->_PL_MEKPP); //To Complete
	vect[reducePL_RE_MEKPP_Phase2] = remaining2(Marking.P->_PL_MEKPP_Phase2); //To Complete
	vect[reducePL_RE_MEKP_RafP] = remaining2(Marking.P->_PL_MEKP_RafP); //To Complete
	vect[reducePL_RE_MEKP] = remaining2(Marking.P->_PL_MEKP); //To Complete
	vect[reducePL_RE_MEKP_Phase2] = remaining2(Marking.P->_PL_MEKP_Phase2); //To Complete
	vect[reducePL_RE_MEK_RafP] = remaining2(Marking.P->_PL_MEK_RafP); //To Complete
	vect[reducePL_RE_MEK] = tock; //remaining(Marking.P->_PL_MEK); //To Complete
	/*
	vect[reducePL_RE_MEKPP] = Marking.P->_PL_MEKPP; //To Complete
	vect[reducePL_RE_MEKPP_Phase2] = Marking.P->_PL_MEKPP_Phase2; //To Complete
	vect[reducePL_RE_MEKP_RafP] = Marking.P->_PL_MEKP_RafP; //To Complete
	vect[reducePL_RE_MEKP] = Marking.P->_PL_MEKP; //To Complete
	vect[reducePL_RE_MEKP_Phase2] = Marking.P->_PL_MEKP_Phase2; //To Complete
	vect[reducePL_RE_MEK_RafP] = Marking.P->_PL_MEK_RafP; //To Complete
	vect[reducePL_RE_MEK] = Marking.P->_PL_MEK; //remaining(Marking.P->_PL_MEK); //To Complete
	*/
	
	
	tock = fmax(0,4*N1 - vect[reducePL_RE_MEK_RafP] - vect[reducePL_RE_MEKP_RafP]) ;
	vect[reducePL_RE_RafP] = remaining1(Marking.P->_PL_RafP); //To Complete
	vect[reducePL_RE_RafP_Phase1] = remaining1(Marking.P->_PL_RafP_Phase1); //To Complete
	vect[reducePL_RE_Raf_RasGTP] = remaining1(Marking.P->_PL_Raf_RasGTP); //To Complete
	vect[reducePL_RE_Raf] = tock; // remaining(Marking.P->_PL_Raf); //To Complete
	
	/*
	vect[reducePL_RE_RafP] = Marking.P->_PL_RafP; //To Complete
	vect[reducePL_RE_RafP_Phase1] = Marking.P->_PL_RafP_Phase1; //To Complete
	vect[reducePL_RE_Raf_RasGTP] = Marking.P->_PL_Raf_RasGTP; //To Complete
	vect[reducePL_RE_Raf] = Marking.P->_PL_Raf; // remaining(Marking.P->_PL_Raf); //To Complete
	 */
	//print_state(vect);
	
}
