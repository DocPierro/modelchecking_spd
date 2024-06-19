#ifndef _MarkingImpl_HPP
#define    _MarkingImpl_HPP
using namespace std;
#include <string.h>
#include "marking.hpp"
#include "markingTemplate.hpp"
class abstractBindingImpl {
public:
};
class abstractMarkingImpl {
public:
	int _PL_B13O1;
	int _PL_B8O1;
	int _PL_B7O1;
	int _PL_B9O1;
	int _PL_B11O1;
	int _PL_B10O1;
};

class abstractBindingIteratorImpl {
public:
	void reset(abstractMarkingImpl& m){};
	bool next(size_t& t, abstractMarkingImpl& m){return false;};
	size_t getIndex(){return 0;};
	abstractBinding getBinding(){return abstractBinding();};
};
#endif
