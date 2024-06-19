#include "spn.hpp"
#include <iomanip>
using namespace std;
#define PL_B13O1_LP 0
#define PL_B8O1_LP 1
#define PL_B7O1_LP 2
#define PL_B9O1_LP 3
#define PL_B11O1_LP 4
#define PL_B10O1_LP 5
#define TR_SimulinkTransition_RT 0

const int _nb_Place_B13O1=0;
const int _nb_Place_B8O1=1;
const int _nb_Place_B7O1=2;
const int _nb_Place_B9O1=3;
const int _nb_Place_B11O1=4;
const int _nb_Place_B10O1=5;
namespace hybridVar {
}
void REHandling::print_state(const vector<int> &vect){}
void REHandling::lumpingFun(const abstractMarking &M,vector<int> &vect){}
bool REHandling::precondition(const abstractMarking &M){return true;}
#include "marking.hpp"
#include "markingImpl.hpp"

void abstractMarking::resetToInitMarking(){
	P->_PL_B13O1 =0  ;
	P->_PL_B8O1 =0  ;
	P->_PL_B7O1 =0  ;
	P->_PL_B9O1 =0  ;
	P->_PL_B11O1 =0  ;
	P->_PL_B10O1 =0  ;
}


abstractMarking::abstractMarking() {
	P= new abstractMarkingImpl;
	resetToInitMarking();
}

abstractMarking::abstractMarking(const std::vector<int>& m) {
	P = new abstractMarkingImpl;
	setVector(m);
}
abstractMarking::abstractMarking(const abstractMarking& m) {
	P= new abstractMarkingImpl;
	*this = m;
};

abstractMarking& abstractMarking::operator = (const abstractMarking& m) {
	*P = *(m.P);
	return *this;
}

abstractMarking::~abstractMarking() {
	delete(P);
}


void abstractMarking::swap(abstractMarking& m) {
	abstractMarkingImpl* tmp = m.P;
	m.P = P;
	P = tmp;
}
void abstractMarking::printHeader(ostream &s)const{
s <<  setw(5) << "B13O1 ";
s <<  setw(5) << "B8O1 ";
s <<  setw(5) << "B7O1 ";
s <<  setw(5) << "B9O1 ";
s <<  setw(5) << "B11O1 ";
s <<  setw(5) << "B10O1 ";
}

void abstractMarking::print(ostream &s,double eTime)const{
	s <<  setw(4) << P->_PL_B13O1<<" ";
	s <<  setw(4) << P->_PL_B8O1<<" ";
	s <<  setw(4) << P->_PL_B7O1<<" ";
	s <<  setw(4) << P->_PL_B9O1<<" ";
	s <<  setw(4) << P->_PL_B11O1<<" ";
	s <<  setw(4) << P->_PL_B10O1<<" ";
}
void abstractMarking::printSedCmd(ostream &s)const{
	s << "-e 's/\\$B13O1\\$/";
	s << P->_PL_B13O1;
	s <<"/g' ";
	s << "-e 's/\\$B8O1\\$/";
	s << P->_PL_B8O1;
	s <<"/g' ";
	s << "-e 's/\\$B7O1\\$/";
	s << P->_PL_B7O1;
	s <<"/g' ";
	s << "-e 's/\\$B9O1\\$/";
	s << P->_PL_B9O1;
	s <<"/g' ";
	s << "-e 's/\\$B11O1\\$/";
	s << P->_PL_B11O1;
	s <<"/g' ";
	s << "-e 's/\\$B10O1\\$/";
	s << P->_PL_B10O1;
	s <<"/g' ";
}

int abstractMarking::getNbOfTokens(int p)const {
	switch (p) {
		case 0: return P->_PL_B13O1;
		case 1: return P->_PL_B8O1;
		case 2: return P->_PL_B7O1;
		case 3: return P->_PL_B9O1;
		case 4: return P->_PL_B11O1;
		case 5: return P->_PL_B10O1;
     }
}

std::vector<int> abstractMarking::getVector()const {
	std::vector<int> v(6);
	v.reserve(7);
	size_t i = 0;
	v[i++]= P->_PL_B13O1;
	v[i++]= P->_PL_B8O1;
	v[i++]= P->_PL_B7O1;
	v[i++]= P->_PL_B9O1;
	v[i++]= P->_PL_B11O1;
	v[i++]= P->_PL_B10O1;
     return v;
}

