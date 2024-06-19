/*========================================================================
  VARIATOR.C
  ========================================================================
  MATLAB version of SPEA 2
  Hamburg Univeristy of Technology
  Institute of Control Systems
  revision by: Andrey Popov, andrey.popov@gmx.net
  ========================================================================
  SPEA2 - Strength Pareto EA 2
  Functions for variating the population
  ========================================================================
  PISA  (www.tik.ee.ethz.ch/pisa/)
  Computer Engineering (TIK) ETH Zurich
  ========================================================================
*/
#include "mex.h"
#include <assert.h>
#include <math.h>

#include "selector.h"
#include "variator.h"

/*--------------------| global variable definitions |-------------------*/
/*         declared in variator.h used in other files as well           */

double *ResultFits;    // Pointer to the matrix with the result fintess values
double *ResultGenes;   // Pointer to the matrix with the result genes

char problem[FILE_NAME_LENGTH]; // name of the .M function, calculating the cost functions

double *LBound; // lower bound of the variables
double *UBound; // upper bound of the variables

int number_decision_variables;
int dimension;  // number of objectives
int maxgen;     // maximum number of generations (stop criterion)

int display;    // display the remianing iter. numbers

int alpha;      // number of individuals in initial population
int mu;         // number of individuals selected as parents
int lambda;     // number of offspring individuals

int seed;       // seed for random number generator

int tournament; // parameter for tournament selection

double individual_mutation_probability;
double individual_recombination_probability;
double variable_mutation_probability;
double variable_swap_probability;
double variable_recombination_probability;
double eta_mutation;
double eta_recombination;

int *current_population; // storing the IDs of the individuals

int current_max_size; 
// starting array size of the individuals array in global_population

population global_population; // pool of all existing individuals
int gen; // current generation





//#########################################################################
/* Determines the objective value of the minimized functions */
// Calulcations in MATLAB enviroment
int eval(individual *ind)      
{
    int i=0;
    
    // Pointer Left(Right) Hand Side (used for the call of a MATLAB function)
    mxArray *plhs1[1], *prhs1[1];
	double *pnter;
    
	// create the vectors with the decision variables 1xdecision_variables
    // and with the calculated cost functions 1xdimension
	plhs1[0] = mxCreateDoubleMatrix(1,dimension,mxREAL);
	prhs1[0] = mxCreateDoubleMatrix(1,number_decision_variables,mxREAL);

	// set the value of the input vector
	pnter = mxGetPr(prhs1[0]);
    for (i=0; i < number_decision_variables; i++)
        pnter[i] = (ind->x[i]);
	
    // calls function in MATLAB
	mexCallMATLAB(1,plhs1,1,prhs1,problem);  // problem contains the name of user defined M file
	
    // assign the values returned from MATLAB as individual's cost functions
	pnter = mxGetPr(plhs1[0]);
    for (i=0; i < dimension; i++)
        ind->f[i] = pnter[i];
	
    // chear the memory
	mxDestroyArray(plhs1[0]);
	mxDestroyArray(prhs1[0]);
	
	return(0);
}

//#########################################################################
//####################|  Fucntions for individuals  |######################

individual *new_individual()
/* create a random new individual and allocate memory for it,
   returns a pointer to the new individual */
{
     individual *return_ind;
     int i;

     return_ind = (individual *) malloc(sizeof(individual));
     return_ind->x = (double *) malloc(sizeof(double) * number_decision_variables);
     return_ind->f = (double *) malloc(sizeof(double) * dimension);
 
     for (i = 0; i < number_decision_variables; i++)
     {
        //& return_ind->x[i] = drand(1.0);
         return_ind->x[i] = LBound[i] + drand(UBound[i]-LBound[i]);
     }
     
     return_ind->n = number_decision_variables;

     for (i = 0; i < dimension; i++)
     {
        return_ind->f[i] = 0;
     }
     
     return (return_ind);
}

individual *copy_individual(individual *ind)
/* copy an individual and return the pointer to it */
{
     individual *return_ind;
     int i;

     return_ind = (individual *) malloc(sizeof(individual));
     return_ind->x = (double *)malloc(sizeof(double) * number_decision_variables);
     return_ind->f = (double *) malloc(sizeof(double) * dimension);
     
     for (i = 0; i < number_decision_variables; i++)
          return_ind->x[i] = ind->x[i];

     for (i = 0; i < dimension; i++)
	  return_ind->f[i] = ind->f[i];

     return_ind->n = ind->n;

     return(return_ind);
}


