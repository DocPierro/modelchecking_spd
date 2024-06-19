/*========================================================================
  SPEA2.C
  ========================================================================
  SPEA2 - Strength Pareto EA 2
  Combined MATLAB - C implementation of the main algorithm loop
  ========================================================================
  MATLAB version of SPEA 2
  Hamburg Univeristy of Technology
  Institute of Control Systems
  revision by: Andrey Popov, andrey.popov@gmx.net
  ========================================================================
  PISA  (www.tik.ee.ethz.ch/pisa/)
  Computer Engineering (TIK) ETH Zurich
  author: Fabian Landis, flandis@ee.ethz.ch
  revision by: Stefan Bleuler, bleuler@tik.ee.ethz.ch
  ========================================================================
  last change: 30/03/2005
  ========================================================================
*/

/*========================================================================
                  MOGA-SPEA Copyright and Legal Statements

  IN NO EVENT SHALL THE TECHNICAL UNIVERSITY HAMBURG-HARBURG, INSTITUTE
  OF CONTROL ENGINEERING BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, 
  SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF
  THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE TECHNICAL UNIVERSITY 
  HAMBURG-HARBURG, INSTITUTE OF CONTROL ENGINEERING HAS BEEN ADVISED OF 
  THE POSSIBILITY OF SUCH DAMAGE.
  
  TECHNICAL UNIVERSITY HAMBURG-HARBURG, INSTITUTE OF CONTROL ENGINEERING,
  SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
  PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
  THE TECHNICAL UNIVERSITY HAMBURG-HARBURG, INSTITUTE OF CONTROL 
  ENGINEERING HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
  ENHANCEMENTS, OR MODIFICATIONS.
========================================================================
*/


/*========================================================================
                        PISA Copyright Statements

Copyright (c) 2002-2003 Swiss Federal Institute of Technology,
Computer Engineering and Networks Laboratory. All rights reserved.

IN NO EVENT SHALL THE SWISS FEDERAL INSTITUTE OF TECHNOLOGY, COMPUTER
ENGINEERING AND NETWORKS LABORATORY BE LIABLE TO ANY PARTY FOR DIRECT,
INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF
THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE SWISS
FEDERAL INSTITUTE OF TECHNOLOGY, COMPUTER ENGINEERING AND NETWORKS
LABORATORY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SWISS FEDERAL INSTITUTE OF TECHNOLOGY, COMPUTER ENGINEERING AND
NETWORKS LABORATORY, SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS
ON AN "AS IS" BASIS, AND THE SWISS FEDERAL INSTITUTE OF TECHNOLOGY,
COMPUTER ENGINEERING AND NETWORKS LABORATORY HAS NO OBLIGATION TO
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
========================================================================
*/

#include "mex.h"
#include <assert.h>

#include "variator.h"
#include "selector.h"




//########################################################################
//#########################  MAIN FUNCTIONs  #############################

int mainRun(void)
{
    int returncode; // storing the values that the state functions return
    
    srand(seed);    // Initialize seed

    //$$$$$$$$$$$$$$ Initial population $$$$$$$$$$$$$$$
    // Allocate memory
    current_population = (int *) malloc(alpha * sizeof(int)); 
    if (current_population == NULL) {
          errMSG( __FILE__, __LINE__, "SPEA out of memory");
          return(1);    // error during run
    }     
        
    returncode = generate_initial_pop();
    
    initialize();// create population sets for the solver
    
    returncode = read_ini();   // read ini file
    free(current_population); 
    if (returncode == 0)       // reading ini file successful
        selection();      // do selection

    //$$$$$$$$$$$$$ Variator - Selector loop $$$$$$$$$$$
    while(!is_finished()) // Until the maximum number of generations is not exceeded
    {
        //=====| Variator |=====
        returncode = variate_pop();
        if (returncode != 0) {
            free_memory();
            return(1);
        }
        //=====| Selector |=====
        returncode = read_var();  // read variated individuals
        free(current_population);
        if (returncode == 0)      
            selection();          // do selection
    }
    returncode = update_from_arc();// removes the individuals, that doesn't exist in the archive
    if (returncode == 0)
        arrange_results();
    clean_population();
    free_memory();
    return (0);
}





//###########################################################################
void mexFunction (int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[]) {
    int i;
    double *Options;
    
    
    //-------- working part -------
    if ( nlhs != 2 || nrhs != 3)
        mexErrMsgTxt("Incorrect function call! SPEA should be called like:\n\t[Genes,Fitness] = spea('funct_name',boundaries,options);");
    
    // Get the .M function name
    i = mxGetString(prhs[0],problem,mxGetN(prhs[0])+1);
    assert(i); // n=0 <=> correct

    // Determine the lower and upper variable boundaries
    // The number of boundaries determine the number of decision variables
    number_decision_variables = mxGetM(prhs[1]);
    LBound = mxGetPr(prhs[1]);
    UBound = LBound + number_decision_variables;
    
    /* ------- Read the Options settings -----*/
    Options = mxGetPr(prhs[2]);
    i = 0;
 
    maxgen = (int) Options[i++];    // maximum number of generations (stop criterion)
    assert(maxgen > 0);
    alpha  = (int) Options[i++];    // number of individuals in initial population
    assert(alpha > 0);
    mu     = (int) Options[i++];    // number of individuals selected as parents
    assert(mu > 0);
    lambda = (int) Options[i++];    // number of offspring individuals
    assert(lambda > 0);

    dimension = (int) Options[i++]; // number of objectives
    assert(dimension > 0);
    //assert(number_decision_variables >= dimension);
    
    seed   = (int) Options[i++];    // seed for random number generator
    tournament = (int) Options[i++];// parameter for tournament selection

    individual_mutation_probability      = Options[i++];
    individual_recombination_probability = Options[i++];
    variable_mutation_probability        = Options[i++];
    variable_swap_probability            = Options[i++];
    variable_recombination_probability   = Options[i++];
    eta_mutation                         = Options[i++];
    eta_recombination                    = Options[i++];
    display                              = (int) Options[i++];
    
    // Create an array for the output's data
    ResultGenes = (double *) mxMalloc(mu * number_decision_variables * sizeof(double));
    ResultFits  = (double *) mxMalloc(mu * dimension * sizeof(double));
    plhs[0] = mxCreateDoubleMatrix(mu, number_decision_variables, mxREAL);
    plhs[1] = mxCreateDoubleMatrix(mu, dimension, mxREAL);
    if (display)
        mexPrintf("\t%d",maxgen);

    
    // ------- Call the Main Loop -------- 
    mainRun();

    // Write the results    
    mxSetPr(plhs[0], ResultGenes);
    mxSetPr(plhs[1], ResultFits);
    if (display)
        mexPrintf("\n");
}
