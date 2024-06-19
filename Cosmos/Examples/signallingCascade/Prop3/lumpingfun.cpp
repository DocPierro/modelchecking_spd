#include "markingImpl.hpp"

const int reducePL_RE_ERK = 0;
const int reducePL_RE_ERKPP_Phase3 = 1;
const int reducePL_RE_ERK_MEKPF = 2;
const int reducePL_RE_Raf = 3;
const int reducePL_RE_Raf_RasGTP = 4;
const int reducePL_RE_ERKPP = 5;
const int reducePL_RE_RafP = 6;
const int reducePL_RE_RafP_Phase1 = 7;
const int reducePL_RE_ERKP = 8;
const int reducePL_RE_MEK = 9;
const int reducePL_RE_ERKP_Phase3 = 10;
const int reducePL_RE_ERKP_MEKPP = 11;
const int reducePL_RE_MEK_RafP = 12;
const int reducePL_RE_MEKP_Phase2 = 13;
const int reducePL_RE_MEKP = 14;
const int reducePL_RE_MEKPP_Phase2 = 15;
const int reducePL_RE_MEKPP = 16;
const int reducePL_RE_MEKP_RafP = 17;


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

double N1=1;
int N2=N;
int N3=0;

int remaining1(int t,int m){
	//cerr << "remaining tock:" << tock << " t: " << t << " remain: ";
	int t2 = fmin(m,fmin(ceil(t /((double)(N/N1))),tock));
	tock -= t2;
	//cerr << t2 << endl;
	return t2;
}
int remaining2(int t,int m){
	//cerr << "remaining tock:" << tock << " t: " << t << " remain: ";
	int t2 = fmin(m,fmin(ceil(t/((double)(N/N2))),tock));
	tock -= t2;
	//cerr << t2 << endl;
	return t2;
}


int remaining3(int t){
	//cerr << "remaining tock:" << tock << " t: " << t << " remain: ";
	int t2 = fmin(ceil(t/((double)(N/N3))),tock);
	tock -= t2;
	//cerr << t2 << endl;
	return t2;
}

bool SPN::precondition(const abstractMarking &Marking){return true;}