void free_individual(individual *ind) 
// Frees the memory for one indiviual.
{
     if (ind == NULL)
          return;
     
     free(ind->x);
     free(ind->f);
     
     free(ind);
}

double get_objective_value(int identity, int i)
/* Gets objective value of an individual.

   pre: 0 <= i <= dimension - 1 (dimension is the number of objectives)

   post: Return value == the objective value number 'i' in individual '*ind'.
         If no individual with ID 'identity' return value == -1. 
*/   
{
     individual *temp_ind;
     
     double objective_value = -1.0;

     assert(0 <= i && i < dimension); //
    
     if (i < 0 || i > (dimension - 1))
	  return(-1);
     
     temp_ind = get_individual(identity);
     if (temp_ind == NULL)
	 return(-1);
     
     objective_value = temp_ind->f[i];
  
     return (objective_value);
}

//########################################################################
int is_finished()
/* Tests if ending criterion of your algorithm applies.
   Return value == 1 if optimization should stop
                == 0 if optimization should continue
*/
{
    if (display) {
        if (gen%20==0) 
            mexPrintf("\n");
        mexPrintf("\t%d",maxgen-gen);
    }
    return (gen >= maxgen);
}

//########################################################################
//###################|  Functions from the main loop  |####################

int generate_initial_pop() 
/* Generates Initial Population and Calulates the Function values for it.
   The global variable 'alpha' contains the number of indiviuals that need
   to be generated.
   Initial population created.
   Return value == 0 if successful,
                == 1 if unspecified errors happened,
*/
{
     int i;
     int min_valid, max_valid;
     
     // create first alpha individuals
     for(i = 0; i < alpha; i++)
     {
        individual *ind = new_individual(); // Generate random individuals
     
        eval(ind);      // Calculate the Functions values
        
        current_population[i] = add_individual(ind);
        if(current_population[i] == -1)
               return(1);
     } 

     gen = 1;           // set the generations counter

     min_valid = 0;
     max_valid = global_population.last_identity;
     for(i = 0; i < alpha; i++)
     {
          if (current_population[i] < min_valid || current_population[i] > max_valid 
              || global_population.individual_array[current_population[i]] == NULL) 
          {
               errMSG( __FILE__, __LINE__,"bad id, checked in generate_initial_pop");
               return (1);
          } 
     }

     return (0);
}

int variate_pop()
/* Variates the population and caluclated the function values for the 
   offspring population
   The global variable 'mu' contains the number of indiviuals in
   the parent population.
   The global variable 'lambda' contains the number of individuals
   you need to create by variation. 
        
   Calls update_from_arc() in order to delete old uncessary
   individuals from the global population.
   'lambda' children generated from the 'mu' parents
   Children added to the global population using add_individual().
   Return value == 0 if successful,
                == 1 if unspecified errors happened,
*/
{
     int i;
     int result; // stores return values of called functions
     int min_valid, max_valid;
     
     current_population = (int *) malloc(lambda * sizeof(int)); 
     if (current_population == NULL)
     {
          errMSG( __FILE__, __LINE__, "SPEA variator: out of memory");
          return (1);
     }

     result = update_from_arc(); 
     if (result != 0) 
          return (1);

     result = variate(pp_sel, current_population);
     if (result != 0)
          return (1);
          
     gen++;         // update the counter

     min_valid = 0;
     max_valid = global_population.last_identity;
     for(i = 0; i < lambda; i++)
     {
          if (current_population[i] < min_valid || current_population[i] > max_valid 
              || global_population.individual_array[current_population[i]] == NULL) 
          {
               errMSG( __FILE__, __LINE__,"bad ID, checked in variate_pop");
               return(1);
          } 
     }     
     return (0);
}
 

