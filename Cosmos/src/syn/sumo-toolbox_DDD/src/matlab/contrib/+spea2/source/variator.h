/*========================================================================
  SPEA2.H
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
  author: Marco Laumanns, laumanns@tik.ee.ethz.ch
  revision by: Stefan Bleuler, bleuler@tik.ee.ethz.ch
  ========================================================================
  last change: 30/03/2005
  ========================================================================
*/

#ifndef VARIATOR_H
#define VARIATOR_H

// Start with array of this size for global population
#define STANDARD_SIZE 32200  

// maximal length of filenames 
#define FILE_NAME_LENGTH 128  // change the value if you like

/*-----------------------| common parameters |---------------------------*/

extern double *ResultFits;    // Pointer to the matrix with the result fintess values
extern double *ResultGenes;   // Pointer to the matrix with the result genes

extern char problem[FILE_NAME_LENGTH];

extern double *LBound;  // lower bound of the genes
extern double *UBound;  // upper bound of the genes

extern int number_decision_variables;
extern int dimension; // number of objectives
extern int maxgen;     // maximum number of generations (stop criterion)

extern int display;

extern int alpha;   // number of individuals in initial population
extern int mu;      // number of individuals selected as parents
extern int lambda;  // number of offspring individuals

extern int seed;       // seed for random number generator

extern int tournament; // parameter for tournament selection

extern double individual_mutation_probability;
extern double individual_recombination_probability;
extern double variable_mutation_probability;
extern double variable_swap_probability;
extern double variable_recombination_probability;
extern double eta_mutation;
extern double eta_recombination;

extern int *current_population; // storing the IDs of the individuals



//#########################| individual |#############################

struct individual_t
{
     double *x;  /* decision variable vector */
     int n;      /* length of the bit_string */
     double *f;  /* objective vector */
};

typedef struct individual_t individual; 


//#########################| stack |##################################

/* stack structure used in global population for the free available ids
   between 0 and last_identity */
typedef struct stack_node_t
{
  int identity;
  struct stack_node_t *next;
} stack_node;

typedef struct stack_t
{
  stack_node *top;
  int size;
} stack;

typedef struct population_t
{
  int size; // size of the population
  individual **individual_array; // array of pointers to individuals
  int last_identity; // identity of last individual
  stack free_ids_stack; // stack for keeping freed ids with remove

} population;

// the only population we need is
extern population global_population;

//########################################################################
int eval(individual *p_ind);


/*-------------------| functions for individual struct |----------------*/
void free_individual(individual *ind);

double get_objective_value(int identity, int i); 

/*---------------------| Functions from the main loop |-----------------*/
int generate_initial_pop(void); 

int variate_pop(void);

int is_finished();


individual *new_individual();

individual *copy_individual(individual* ind);

int variate(int *parents, int *offspring);

int mutation(individual *ind);
int uniform_crossover(individual *ind1, individual *ind2);
int sbx(individual *ind1, individual *ind2);

int irand(int range);
double drand(double range);

void arrange_results();

int free_stack(stack *st);
int push(stack *st, int id);
int pop_fct(stack *st);

int clean_population(void);
int remove_individual(int identity);

int cmp_int(const void *p_i1, const void *p_i2);

int add_individual(individual *ind);

individual *get_individual(int identity);

int get_first(void);

int get_next(int identity);

int update_from_arc(void);

void errMSG(char *infile, int linenumber, char *string);

#endif // VARIATOR_H
