/*
Copyright (c) 2011, Johannes
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are 
met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the distribution
      
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.



This is a modified version of the original eureqa_select.cpp -- SUMO team

MATLAB API to use Eureqa server for nonlinear regression and predictor selection
(http://creativemachines.cornell.edu/eureqa)

tested on Linux, with possible usage on Windows or Macos

1.) INSTALLATION:
    
    a) install c++ boost libraries
       (http://www.boost.org/, tested version was boost_1_42_0, compile with flag '-fPIC')

    b) download and unzip c++ Eureqa api 
       (http://code.google.com/p/eureqa-api/downloads/list)

    c) download Eureqa server (http://creativemachines.cornell.edu/eureqa_download)
       
    d) download and unzip eureqa_select.zip
       (containing eureqa_select.cpp and Makefile)

    e) adjust paths in Makefile: 
       EUREQA_INCLUDE_PATH = <your path to c++ Eureqa api>
       BOOST_INCLUDE_PATH = <your path to c++ boost include files>
       BOOST_LIBRARY_PATH = <your path to c++ boost library files>
       MHOME	= <your path to matlab>
       compile with make

    f) include directory with mex file in your matlab path

2.) MATLAB USAGE:

ATTENTION: You first have to run the Eureqa server (see 1c) on your machine!

[formula,allsolutions,fitnessScores] = eureqa_select(predictors,predictand,predstr, ...
                                         fitness,ksteps,verbose,operators)

output: 
formula ...... analytical formula (matlab syntax) found by Eureqa
allsolutions ....... a string of all the solutions found by the GP search, each solution is separated by ";;"
fitnessScores .... the corresponding fitness scores for each solution

mandatory input:
predictors ... double nxp predictor matrix (typically denoted as X)
predictand ... double nx1 predictand matrix (typically denoted as y)
predstr ...... string containing the matlab name of the predictor matrix
fitness ...... integer defining the fitness function to be used by Eureqa
               (http://code.google.com/p/eureqa-api/wiki/doc_fitness_types)
               (0-13, e.g. 0 for absolute error
                           1 for mean squared error
                           7 for Akaike information criterion
                           8 for Bayesian information criterion)
ksteps ....... number of search steps in Eureqa, i.e. search time in seconds

optional input:
verbose ...... integer flag for printing out search progression
               (either 0 or 1, default is 0 denoting no print out)
operators .... string containing the user-defined operators to be used by Eureqa
               (arbitrary order, spaces not necessary)
               correspond to building blocks of Eureqa 
               (http://code.google.com/p/eureqa-api/wiki/doc_building_blocks)
               e.g. '+ - *' for linear regression with simple interaction
                    '+ - * sin cos' as above with simple trigonometric terms
                    '+ - * sin cos log' as above with logarithmic transformation
                    '+ - * / ^' for nonlinear regression
                    '+ - * / ^ sin cos exp log sin cos abs tan' 
                                      with all user-defined operators
               (default is to use default building blocks of Eureqa as defined
                in http://code.google.com/p/eureqa-api/wiki/doc_building_blocks)   
*/

//#ifdef
 //#endif
 
#ifdef _WIN32
	#include "yvals.h"
	#if (_MSC_VER >= 1600)
		#define __STDC_UTF_16__
	#endif
#endif

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread/thread.hpp" 

#include "mex.h"
#include "eureqa/eureqa.h"


int maxloc(float b[],int n)
{
  int mloc;
  mloc=0;
  for(int c=1; c<n; c++)
    if(b[mloc]<b[c])
      mloc=c;
  return mloc;
}

