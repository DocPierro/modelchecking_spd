#include "marking.hpp"
#include "markingImpl.hpp"
#include "SKTime.hpp"
#include <cmath>

abstractMarking::abstractMarking() {
	P = new abstractMarkingImpl;
	resetToInitMarking();
}

abstractMarking::abstractMarking(const abstractMarking& m) {
	P = new abstractMarkingImpl;
	*this = m;
}

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

void abstractMarking::printHeader(ostream& s) const{
	s << setw(5) << "Integrator2(13).1 ";
	s << setw(5) << "Integrator1(8).1 ";
	s << setw(5) << "Integrator(7).1 ";
	s << setw(5) << "Gain(9).1 ";
	s << setw(5) << "Gain1(11).1 ";
	s << setw(5) << "Switch(10).1 ";
}

void abstractMarking::print(ostream& s,double eTime) const{
	s << setw(1) << (eTime > 0 ? P->_BLOCK13_OUT1[(P->lastEntry)] : P->_BLOCK13_OUT1[(P->lastPrintEntry)]) << " ";
	s << setw(1) << (eTime > 0 ? P->_BLOCK8_OUT1[(P->lastEntry)] : P->_BLOCK8_OUT1[(P->lastPrintEntry)]) << " ";
	s << setw(1) << (eTime > 0 ? P->_BLOCK7_OUT1[(P->lastEntry)] : P->_BLOCK7_OUT1[(P->lastPrintEntry)]) << " ";
	s << setw(1) << (eTime > 0 ? P->_BLOCK9_OUT1[(P->lastEntry)] : P->_BLOCK9_OUT1[(P->lastPrintEntry)]) << " ";
	s << setw(1) << (eTime > 0 ? P->_BLOCK11_OUT1[(P->lastEntry)] : P->_BLOCK11_OUT1[(P->lastPrintEntry)]) << " ";
	s << setw(1) << (eTime > 0 ? P->_BLOCK10_OUT1[(P->lastEntry)] : P->_BLOCK10_OUT1[(P->lastPrintEntry)]) << " ";
}

void abstractMarking::printSedCmd(ostream& s) const{
	s << "-e 's/\\$B13O1\\$/" << P->_BLOCK13_OUT1[(P->lastPrintEntry)] << "/g' ";
	s << "-e 's/\\$B8O1\\$/" << P->_BLOCK8_OUT1[(P->lastPrintEntry)] << "/g' ";
	s << "-e 's/\\$B7O1\\$/" << P->_BLOCK7_OUT1[(P->lastPrintEntry)] << "/g' ";
	s << "-e 's/\\$B9O1\\$/" << P->_BLOCK9_OUT1[(P->lastPrintEntry)] << "/g' ";
	s << "-e 's/\\$B11O1\\$/" << P->_BLOCK11_OUT1[(P->lastPrintEntry)] << "/g' ";
	s << "-e 's/\\$B10O1\\$/" << P->_BLOCK10_OUT1[(P->lastPrintEntry)] << "/g' ";
}

void abstractMarking::resetToInitMarking() {
	P->lastEntry = 0;
	P->lastPrintEntry = 0;
	P->countDown = 0;
	P->currentLookup = 0.0;
	P->_TIME = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	P->_BLOCK13_OUT1 = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	P->_BLOCK8_OUT1 = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	P->_BLOCK7_OUT1 = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	P->_BLOCK9_OUT1 = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	P->_BLOCK11_OUT1 = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	P->_BLOCK10_OUT1 = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	P->_STATE10 = {0,0,0,0,0,0,0,0,0,0};
}

bool abstractBinding::next() {
	idcount++;
	return false;
}

abstractBinding::abstractBinding() {
	idcount=0;
}

abstractBinding::abstractBinding(const abstractBinding& m) {
	idcount=m.idcount;
}
abstractBinding::~abstractBinding() { }
abstractBinding& abstractBinding::operator = (const abstractBinding& m) {
	idcount = m.idcount;
	return *this;
}
void abstractBinding::print() const{ }
int abstractBinding::id() const { return idcount; }
int abstractBinding::idTotal() const { return 0; }

