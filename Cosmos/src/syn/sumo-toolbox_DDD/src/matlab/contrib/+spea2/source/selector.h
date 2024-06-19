/*========================================================================
  SELECTOR.H
  ========================================================================
  SPEA2 - Strength Pareto EA 2
  Implementation in C for the selector side.
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

#ifndef SELECTOR_H
#define SELECTOR_H

/*---------------------------| constants |-------------------------------*/
#define PISA_MAXDOUBLE 1E99  // Internal maximal value for double

/*----------------------------| structs |--------------------------------*/

typedef struct ind_st  /* an individual */
{
    int index;
    double *f; /* objective vector */
    double fitness;
} ind;

typedef struct pop_st  /* a population */
{
    int size;
    int maxsize;
    ind **ind_array;
} pop;

extern pop *pp_all;
extern pop *pp_sel;
extern int display;

/*---------| initialization function (in spea2_functions.c) |---------*/

void initialize(void);

/*--------| memory allocation functions (in spea2_functions.c) |------*/

void* chk_malloc(size_t size);
pop* create_pop(int size, int dim);
ind* create_ind(int dim);

void free_memory(void);
void free_pop(pop *pp);
void free_ind(ind *p_ind);

/*-----| functions implementing the selection (spea2_functions.c) |---*/

void selection();
void mergeOffspring();
void calcFitnesses();
void calcDistances();
int getNN(int index, int k);
double getNNd(int index, int k);
void environmentalSelection();
void truncate_nondominated();
void truncate_dominated();
void matingSelection();

int dominates(ind *p_ind_a, ind *p_ind_b);
int is_equal(ind *p_ind_a, ind *p_ind_b);
double calcDistance(ind *p_ind_a, ind *p_ind_b);

int read_ini();
int read_population(pop *pp, int size, int dim);
int read_var();

#endif // SELECTOR_H
