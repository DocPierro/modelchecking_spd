#include "markingImpl.hpp"

const int reducePL_RE_p1 = 0;
const int reducePL_RE_p2 = 1;
const int reducePL_RE_p3 = 2;
const int reducePL_RE_p5 = 3;
const int reducePL_RE_p6 = 4;
const int reducePL_RE_p4 = 5;
void SPN::print_state(const vector<int> &vect) {
  cerr << "RE_p1 = " << vect[reducePL_RE_p1] << endl;
  cerr << "RE_p2 = " << vect[reducePL_RE_p2] << endl;
  cerr << "RE_p3 = " << vect[reducePL_RE_p3] << endl;
  cerr << "RE_p5 = " << vect[reducePL_RE_p5] << endl;
  cerr << "RE_p6 = " << vect[reducePL_RE_p6] << endl;
  cerr << "RE_p4 = " << vect[reducePL_RE_p4] << endl;
}
bool SPN::precondition(const abstractMarking &Marking) { return true; }

void SPN::lumpingFun(const abstractMarking &Marking, vector<int> &vect) {
  vect[reducePL_RE_p1] =
      Marking.P->_PL_RE_p1 + Marking.P->_PL_RE_p1bis; // To Complete
  vect[reducePL_RE_p2] =
      Marking.P->_PL_RE_p2 + Marking.P->_PL_RE_p2bis; // To Complete
  vect[reducePL_RE_p3] = Marking.P->_PL_RE_p3;        // To Complete
  vect[reducePL_RE_p4] =
      Marking.P->_PL_RE_p4 + Marking.P->_PL_RE_p4bis; // To Complete
  vect[reducePL_RE_p5] = Marking.P->_PL_RE_p5;        // To Complete
  vect[reducePL_RE_p6] =
      Marking.P->_PL_RE_p6 + Marking.P->_PL_RE_p6bis; // To Complete
}

double REHandling::direct_computation(const abstractMarking &Marking) {
  return -1.0;
}