template<class EQT>
SKModel<EQT>::SKModel() {
	Transition.push_back(SKTransition());
}

template<class EQT>
void SKModel<EQT>::initialEventsQueue(EQT& EQ, timeGen& tg) {
	Event E;
	generateEvent(0.0, E, 0, tg);
	EQ.insert(E);
}

template<class EQT>
void SKModel<EQT>::generateEvent(double ctime, Event& E,size_t Id, timeGen& tg) {
	double t = ctime;

	E.transition = Id;
	E.time = t;
	E.priority = 1;
	E.weight = 1;
};

template<class EQT>
void SKModel<EQT>::reset() {
	Marking.resetToInitMarking();
};

template <class EQT>
int SKModel<EQT>::findLatencyIndex(double latency) {
	SKTime currTime = Marking.P->currentLookup;
	while (Marking.P->countDown > 0 && Marking.P->_TIME[Marking.P->countDown] > (currTime - latency)) {
		Marking.P->countDown--;
	}
	if (Marking.P->_TIME[Marking.P->countDown] > (currTime - latency)) { return -1;
	} else { return Marking.P->countDown; }
}

template<class EQT>
void SKModel<EQT>::initialiseIntegrators(int idx) {
	Marking.P->_BLOCK13_OUT1[Marking.P->lastEntry] = 0.000000;
	Marking.P->_BLOCK8_OUT1[Marking.P->lastEntry] = 0.000000;
	Marking.P->_BLOCK7_OUT1[Marking.P->lastEntry] = 1.000000;
};

template<class EQT>
void SKModel<EQT>::executeIntegrators(int idx) {
	estimateIntegrators(idx,Marking.P->lastEntry-1,(Marking.P->_TIME[idx] - Marking.P->_TIME[Marking.P->lastEntry-1]).getDouble());
};