void abstractMarking::setVector(const std::vector<int>&v) {
	size_t i = 0;
	P->_PL_B13O1 = v[i++];
	P->_PL_B8O1 = v[i++];
	P->_PL_B7O1 = v[i++];
	P->_PL_B9O1 = v[i++];
	P->_PL_B11O1 = v[i++];
	P->_PL_B10O1 = v[i++];
};

void abstractMarking::Symmetrize(){
}bool abstractBinding::next() {
	idcount++;
	return false;
};
abstractBinding::abstractBinding() {
       idcount=0;
}
abstractBinding::abstractBinding(const abstractBinding& m) {
	idcount = m.idcount;
}
abstractBinding::~abstractBinding() {
}
abstractBinding& abstractBinding::operator = (const abstractBinding& m) {
	idcount = m.idcount;
       return *this;
}
void abstractBinding::print()const{
}
int abstractBinding::id()const{
	return idcount;
}
int abstractBinding::idTotal()const{
	 return 0;
}

void abstractBindingIterator::reset(abstractMarking& m) {
	P->reset(*(m.P));
};
abstractBindingIterator::abstractBindingIterator(abstractMarking& m) {
	P = new abstractBindingIteratorImpl;
	reset(m);
};
bool abstractBindingIterator::next(size_t& t,abstractMarking& m) {
	return P->next(t,*(m.P));
};
abstractBindingIterator::~abstractBindingIterator() {
	delete(P);
};
size_t abstractBindingIterator::getIndex() {
	return P->getIndex();
};
abstractBinding abstractBindingIterator::getBinding() {
	return P->getBinding();
};
static const int EMPTY_array[1]={-1};
const int* SPN::PossiblyEnabled[] = {EMPTY_array};

const int* SPN::PossiblyDisabled[] = {EMPTY_array};

const int* SPN::FreeMarkDepT[] = {EMPTY_array};

static spn_trans TransArray[1] = { _trans(0,DETERMINISTIC,0,1, 0, "SimulinkTransition"),  }; 
SPN::SPN():
customDistr(*(new CustomDistr())),pl(6), tr(1) ,Transition(TransArray,TransArray +1),Place(6),ParamDistr(),TransitionConditions(1,0){
    Path ="main.cpp";
    Place[0].label =" B13O1";
    Place[0].isTraced = 1;
    Place[1].label =" B8O1";
    Place[1].isTraced = 1;
    Place[2].label =" B7O1";
    Place[2].isTraced = 1;
    Place[3].label =" B9O1";
    Place[3].isTraced = 1;
    Place[4].label =" B11O1";
    Place[4].isTraced = 1;
    Place[5].label =" B10O1";
    Place[5].isTraced = 1;
}

bool SPN::IsEnabled(TR_PL_ID t, const abstractBinding &b)const{


		return true;
}

void SPN::fire(TR_PL_ID t, const abstractBinding &b,REAL_TYPE time){
	lastTransition = t;

{
	}
}

void SPN::unfire(TR_PL_ID t, const abstractBinding &b){


}

const abstractBinding* SPN::nextPossiblyEnabledBinding(size_t targettr,const abstractBinding& b,size_t *bindingNum)const {
	switch(lastTransition*(tr+1) + targettr){
	default:
		if(*bindingNum==Transition[targettr].bindingList.size())return NULL;
		*bindingNum = *bindingNum +1;
		return &(Transition[targettr].bindingList[*bindingNum-1]);
}}
const abstractBinding* SPN::nextPossiblyDisabledBinding(size_t targettr,const abstractBinding& b,size_t *bindingNum)const {
	switch(lastTransition*(tr+1) + targettr){
	default:
		if(*bindingNum==Transition[targettr].bindingList.size())return NULL;
		*bindingNum = *bindingNum +1;
		return &(Transition[targettr].bindingList[*bindingNum-1]);
}}
void SPN::setConditionsVector(){
}
void SPN::GetDistParameters(TR_PL_ID t, const abstractBinding &b)const{
using namespace hybridVar;

	{
		ParamDistr[0]= ( double ) 0 ;
	}

}

REAL_TYPE SPN::GetPriority(TR_PL_ID t, const abstractBinding &b)const{
using namespace hybridVar;

		return (double)1 ;
}

REAL_TYPE SPN::GetWeight(TR_PL_ID t, const abstractBinding &b)const{
using namespace hybridVar;

		return (double)1 ;
}

void SPN::Msimple(){
	vector<int> tab;
	Msimpletab = tab;
}
void SPN::reset() {
	lastTransitionTime = 0;
	Marking.resetToInitMarking();
}

