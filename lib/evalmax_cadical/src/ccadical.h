#ifndef _ccadical_h_INCLUDED
#define _ccadical_h_INCLUDED

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------*/

#include <stdint.h>

// C wrapper for EvalMax_CaDiCaL's C++ API following IPASIR.

typedef struct CEvalMax_CaDiCaL CEvalMax_CaDiCaL;

const char * ccadical_signature (void);
CEvalMax_CaDiCaL * ccadical_init (void);
void ccadical_release (CEvalMax_CaDiCaL *);

void ccadical_add (CEvalMax_CaDiCaL *, int lit);
void ccadical_assume (CEvalMax_CaDiCaL *, int lit);
int ccadical_solve (CEvalMax_CaDiCaL *);
int ccadical_val (CEvalMax_CaDiCaL *, int lit);
int ccadical_failed (CEvalMax_CaDiCaL *, int lit);

void ccadical_set_terminate (CEvalMax_CaDiCaL *,
  void * state, int (*terminate)(void * state));

void ccadical_set_learn (CEvalMax_CaDiCaL *,
  void * state, int max_length, void (*learn)(void * state, int * clause));

/*------------------------------------------------------------------------*/

// Non-IPASIR conformant 'C' functions.

void ccadical_constrain (CEvalMax_CaDiCaL *, int lit);
int ccadical_constraint_failed (CEvalMax_CaDiCaL *);
void ccadical_set_option (CEvalMax_CaDiCaL *, const char * name, int val);
void ccadical_limit (CEvalMax_CaDiCaL *, const char * name, int limit);
int ccadical_get_option (CEvalMax_CaDiCaL *, const char * name);
void ccadical_print_statistics (CEvalMax_CaDiCaL *);
int64_t ccadical_active (CEvalMax_CaDiCaL *);
int64_t ccadical_irredundant (CEvalMax_CaDiCaL *);
int ccadical_fixed (CEvalMax_CaDiCaL *, int lit);
void ccadical_terminate (CEvalMax_CaDiCaL *);
void ccadical_freeze (CEvalMax_CaDiCaL *, int lit);
int ccadical_frozen (CEvalMax_CaDiCaL *, int lit);
void ccadical_melt (CEvalMax_CaDiCaL *, int lit);
int ccadical_simplify (CEvalMax_CaDiCaL *);

/*------------------------------------------------------------------------*/

// Support legacy names used before moving to more IPASIR conforming names.

#define ccadical_reset ccadical_release
#define ccadical_sat ccadical_solve
#define ccadical_deref ccadical_val

/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/

#endif