template<class EQT>
SKTime SKModel<EQT>::estimateIntegrators(int idx,int previdx,SKTime stepSK) {
	double step = stepSK.getDouble();
	int idxtampon = Marking.P->lastEntry+1;
	SKTime t0 = Marking.P->_TIME[previdx];
	Marking.P->_TIME[idxtampon+0] = Marking.P->_TIME[previdx];
	double k0_b13 = Marking.P->_BLOCK13_OUT1[previdx];
	double y0_b13 = k0_b13;
	double k0_b8 = Marking.P->_BLOCK8_OUT1[previdx];
	double y0_b8 = k0_b8;
	double k0_b7 = Marking.P->_BLOCK7_OUT1[previdx];
	double y0_b7 = k0_b7;
	Marking.P->_BLOCK8_OUT1[idxtampon+0] = y0_b8;
	Marking.P->_BLOCK7_OUT1[idxtampon+0] = y0_b7;
	executeBlock9(idxtampon+0);
	Marking.P->_BLOCK8_OUT1[idxtampon+0] = y0_b8;
	executeBlock11(idxtampon+0);
	executeBlock10(idxtampon+0);
	SKTime t1 = t0.getDouble() + step/4.;
	Marking.P->currentLookup = t1;
	Marking.P->countDown = Marking.P->lastEntry;
	Marking.P->_TIME[idxtampon+1] = t1;
	double k1_b13 = Marking.P->_BLOCK10_OUT1[idxtampon+0];
	double y1_b13 = y0_b13 + step/4. * k1_b13;
	double k1_b8 = Marking.P->_BLOCK9_OUT1[idxtampon+0];
	double y1_b8 = y0_b8 + step/4. * k1_b8;
	double k1_b7 = Marking.P->_BLOCK8_OUT1[idxtampon+0];
	double y1_b7 = y0_b7 + step/4. * k1_b7;
	Marking.P->_BLOCK8_OUT1[idxtampon+1] = y1_b8;
	Marking.P->_BLOCK7_OUT1[idxtampon+1] = y1_b7;
	executeBlock9(idxtampon+1);
	Marking.P->_BLOCK8_OUT1[idxtampon+1] = y1_b8;
	executeBlock11(idxtampon+1);
	executeBlock10(idxtampon+1);
	SKTime t2 = t0.getDouble() + step*3/8.;
	Marking.P->currentLookup = t2;
	Marking.P->countDown = Marking.P->lastEntry;
	Marking.P->_TIME[idxtampon+2] = t2;
	double k2_b13 = Marking.P->_BLOCK10_OUT1[idxtampon+1];
	double y2_b13 = y0_b13 + step*3/32. * k1_b13 + step*9/32. * k2_b13;
	double k2_b8 = Marking.P->_BLOCK9_OUT1[idxtampon+1];
	double y2_b8 = y0_b8 + step*3/32. * k1_b8 + step*9/32. * k2_b8;
	double k2_b7 = Marking.P->_BLOCK8_OUT1[idxtampon+1];
	double y2_b7 = y0_b7 + step*3/32. * k1_b7 + step*9/32. * k2_b7;
	Marking.P->_BLOCK8_OUT1[idxtampon+2] = y2_b8;
	Marking.P->_BLOCK7_OUT1[idxtampon+2] = y2_b7;
	executeBlock9(idxtampon+2);
	Marking.P->_BLOCK8_OUT1[idxtampon+2] = y2_b8;
	executeBlock11(idxtampon+2);
	executeBlock10(idxtampon+2);
	SKTime t3 = t0.getDouble() + step*12/3.;
	Marking.P->currentLookup = t3;
	Marking.P->countDown = Marking.P->lastEntry;
	Marking.P->_TIME[idxtampon+3] = t3;
	double k3_b13 = Marking.P->_BLOCK10_OUT1[idxtampon+2];
	double y3_b13 = y0_b13 + step*1932/2197. * k1_b13 - step*7200/2197. * k2_b13 + step*7296/2197 * k3_b13;
	double k3_b8 = Marking.P->_BLOCK9_OUT1[idxtampon+2];
	double y3_b8 = y0_b8 + step*1932/2197. * k1_b8 - step*7200/2197. * k2_b8 + step*7296/2197 * k3_b8;
	double k3_b7 = Marking.P->_BLOCK8_OUT1[idxtampon+2];
	double y3_b7 = y0_b7 + step*1932/2197. * k1_b7 - step*7200/2197. * k2_b7 + step*7296/2197 * k3_b7;
	Marking.P->_BLOCK8_OUT1[idxtampon+3] = y3_b8;
	Marking.P->_BLOCK7_OUT1[idxtampon+3] = y3_b7;
	executeBlock9(idxtampon+3);
	Marking.P->_BLOCK8_OUT1[idxtampon+3] = y3_b8;
	executeBlock11(idxtampon+3);
	executeBlock10(idxtampon+3);
	SKTime t4 = t0.getDouble() + step;
	Marking.P->currentLookup = t4;
	Marking.P->countDown = Marking.P->lastEntry;
	Marking.P->_TIME[idxtampon+4] = t4;
	double k4_b13 = Marking.P->_BLOCK10_OUT1[idxtampon+3];
	double y4_b13 = y0_b13 + step*439/216. * k1_b13 - step*8. * k2_b13 + step*3680/513. * k3_b13 - step*845/4104. * k4_b13;
	double k4_b8 = Marking.P->_BLOCK9_OUT1[idxtampon+3];
	double y4_b8 = y0_b8 + step*439/216. * k1_b8 - step*8. * k2_b8 + step*3680/513. * k3_b8 - step*845/4104. * k4_b8;
	double k4_b7 = Marking.P->_BLOCK8_OUT1[idxtampon+3];
	double y4_b7 = y0_b7 + step*439/216. * k1_b7 - step*8. * k2_b7 + step*3680/513. * k3_b7 - step*845/4104. * k4_b7;
	Marking.P->_BLOCK8_OUT1[idxtampon+4] = y4_b8;
	Marking.P->_BLOCK7_OUT1[idxtampon+4] = y4_b7;
	executeBlock9(idxtampon+4);
	Marking.P->_BLOCK8_OUT1[idxtampon+4] = y4_b8;
	executeBlock11(idxtampon+4);
	executeBlock10(idxtampon+4);
	SKTime t5 = t0.getDouble() + step*1/2.;
	Marking.P->currentLookup = t5;
	Marking.P->countDown = Marking.P->lastEntry;
	Marking.P->_TIME[idxtampon+5] = t5;
	double k5_b13 = Marking.P->_BLOCK10_OUT1[idxtampon+4];
	double y5_b13 = y0_b13 - step*8/27. * k1_b13 + step*2. * k2_b13 - step*3544/2565. * k3_b13 + step*1859/4104. * k4_b13 - step*11/40. * k5_b13;
	double k5_b8 = Marking.P->_BLOCK9_OUT1[idxtampon+4];
	double y5_b8 = y0_b8 - step*8/27. * k1_b8 + step*2. * k2_b8 - step*3544/2565. * k3_b8 + step*1859/4104. * k4_b8 - step*11/40. * k5_b8;
	double k5_b7 = Marking.P->_BLOCK8_OUT1[idxtampon+4];
	double y5_b7 = y0_b7 - step*8/27. * k1_b7 + step*2. * k2_b7 - step*3544/2565. * k3_b7 + step*1859/4104. * k4_b7 - step*11/40. * k5_b7;
	Marking.P->_BLOCK8_OUT1[idxtampon+5] = y5_b8;
	Marking.P->_BLOCK7_OUT1[idxtampon+5] = y5_b7;
	executeBlock9(idxtampon+5);
	Marking.P->_BLOCK8_OUT1[idxtampon+5] = y5_b8;
	executeBlock11(idxtampon+5);
	executeBlock10(idxtampon+5);
	double k6_b7 = Marking.P->_BLOCK8_OUT1[idxtampon+5];
	double rk4_7 = y0_b7 + 25/216.*step*k1_b7 + 1408/2565.*step*k3_b7 + 2197/4101.*step*k4_b7 - 1/5.*step*k5_b7;
	double rk5_7 = y0_b7 + 16/135.*step*k1_b7 + 6656/12825.*step*k3_b7 + 28561/56430.*step*k4_b7 - 9/50.*step*k5_b7 + 2/55.*step*k6_b7;
	Marking.P->_BLOCK7_OUT1[idx] = rk4_7;
	if (abs(rk4_7 - rk5_7) > 0.0001) { return (stepSK.getDouble() / 2); }
	double k6_b8 = Marking.P->_BLOCK9_OUT1[idxtampon+5];
	double rk4_8 = y0_b8 + 25/216.*step*k1_b8 + 1408/2565.*step*k3_b8 + 2197/4101.*step*k4_b8 - 1/5.*step*k5_b8;
	double rk5_8 = y0_b8 + 16/135.*step*k1_b8 + 6656/12825.*step*k3_b8 + 28561/56430.*step*k4_b8 - 9/50.*step*k5_b8 + 2/55.*step*k6_b8;
	Marking.P->_BLOCK8_OUT1[idx] = rk4_8;
	if (abs(rk4_8 - rk5_8) > 0.0001) { return (stepSK.getDouble() / 2); }
	double k6_b13 = Marking.P->_BLOCK10_OUT1[idxtampon+5];
	double rk4_13 = y0_b13 + 25/216.*step*k1_b13 + 1408/2565.*step*k3_b13 + 2197/4101.*step*k4_b13 - 1/5.*step*k5_b13;
	double rk5_13 = y0_b13 + 16/135.*step*k1_b13 + 6656/12825.*step*k3_b13 + 28561/56430.*step*k4_b13 - 9/50.*step*k5_b13 + 2/55.*step*k6_b13;
	Marking.P->_BLOCK13_OUT1[idx] = rk4_13;
	if (abs(rk4_13 - rk5_13) > 0.0001) { return (stepSK.getDouble() / 2); }
	return stepSK;
/* 
	return stepSK;
*/ 
};