std::string convertInt(int number)
{
   std::stringstream ss;
   ss << number;
   return ss.str();
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nrhs<5 || nrhs>9)
    mexErrMsgTxt("usage: [formula,select] = eureqa_select(predictors,predictand,predstr,fitness,ksteps,verbose,operators,host,port)");
  // minh: original code changed here!!!
  if (nlhs>3)
    mexErrMsgTxt("too many output arguments <eureqa_select>");
    
  int i,j,status;
  std::string msg;
  std::stringstream ss;
  mwSize charlength;
  size_t found;

  double *yin = mxGetPr(prhs[1]);
  long n1 = mxGetM(prhs[1]);
  long p = mxGetN(prhs[1]);
  if (p!=1)
    mexErrMsgTxt("predictand must be univariate <eureqa_select>");
  double *xin = mxGetPr(prhs[0]);
  long n2 = mxGetM(prhs[0]);
  p = mxGetN(prhs[0]);
  if (n1!=n2)
    mexErrMsgTxt("mismatch of predictors and predictand <eureqa_select>");
  double **xin2d = (double**) malloc(p*sizeof(double*));
  for(j=0;j<p;j++)
    xin2d[j] = xin + (long)j*n1;
  char *predstr;
  charlength = mxGetN(prhs[2])*sizeof(mxChar)+1;
  predstr = (char*) malloc(charlength);
  status = mxGetString(prhs[2],predstr,charlength);
  long fitfun = (long) *mxGetPr(prhs[3]);
  long ksteps = (long) *mxGetPr(prhs[4]);
  bool verbose = (nrhs>5 && !mxIsChar(prhs[5]));
   
  if (verbose)
  {
    n2 = (long) *mxGetPr(prhs[5]);
    if (n2!=0 && n2!=1)
      mexErrMsgTxt("verbose must be either 0 or 1 <eureqa_select>");
      verbose = (n2==1);
  }

  // host is a pointer
  char *host = "localhost";
  int port = eureqa::default_port_tcp;
  
  if(nrhs>7) {  
    charlength = mxGetN(prhs[7])*sizeof(mxChar)+1;
    host = (char*) malloc(charlength);
    // mxGetString returns an integer indicating success or not 
    status = mxGetString(prhs[7],host,charlength);
    // the port has to be an int
    port = (int) *mxGetPr(prhs[8]);
  } 
  
  // transfer input data
  eureqa::data_set data(n1,p+1);
  for (i=0;i<n1;i++)
    data(i,0) = yin[i];
  for (j=0;j<p;j++)
    for (i=0;i<n1;i++)
      data(i,j+1) = xin2d[j][i];
  free(xin2d);
  if (verbose)
  {
    mexPrintf("data set imported successfully\n");
    msg = data.summary();
    mexPrintf(msg.c_str());
    mexPrintf("\n\n");
  }
      
  // set default symbols
  data.set_default_symbols();

  // set search options
  eureqa::search_options options;
  std::string formula = "x0=f(x1";
  for (j=1;j<p;j++)
    formula = formula+",x"+convertInt(j+1);
  formula = formula+")";
  options.search_relationship_ = formula;
  options.fitness_metric_ = fitfun;
  if (nrhs>5 && mxIsChar(prhs[6]))
  {
    char *buildstr;
    charlength = mxGetN(prhs[6])*sizeof(mxChar)+1;
    buildstr = (char*) malloc(charlength);
    status = mxGetString(prhs[6],buildstr,charlength);
    ss << buildstr;
    msg = ss.str();
    options.building_blocks_.clear();
    options.building_blocks_.push_back("1.23");
    options.building_blocks_.push_back("x");
    found = msg.find("+");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("x+y");
    found = msg.find("-");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("x-y");
    found = msg.find("*");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("x*y"); 
    found = msg.find("/");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("x/y");
    found = msg.find("^");
    if (found!=std::string::npos)
    {
      options.building_blocks_.push_back("powc(x,1.2)");  
      options.building_blocks_.push_back("x^y");
    }
    found = msg.find("exp");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("exp(x)");
    found = msg.find("log");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("log(x)");
    found = msg.find("sin");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("sin(x)"); 
    found = msg.find("cos");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("cos(x)");    
    found = msg.find("abs");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("abs(x)"); 
    found = msg.find("tan");
    if (found!=std::string::npos)
      options.building_blocks_.push_back("tan(x)");
    free(buildstr);
  } 
  
  // connect to a eureqa server
  eureqa::connection conn;
  if (verbose)
    mexPrintf( "connecting to a eureqa server at \n");
  if (!conn.connect(host, port)) 
    mexErrMsgTxt("unable to connect to server, run eureqa_server first\n");
  else if (!conn.last_result()) 
  { 
    mexPrintf("connection made successfully, but ");
    mexPrintf("the server sent back an error message: ");
    std::cout << conn.last_result() << std::endl;
    mexErrMsgTxt("");
  }
  else if (verbose)
  {
    mexPrintf("connected to server successfully, and ");
    mexPrintf("the server sent back a success message: ");
    std::cout << conn.last_result() << std::endl;
    mexPrintf("\n\n");
  }
    
  // query the server's information
  eureqa::server_info serv;
  if (verbose)
    mexPrintf("querying the server systems information\n");
  if (!conn.query_server_info(serv))
    mexErrMsgTxt("unable to receive the server information");
  else if (verbose)
  {
    mexPrintf("received server information successfully: ");
    msg = serv.summary();
    mexPrintf(msg.c_str());
    mexPrintf("\n\n");
  }
    
  // send data set
  if (verbose)
    mexPrintf("sending the data set to the server\n");
  if (!conn.send_data_set(data))
    mexErrMsgTxt("unable to transfer the data set");
  else if (!conn.last_result())
  {
    mexPrintf("data set transferred successfully, but ");
    mexPrintf("the server sent back an error message: ");
    std::cout << conn.last_result() << std::endl;
    mexErrMsgTxt("");
  }
  else if (verbose)
  {
    mexPrintf("data set transferred successfully, and ");
    mexPrintf("the server sent back a success message: ");
    std::cout << conn.last_result() << std::endl;
    mexPrintf("\n\n");
  }
    
  // send options
  if (verbose)
  mexPrintf("sending search options to the server\n");
  if (!conn.send_options(options))
    mexErrMsgTxt("unable to transfer the search options");
  else if (!conn.last_result())
  {
    mexPrintf("search options transferred successfully, but ");
    mexPrintf("the server sent back an error message:" );
    std::cout << conn.last_result() << std::endl;
    mexErrMsgTxt("");
  }
  else if (verbose)
  {
    mexPrintf("search options transferred successfully, and ");
    mexPrintf("the server sent back a success message: ");
    std::cout << conn.last_result() << std::endl;
    mexPrintf("\n\n");
  }
    
  // start searching
  if (verbose)
    mexPrintf("telling server to start searching\n");
  if (!conn.start_search())
    mexErrMsgTxt("unable to send the start command");
  else if (!conn.last_result())
  {
    mexPrintf("start command sent successfully, but ");
    mexPrintf("the server sent back an error message: ");
    std::cout << conn.last_result() << std::endl;
    mexErrMsgTxt("");
  }
  else if (verbose)
  {
    mexPrintf("start command sent successfully, and ");
    mexPrintf("the server sent back a success message: ");
    std::cout << conn.last_result() << std::endl;
    mexPrintf("\n\n");
  }
  
  // monitor the search
  if (verbose)
    mexPrintf("monitoring the search progress\n");
  eureqa::search_progress progress; // receives the progress and new solutions
  eureqa::solution_frontier best_solutions; // filters out the best solutions

  // continue searching for kstep seconds
  for (long k=0;k<ksteps;k++)
  {	
    if (!conn.query_progress(progress))
    {  		
      mexPrintf("warning: connection to server lost\n");
      break;
    }
    if (verbose)
    {
      msg = progress.summary();
      mexPrintf(msg.c_str());
      mexPrintf("\n\n");
    }
    if (best_solutions.add(progress.solution_) && verbose)
    {
      mexPrintf("new solution found: ");
      std::cout << progress.solution_ << std::endl;
    }
    if (verbose)
    {
      msg = best_solutions.to_string();
      mexPrintf(msg.c_str());
      mexPrintf("\n\n");
    }
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }

  // evaluate overall best solution
  int nbest = best_solutions.size();
  // float eval[nbest];
  float* eval = new float[nbest];
  for (i=0;i<nbest;i++)
    eval[i] = best_solutions[i].fitness_;
  std::string eureqa_output = best_solutions[maxloc(eval,nbest)].text_;
  if (verbose)
  {
    mexPrintf("the selected solution is: ");
    mexPrintf("%s\n",eureqa_output.c_str());
  }
  
  // retrieve formula
  size_t eq_loc = eureqa_output.find("=");
  std::string function = eureqa_output.substr(eq_loc+1);
  std::string search = function;

  // get rid of "powc" syntax
  found = function.find("powc");
  while (found!=std::string::npos)
  {
    function.erase(found,4);
    found = function.find(",",found);
    if (function.substr(found+1,1)==" ")
      function.erase(found+1,1);
    function.replace(found,1,")^(");
    found = function.find("powc");
  }

  // look for predictors and adjust formula
  std::string pattern,matlabpattern;
  // long select[p],s=0;
  long s = 0;
  long* select = new long[p];
  for (j=0;j<p;j++)
  {
    pattern = "x"+convertInt(p-j);
    found = search.find(pattern);
    if (found!=std::string::npos)
    {
      select[s] = p-j;
      s++;
      while (found!=std::string::npos)
      {
        search.erase(found,pattern.length());
        found = search.find(pattern);
      }
    }
    found = function.find(pattern);
    if (found!=std::string::npos)
    {
      ss.str(std::string());
      ss << predstr;
      matlabpattern = ss.str();
      matlabpattern = matlabpattern+"(:,"+convertInt(p-j)+")";
      while (found!=std::string::npos)
      {
        function.replace(found,pattern.length(),matlabpattern);
        found = function.find(pattern);
      }
    }
  }           	

  // convert formula to syntax in matlab
  found = function.find("*");
  while (found!=std::string::npos)
  {
    function.replace(found,1,".*");
    found = function.find("*",found+2);
  }
  found = function.find("/");
  while (found!=std::string::npos)
  {
    function.replace(found,1,"./");
    found = function.find("/",found+2);
  }
  found = function.find("^");
  while (found!=std::string::npos)
  {
    function.replace(found,1,".^");
    found = function.find("^",found+2);
  } 

  // output formula and selected predictors
  plhs[0] = mxCreateString(function.c_str());
  
  if (nlhs==2)
  {
    // minh: original code modified here -- BEGIN
    std::string overig;

    for (i=0;i<nbest;i++) {
        overig = overig + best_solutions[i].text_ + ";;";
    }
      
    plhs[1] = mxCreateString(overig.c_str()); 
      
   }
  
    if (nlhs==3)
   {
    std::string overig;
    
    /* Create an mxArray for the output data */  
    plhs[2] = mxCreateDoubleMatrix(nbest,1,mxREAL);

    /* Create a pointer to the output data */
    double * data = mxGetPr(plhs[2]);
 
    
    for (i=0;i<nbest;i++) {
        overig = overig + best_solutions[i].text_ + ";;";
        data[i] = -1.0*best_solutions[i].fitness_; // best_solutions[j].fitness_ has the negative of the fitnesses
                                                                        // brackets here are offset operators
    }
    plhs[1] = mxCreateString(overig.c_str()); 
      
  }
  
  // return to matlab
  free(predstr);
  delete(select);
  delete(eval);
  return;
}