void SPN::lumpingFun(const abstractMarking &Marking,vector<int> &vect){
	tock = 3*N3;
	vect[reducePL_RE_ERKPP] = remaining3(Marking.P->_PL_ERKPP); //To Complete
	vect[reducePL_RE_ERKPP_Phase3] = remaining3(Marking.P->_PL_ERKPP_Phase3); //To Complete
	vect[reducePL_RE_ERKP_MEKPP] = remaining3(Marking.P->_PL_ERKP_MEKPP); //To Complete
	vect[reducePL_RE_ERKP] = remaining3(Marking.P->_PL_ERKP); //To Complete
	vect[reducePL_RE_ERKP_Phase3] = remaining3(Marking.P->_PL_ERKP_Phase3); //To Complete
	vect[reducePL_RE_ERK_MEKPF] = remaining3(Marking.P->_PL_ERK_MEKPF); //To Complete
	vect[reducePL_RE_ERK] = tock; //remaining(Marking.P->_PL_ERK); //To Complete

	//tock = 2*N2;
	vect[reducePL_RE_MEK_RafP] = min(4,Marking.P->_PL_MEK_RafP);
	vect[reducePL_RE_MEK] = Marking.P->_PL_MEK + Marking.P->_PL_MEK_RafP - vect[reducePL_RE_MEK_RafP];
	
	vect[reducePL_RE_MEKP_RafP] = min(4 - vect[reducePL_RE_MEK_RafP] , Marking.P->_PL_MEKP_RafP); //To Complete
	vect[reducePL_RE_MEKP] = Marking.P->_PL_MEKP + Marking.P->_PL_MEKP_RafP-vect[reducePL_RE_MEKP_RafP]; //To Complete

	
	vect[reducePL_RE_MEKP_Phase2] = Marking.P->_PL_MEKP_Phase2; //To Complete
	vect[reducePL_RE_MEKPP_Phase2] = Marking.P->_PL_MEKPP_Phase2; //To Complete-
	vect[reducePL_RE_MEKPP] = Marking.P->_PL_MEKPP;; //To Complete
	
	/*tock = max(0,5 - vect[reducePL_RE_MEK_RafP] - vect[reducePL_RE_MEKP_RafP]);
	vect[reducePL_RE_RafP] = remaining1(Marking.P->_PL_RafP,5);// + Marking.P->_PL_RafP_Phase1;
	vect[reducePL_RE_RafP_Phase1] = remaining1(Marking.P->_PL_RafP_Phase1,3);
	vect[reducePL_RE_Raf_RasGTP] = remaining1(Marking.P->_PL_Raf_RasGTP,2);
	vect[reducePL_RE_Raf] = tock;*/
	tock = max(0,4 - vect[reducePL_RE_MEK_RafP] - vect[reducePL_RE_MEKP_RafP]);
	vect[reducePL_RE_RafP] = remaining1(Marking.P->_PL_RafP,4);// + Marking.P->_PL_RafP_Phase1;
	vect[reducePL_RE_RafP_Phase1] = remaining1(Marking.P->_PL_RafP_Phase1,3);
	vect[reducePL_RE_Raf_RasGTP] = remaining1(Marking.P->_PL_Raf_RasGTP,2);
	vect[reducePL_RE_Raf] = tock;
	
	
	/*vect[reducePL_RE_RafP] = Marking.P->_PL_RafP;// + Marking.P->_PL_RafP_Phase1;
	vect[reducePL_RE_RafP_Phase1] = Marking.P->_PL_RafP_Phase1;
	vect[reducePL_RE_Raf_RasGTP] = Marking.P->_PL_Raf_RasGTP;
	vect[reducePL_RE_Raf] = Marking.P->_PL_Raf;*/

	
	
	/*
	vect[reducePL_RE_ERK] = Marking.P->_PL_RE_ERK; //To Complete
	vect[reducePL_RE_ERKP] = Marking.P->_PL_RE_ERKP; //To Complete
	vect[reducePL_RE_ERKPP] = Marking.P->_PL_RE_ERKPP; //To Complete
	vect[reducePL_RE_ERKPP_Phase3] = Marking.P->_PL_RE_ERKPP_Phase3; //To Complete
	vect[reducePL_RE_ERKP_MEKPP] = Marking.P->_PL_RE_ERKP_MEKPP; //To Complete
	vect[reducePL_RE_ERKP_Phase3] = Marking.P->_PL_RE_ERKP_Phase3; //To Complete
	vect[reducePL_RE_ERK_MEKPF] = Marking.P->_PL_RE_ERK_MEKPF; //To Complete
	vect[reducePL_RE_MEK] = Marking.P->_PL_RE_MEK; //To Complete
	vect[reducePL_RE_MEKP] = Marking.P->_PL_RE_MEKP; //To Complete
	vect[reducePL_RE_MEKPP] = Marking.P->_PL_RE_MEKPP; //To Complete
	vect[reducePL_RE_MEKPP_Phase2] = Marking.P->_PL_RE_MEKPP_Phase2; //To Complete
	vect[reducePL_RE_MEKP_Phase2] = Marking.P->_PL_RE_MEKP_Phase2; //To Complete
	vect[reducePL_RE_MEKP_RafP] = Marking.P->_PL_RE_MEKP_RafP; //To Complete
	vect[reducePL_RE_MEK_RafP] = Marking.P->_PL_RE_MEK_RafP; //To Complete
	vect[reducePL_RE_Raf] = Marking.P->_PL_RE_Raf; //To Complete
	vect[reducePL_RE_RafP] = Marking.P->_PL_RE_RafP; //To Complete
	vect[reducePL_RE_Raf_RasGTP] = Marking.P->_PL_RE_Raf_RasGTP; //To Complete
	 */
}