int variate(int *selected, int *result_ids)
// Performs the real variation of individuals
// *selected points to the selected individuals
// *result_ids will contain the offspring individuals
{
     int result, i, k;
     result = 1;

     // copying all individuals from selected
     for(i = 0; i < mu; i++)
     {
          result_ids[i] = 
               add_individual(copy_individual(get_individual(pp_sel->ind_array[i]->index)));//selected[i]
          if(result_ids[i] == -1)
          {
               errMSG( __FILE__, __LINE__,"copying + adding failed");
               return (1);
          }
     }
     // if odd number of individuals, last one is left as is
     if((((double)mu/2) - (int)(mu/2)) != 0) k = mu - 1; 
     else k = mu;

     // do recombination
     for(i = 0; i < k; i+= 2)
     {  
	  if (drand(1) <= variable_swap_probability)
	  {
	      result = uniform_crossover(get_individual(result_ids[i]),get_individual(result_ids[i + 1]));
	      if (result != 0)
		  errMSG( __FILE__, __LINE__, "recombination failed!");
	  }
	  
	  if (drand(1) <= individual_recombination_probability)
	  {
	      result = sbx(get_individual(result_ids[i]), get_individual(result_ids[i + 1]));
	      if (result != 0)
		  errMSG( __FILE__, __LINE__, "recombination failed!");
	  }
	  
     }
    // do mutation
     for(i = 0; i < mu; i++)
     {
        if (drand(1) <= individual_mutation_probability)
        { 
            result = mutation(get_individual(result_ids[i]));
            if(result != 0)
                errMSG( __FILE__, __LINE__, "mutation failed!");
        }  
     }
     // do evaluation
     for(i = 0; i < mu; i++)
     {
	 int result;
	 result = eval(get_individual(result_ids[i]));
     }
     return (0);
}

//####################################
int mutation(individual *ind)
{
     int i;

     if (ind == NULL)
     {
	 return (1);
     }
     
     for (i = 0; i < ind->n; i++)
     {
	 if (drand(1) <= variable_mutation_probability)
	 {
	     double eta = eta_mutation;
	     double u = drand(1.0);
	     double delta = 0;
	     double x = ind->x[i];
	     double lb = LBound[i];    // lower bound of variable i
	     double ub = UBound[i];    // upper bound of variable i
	     double diff = ub - lb;    // range of variable i
	     double maxmut0 = x - lb;
	     double maxmut = ub - x;
	     double delta_max = maxmut0 / diff;
	     if (maxmut0 > maxmut)
	     {
		 delta_max = maxmut / diff;
	     }
	     
	     if (u < 0.5)
	     {
		 double b =  2*u + (1-2*u)*(pow(1-delta_max,(eta+1)));
		 delta = pow(b,(1.0/(eta+1))) - 1.0;
	     }
	     else
	     {
		 double b = 2*(1-u) + 2*(u-0.5)*(pow(1-delta_max,(eta+1)));
		 delta = 1.0 - pow(b,(1.0/(eta+1)));
	     }
	     if (delta > delta_max)  // machine accuracy problem
		 delta = delta_max;
	     else if (delta < -delta_max)
		 delta = -delta_max;
	     
	     ind->x[i] = x + delta * diff;
	 }
     }
     
     return (0);
}

//####################################
int uniform_crossover(individual *ind1, individual *ind2)
{
     int i;
   
     for (i = 0; i < ind2->n; i++)
     {
	 if (irand(2) == 1) // switch variable
	 {
	     double x = ind2->x[i];
	     ind2->x[i] = ind1->x[i];
	     ind1->x[i] = x;
          } 
     }  
     return (0);
}

//####################################
// Simulated binary crossover
int sbx(individual *ind1, individual *ind2)
{
     int i;
   
     for (i = 0; i < ind2->n; i++)
     {
	 if (drand(1) <= individual_recombination_probability)  
	 {
	     double di = eta_recombination; // distribution index
	     int bounded = 1;
	     double lb = LBound[i];    // lower bound of variable i
	     double ub = UBound[i];    // upper bound of variable i
	     double u = drand(1);
	     double b0=0, b1=0;   // spread factors
	     double x0 = ind1->x[i];
	     double x1 = ind2->x[i];
	     double bl=0, bu=0, p_bl=0, p_bu=0, bll=0, buu=0, blll=0, buuu=0;
	     double dx = 0;
	     double u0=0, u1=0;
             
            // calculate spread factor(s) b0, b1
            if (bounded == 1)
            {
                dx = fabs(x1-x0);   // difference of x values
                if (dx > 0)
                {
                    bl = (x0 + x1 - 2*lb) / dx;
                    bu = (2*ub - x0 - x1) / dx;
                    bll = (x0 + x1 - 2*(x0-lb)) / dx;
                    buu = (2*(ub-x1)-x0-x1) / dx;
                    if (x0 < x1)
                    {
                        blll = 1 + 2 * (x0 - lb) / dx;
                        buuu = 1 + 2 * (ub - x1) / dx;
                    }
                    else
                    {
                        blll = 1 + 2 * (x1 - lb) / dx;
                        buuu = 1 + 2 * (ub-x0) / dx;
		    }
		    
		    bl = blll; // take Deb's version (numerically stable)
                    bu = buuu;
                    if (bl < bu)  // symmetric distribution, like Deb
                        bu = bl;
                    else
                        bl = bu;
                    assert(bl > 0 && bu > 0);
                    p_bl = 1 - 1/(2*pow(bl,di+1));
                    p_bu = 1 - 1/(2*pow(bu,di+1));
		}
                else
                {
                    p_bl = 1;
                    p_bu = 1;
                }
                u0 = u*p_bl;
                u1 = u*p_bu;
                if (u0<=0.5)
                    b0 = pow(2*u0,1/(di+1));
                else
                    b0 = pow(0.5/(1-u0),1/(di+1));
                if (u1<=0.5)
                    b1 = pow(2*u1,1/(di+1));
                else
                    b1 = pow(0.5/(1-u1),1/(di+1));
                assert(dx==0 || (b0<=bl && b1<=bu)); // machine accuracy
            }
            else
            {
                if (u<=0.5)
                    b0 = pow(2*u,1/(di+1));
                else
                    b0 = pow(0.5/(1-u),1/(di+1));
                b1 = b0;
            }
            assert (b0 == b1);
            if (x0 < x1)
            {
                ind1->x[i] = 0.5*(x0+x1 + b0*(x0-x1));
                ind2->x[i] = 0.5*(x0+x1 + b1*(x1-x0));
            }
            else
            {
                ind1->x[i] = 0.5*(x0+x1 + b1*(x0-x1));
                ind2->x[i] = 0.5*(x0+x1 + b0*(x1-x0));
            }
	 }
     }  
     
     return (0);
}


