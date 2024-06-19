#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <math.h>
//#include <unistd.h>

using namespace std;



int main(int argc, char** argv) {
  string ss;
  int k=0;
  ofstream file("grid.txt", ios::out | ios::trunc);
  file.close();
  for(int i=1;i<=4;i++)
    for(int j=1;j<=4;j++){
      k++;
    std::ostringstream st;
    st<<"./swn grid "<<k<<" "<<i<<" "<<j;
    ss=st.str();
    system(ss.c_str());
  }
    
    return(EXIT_SUCCESS);
}