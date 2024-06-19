#include <vector>
using namespace std;

vector< vector<double> > generateMatrix(int n);


vector< vector<double> > generateLinearMatrix(int n, double p, double q);


vector<double > generateLinearReward(int n,double Rmax);


vector< vector<double> > generateGridMatrix(int n,
											int m,
											double u,
											double r,
											double d,
											double l);


vector<double > generateGridReward(int n,int m);