/* Generate a random integer. */
int irand(int range)
{
     int j;
     j=(int) ((double) range * (double) rand() / (RAND_MAX + 1.0));
     return (j);
}


/* Generate a random double. */
double drand(double range)
{
     double j;
     j=(range * (double) rand() / (RAND_MAX + 1.0));
     return (j);
}


//#############################################################
void arrange_results()
// Arranges the obtained results in ResultGenes and ResultFits matrixes
// that are returned from MATLAB
{
     int j, current_id;
     int i=0;
     individual *temp;
     
     current_id = get_first();

     while (current_id != -1)
     {      
          temp = get_individual(current_id);
          for (j = 0; j < dimension; j++) {
               ResultFits[i+j*mu] = temp->f[j]; 
          }
               // mu*j, because MATLAB data are arranged by columns, not rows
          
          for (j = 0; j < temp->n; j++)
               ResultGenes[i+j*mu] = temp->x[j];

          current_id = get_next(current_id);
          i++;
     }
}


//########################|  stack functions  |############################

int free_stack(stack *st)
{
     while(pop_fct(st) != -1);
          // don't do anything, pop_fct() does it already.
     return (0);
}

int push(stack *st,int id)
{
     stack_node *new_el = (stack_node *) malloc(sizeof(stack_node));
     if (new_el == NULL)
          return(1);
     new_el->next = st->top;
     new_el->identity = id;
     st->top = new_el;
     st->size++;
     return(0);
}

int pop_fct(stack *st)
{
     int identity;
     stack_node *next;
     if(st->size == 0)
          return(-1);
     identity = st->top->identity;
     next = st->top->next;
     free(st->top);
     st->top = next;
     st->size--;
     return(identity);
}


int cmp_int(const void *p_i1, const void *p_i2)
/* Compares the two integers '*p_i1' and '*p_i2'.
   Returns 0 if *p_i1 == *p_i2.
   Returns 1 if *p_i1 > *p_i2.
   Returns -1 if *p_i1 < *p_i2. */
{
     int i1 = *((int *)p_i1);
     int i2 = *((int *)p_i2);
     
     if(i1 == i2)
          return (0);

     if(i1 > i2)
          return (1);
     else
          return (-1);
}

//#########################################################################
//####################| global population functions |######################

int remove_individual(int identity)
/* Removes the individual with ID 'identity' from the global population. */
{
     individual *temp;
     int result;
     if((identity > global_population.last_identity) || (identity < 0))
          return (1);
     temp = get_individual(identity);
     if(temp == NULL)
          return (1);

     global_population.individual_array[identity] = NULL;

     if(identity == global_population.last_identity)
     {
          global_population.last_identity--;
     }
     else
     {
          result = push(&global_population.free_ids_stack, identity);
          if (result == 1)
          {
               errMSG( __FILE__, __LINE__, "Pushing a free identity to stack failed.");
               return (1);
          }
     }

     global_population.size--;

     free_individual(temp);
     
     return (0);
}