template<class EQT>
double SKModel<EQT>::linearInterpolation(SKTime sktA, SKTime sktB, double vA, double vB, SKTime time) {
	double tA = sktA.getDouble(); double tB = sktB.getDouble(); double t = time.getDouble();
	return (vB - vA)/(tB - tA) * (t - tA) + vA;
};

template<class EQT>
void SKModel<EQT>::checkState10(int idx) {
	Marking.P->_STATE10[idx] = (Marking.P->_BLOCK8_OUT1[idx] > 0.000000) ? 1 : 0;
};
template<class EQT>
SKTime SKModel<EQT>::linAppCrossing10(SKTime tU,SKTime tA,double vU,double vA) {
	fprintf(stderr,"[lAC10:] times (%f %f) values (%f %f)\n",tU.getDouble(),tA.getDouble(),vU,vA);
	return tU.getDouble() + (0.000000 - vU)*(tA.getDouble() - tU.getDouble())/(vA - vU);
};

template<class EQT>
void SKModel<EQT>::computeBkwd10(int idx) {
	SKTime step = Marking.P->_TIME[idx] - Marking.P->_TIME[Marking.P->lastEntry-1];
	estimateIntegrators(idx,Marking.P->lastEntry-1,step);
};
template<class EQT>
SKTime SKModel<EQT>::findStateChange10(int idx,SKTime stepSK) {
	int idxtampon = Marking.P->lastEntry+7;
	SKTime step = stepSK;
	bool signChange = true;
	Marking.P->_TIME[idxtampon+0] = Marking.P->_TIME[idx-1];
	Marking.P->_TIME[idxtampon+1] = Marking.P->_TIME[idx-1] + stepSK;
	SKTime candidate = Marking.P->_TIME[idxtampon+1];
	while (signChange) {
		step = Marking.P->_TIME[idxtampon+1] - Marking.P->_TIME[idxtampon+0];
		fprintf(stderr,"[fSC10:Step] %f\n",step.getDouble());
		computeBkwd10(idxtampon+0); computeBkwd10(idxtampon+1);
		checkState10(idxtampon+0); checkState10(idxtampon+1);
		signChange = not (Marking.P->_STATE10[idxtampon+0] == Marking.P->_STATE10[idxtampon+1]);
		if (signChange) {
			candidate = linAppCrossing10(Marking.P->_TIME[idxtampon+0], Marking.P->_TIME[idxtampon+1], Marking.P->_BLOCK8_OUT1[idxtampon+0], Marking.P->_BLOCK8_OUT1[idxtampon+1]);
			fprintf(stderr,"[fSC10: New Time Candidate is : %f\n",candidate.getDouble());
			if (Marking.P->_TIME[idxtampon+0]==candidate) break;
			if (Marking.P->_TIME[idxtampon+1]==candidate) break;
			Marking.P->_TIME[idxtampon+2] = candidate; computeBkwd10(idxtampon+2); checkState10(idxtampon+2);
			(Marking.P->_STATE10[idxtampon+0] == Marking.P->_STATE10[idxtampon+2]) ? (Marking.P->_TIME[idxtampon+0]=candidate) : (Marking.P->_TIME[idxtampon+1]=candidate);
		}
	}
	fprint(stderr,"[fSC10: Result] Candidate is %f old time was %f\n",candidate.getDouble(),Marking.P->_TIME[idx-1].getDouble());
	step = candidate - Marking.P->_TIME[idx-1];
	fprintf(stderr,"[fSC10: Result] Step is %f\n",step.getDouble());
	return candidate - Marking.P->_TIME[idx-1];
};

