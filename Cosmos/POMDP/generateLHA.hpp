#include <vector>
using namespace std;


void generateLHAfun(
					double ron ,
					int N,
					vector< vector< vector<double> > >& OldPlast,
					vector< vector< vector<double> > >& Plast,
					vector<double>& reward,
					double H,
					vector< vector<double> >& M,
					bool);


void generateLHAfromStrat(
						  double ron,
						  int N,
						  vector< pair<int, vector<int> > >& strat,
						  vector<double>& reward,
						  double H,
						  vector< vector<double> >& M,
						  bool);