int clean_population()
/* Frees memory for all individuals in population and for the global
   population itself. */
{
     int current_id;
     current_id = get_first();
     while(current_id != -1)
     {
          remove_individual(current_id);
          current_id = get_next(current_id);
     }

     free_stack(&global_population.free_ids_stack);
     free(global_population.individual_array);
     
     return (0);
}

int add_individual(individual *ind)
/* Takes a pointer to an individual and adds the individual to the
   global population.
   Returns the identity assigned to the individual.
   Returns -1 if adding failed.*/
{
     int i;
     int identity = -1;
     individual **tmp; // in case we need to double array size

     if (ind == NULL) // there is no individual to add
          return (-1);
     
     // if size == 0 we need to allocate memory for our population
     if(global_population.size == 0)
     {
          current_max_size = STANDARD_SIZE;
          global_population.individual_array =
               (individual **) malloc(current_max_size * sizeof(int));
          if (global_population.individual_array == NULL)
          {
                errMSG( __FILE__, __LINE__, "SPEA variator: out of memory");
                return (-1);
          }
          global_population.last_identity = -1;
     }
  
     // search for free id
     identity = pop_fct(&global_population.free_ids_stack);
     if (identity == -1)
     {
          identity = global_population.last_identity + 1;
          global_population.last_identity++;
     } 

     global_population.size++;

     if (global_population.last_identity < current_max_size)
     {
          global_population.individual_array[identity] = ind;    
     }
     else // enlargement of individual array (size doubling)
     { 
          tmp = (individual **) malloc(sizeof(int) * current_max_size * 2);
          if (tmp == NULL)
          {
               errMSG( __FILE__, __LINE__, "SPEA variator out of memory");
               return (-1);
          }
          /* copy old array */
          for (i = 0; i < current_max_size; i++)
               tmp[i] = global_population.individual_array[i];
          current_max_size = current_max_size * 2;
          /* free memory */
          free(global_population.individual_array);
          global_population.individual_array = tmp;
     }

     return (identity);
}


individual *get_individual(int identity) 
// Returns a pointer to the individual corresponding to 'identity'.
{
     if((identity > global_population.last_identity) || (identity < 0))
          return (NULL);
     return (global_population.individual_array[identity]);
}


int get_first(void)
// Returns the identity of first individual in the global population.
{
     return (get_next(-1));
}


int get_next(int identity) 
/* Takes an identity an returns the identity of the next following
   individual in the global population. */
{ 
     int next_id;

     if((identity < -1) || (identity > global_population.last_identity))
          return (-1);
  
     next_id = identity + 1;
     while(next_id <= global_population.last_identity) {
          if(global_population.individual_array[next_id] != NULL)
               return (next_id);
          next_id++;
     }

     return(-1);
}

//#############################################################
int update_from_arc(void)
// Removes the individuals, that are not in the archive
{
     int size, result; 
     int *keep;
     int i, current;

     size = pp_all->size;

     result = 0;
     keep = (int *) malloc(sizeof(int)*size);

     if (keep == NULL)
     {
          errMSG( __FILE__, __LINE__, "SPEA variator: out of memory");
          return (1);
     }  

     for(i=0; i < size; i++)
         keep[i] = pp_all->ind_array[i]->index;

     // sort the array of indexes to keep,
     // so we can go through the array and delete all indexes,
     // that are in between
     qsort(keep, (size_t) size, sizeof(int), cmp_int);      
    
     // delete all indexes in global_population not found in keep array
     current = get_first();
     for(i = 0; i < size; i++)
     {
          while(current < keep[i])
          {
               result = remove_individual(current);
               if (result != 0)
                    return(1);
               current = get_next(current);
          }
          if (current == keep[i])
          {
               current = get_next(current);
          } // this one we keep
          else  // current must be bigger than keep[i],
                // something went wrong...
          { 
              
               errMSG( __FILE__, __LINE__,"identity in archive is not in the global population!");
               return (1);
          }
     }

     // delete the last individuals at end of list
     while(current != -1)
     {
          result = remove_individual(current);
          if (result != 0) 
               return (1);
          current = get_next(current);
     } 
  
     free(keep);
     return (0);
}


//#############################################################
void errMSG(char *infile, int linenumber, char *string)
{
    mexPrintf("Error while running SPEA2\nFile:%s \tLine:%d\t%s\n",infile,linenumber,string);
    mexErrMsgTxt("If you continue receiving this massage, please inform the authors for the problem.\n");
}