template<class EQT>
void SKModel<EQT>::executeBlock9(int idx) {
	Marking.P->_BLOCK9_OUT1[idx] = -1.000000 * Marking.P->_BLOCK7_OUT1[idx];
};

template<class EQT>
void SKModel<EQT>::executeBlock11(int idx) {
	Marking.P->_BLOCK11_OUT1[idx] = -1.000000 * Marking.P->_BLOCK8_OUT1[idx];
};

template<class EQT>
void SKModel<EQT>::executeBlock10(int idx) {
	Marking.P->_STATE10[idx] = Marking.P->_STATE10[Marking.P->lastEntry];
	if (Marking.P->_STATE10[idx] == 0) {
		Marking.P->_BLOCK10_OUT1[idx] = Marking.P->_BLOCK11_OUT1[idx];
	} else {
		Marking.P->_BLOCK10_OUT1[idx] = Marking.P->_BLOCK8_OUT1[idx];
	}
};

template<class EQT>
void SKModel<EQT>::executeBlock14(int idx) {
};

template<class EQT>
void SKModel<EQT>::fire(size_t tr,const abstractBinding&, double ctime) {
	Marking.P->countDown = Marking.P->lastEntry;
	Marking.P->lastPrintEntry = Marking.P->lastEntry;
	Marking.P->_TIME[Marking.P->lastEntry] = ctime;
	Marking.P->currentLookup = Marking.P->_TIME[Marking.P->lastEntry];
	if (Marking.P->lastEntry==0) { initialiseIntegrators(0); }
	else { executeIntegrators(Marking.P->lastEntry); }
	executeBlock9(Marking.P->lastEntry);
	executeBlock11(Marking.P->lastEntry);
	checkState10(Marking.P->lastEntry);
	executeBlock10(Marking.P->lastEntry);
	executeBlock14(Marking.P->lastEntry);
	fprintf(stderr,"[FIRED!]\n");
};

template<class EQT>
void SKModel<EQT>::update(double ctime, size_t tr, const abstractBinding& b, EQT &EQ, timeGen &TG) {
	EQ.remove(tr, b.id());
	Marking.P->lastEntry = Marking.P->lastEntry + 1;
	Marking.P->_TIME.push_back(0.0);
	Marking.P->_BLOCK13_OUT1.push_back(0.0);
	Marking.P->_BLOCK8_OUT1.push_back(0.0);
	Marking.P->_BLOCK7_OUT1.push_back(0.0);
	Marking.P->_BLOCK9_OUT1.push_back(0.0);
	Marking.P->_BLOCK11_OUT1.push_back(0.0);
	Marking.P->_BLOCK10_OUT1.push_back(0.0);
	Marking.P->_STATE10.push_back(0);
	SKTime t = ctime;
	SKTime step = 0.200000;
	SKTime oldStep = 0.0;
	if (t < 10.) {
		Event E;
		while (oldStep != step) {
			oldStep = step;
			t = step + ctime;
			if (t < 10.) {
				Marking.P->countDown = Marking.P->lastEntry;
				Marking.P->_TIME[Marking.P->lastEntry] = t;
				step = estimateIntegrators(Marking.P->lastEntry,Marking.P->lastEntry-1,step);
				if (oldStep != step) { continue; }
				executeBlock9(Marking.P->lastEntry);
				executeBlock11(Marking.P->lastEntry);
				step = findStateChange10(Marking.P->lastEntry,step);
				fprintf(stderr,"[MAIN] New step : %f\n",step.getDouble());
				checkState10(Marking.P->lastEntry);
				executeBlock10(Marking.P->lastEntry);
				executeBlock14(Marking.P->lastEntry);
			} else {
				step = 10. - ctime;

			}
		}
	generateEvent(t.getDouble(), E, 0, TG);
	EQ.insert(E);
	} else { EQ.reset(); }
};
template class SKModel<EventsQueue<std::vector<SKTransition>>>;
