#include "cadical.hpp"

#include <cstdlib>
#include <cstring>

namespace EvalMax_CaDiCaL {

struct Wrapper : Learner, Terminator {

  Solver * solver;
  struct {
    void * state;
    int (*function) (void *);
  } terminator;

  struct {
    void * state;
    int max_length;
    int * begin_clause, * end_clause, * capacity_clause;
    void (*function) (void *, int *);
  } learner;

  bool terminate () {
    if (!terminator.function)
      return false;
    return terminator.function (terminator.state);
  }

  bool learning (int size) {
    if (!learner.function)
      return false;
    return size <= learner.max_length;
  }

  void learn (int lit) {
    if (learner.end_clause == learner.capacity_clause) {
      size_t count = learner.end_clause - learner.begin_clause;
      size_t size = count ? 2*count : 1;
      learner.begin_clause = (int*)
        realloc (learner.begin_clause, size * sizeof (int));
      learner.end_clause = learner.begin_clause + count;
      learner.capacity_clause = learner.begin_clause + size;
    }
    *learner.end_clause++ = lit;
    if (lit)
      return;
    learner.function (learner.state, learner.begin_clause);
    learner.end_clause = learner.begin_clause;
  }

  Wrapper () : solver (new Solver ()) {
    memset (&terminator, 0, sizeof terminator);
    memset (&learner, 0, sizeof learner);
  }

  ~Wrapper () {
    terminator.function = 0;
    if (learner.begin_clause) free (learner.begin_clause);
    delete solver; }
};

}

using namespace EvalMax_CaDiCaL;

extern "C" {

#include "ccadical.h"

const char * ccadical_signature (void) {
  return Solver::signature ();
}

CEvalMax_CaDiCaL * ccadical_init (void) {
  return (CEvalMax_CaDiCaL*) new Wrapper ();
}

void ccadical_release (CEvalMax_CaDiCaL * wrapper) {
  delete (Wrapper*) wrapper;
}

void ccadical_constrain (CEvalMax_CaDiCaL *wrapper, int lit){
  ((Wrapper*) wrapper)->solver->constrain (lit);
}

int ccadical_constraint_failed (CEvalMax_CaDiCaL * wrapper) {
  return ((Wrapper*) wrapper)->solver->constraint_failed ();
}

void ccadical_set_option (CEvalMax_CaDiCaL * wrapper,
                          const char * name, int val) {
  ((Wrapper*) wrapper)->solver->set (name, val);
}

void ccadical_limit (CEvalMax_CaDiCaL * wrapper,
                     const char * name, int val) {
  ((Wrapper*) wrapper)->solver->limit (name, val);
}

int ccadical_get_option (CEvalMax_CaDiCaL * wrapper, const char * name) {
  return ((Wrapper*) wrapper)->solver->get (name);
}

void ccadical_add (CEvalMax_CaDiCaL * wrapper, int lit) {
  ((Wrapper*) wrapper)->solver->add (lit);
}

void ccadical_assume (CEvalMax_CaDiCaL * wrapper, int lit) {
  ((Wrapper*) wrapper)->solver->assume (lit);
}

int ccadical_solve (CEvalMax_CaDiCaL * wrapper) {
  return ((Wrapper*) wrapper)->solver->solve ();
}

int ccadical_simplify (CEvalMax_CaDiCaL * wrapper) {
  return ((Wrapper*) wrapper)->solver->simplify ();
}

int ccadical_val (CEvalMax_CaDiCaL * wrapper, int lit) {
  return ((Wrapper*) wrapper)->solver->val (lit);
}

int ccadical_failed (CEvalMax_CaDiCaL * wrapper, int lit) {
  return ((Wrapper*) wrapper)->solver->failed (lit);
}

void ccadical_print_statistics (CEvalMax_CaDiCaL * wrapper) {
  ((Wrapper*) wrapper)->solver->statistics ();
}

void ccadical_terminate (CEvalMax_CaDiCaL * wrapper) {
  ((Wrapper*) wrapper)->solver->terminate ();
}

int64_t ccadical_active (CEvalMax_CaDiCaL * wrapper) {
  return ((Wrapper*) wrapper)->solver->active ();
}

int64_t ccadical_irredundant (CEvalMax_CaDiCaL * wrapper) {
  return ((Wrapper*) wrapper)->solver->irredundant ();
}

int ccadical_fixed (CEvalMax_CaDiCaL * wrapper, int lit) {
  return ((Wrapper*) wrapper)->solver->fixed (lit);
}

void ccadical_set_terminate (CEvalMax_CaDiCaL * ptr,
                             void * state, int (*terminate)(void *)) {
  Wrapper * wrapper = (Wrapper *) ptr;
  wrapper->terminator.state = state;
  wrapper->terminator.function = terminate;
  if (terminate) wrapper->solver->connect_terminator (wrapper);
  else wrapper->solver->disconnect_terminator ();
}

void ccadical_set_learn (CEvalMax_CaDiCaL * ptr,
                         void * state, int max_length,
                         void (*learn)(void * state, int * clause)) {
  Wrapper * wrapper = (Wrapper *) ptr;
  wrapper->learner.state = state;
  wrapper->learner.max_length = max_length;
  wrapper->learner.function = learn;
  if (learn) wrapper->solver->connect_learner (wrapper);
  else wrapper->solver->disconnect_learner ();
}

void ccadical_freeze (CEvalMax_CaDiCaL * ptr, int lit) {
  ((Wrapper*) ptr)->solver->freeze (lit);
}

void ccadical_melt (CEvalMax_CaDiCaL * ptr, int lit) {
  ((Wrapper*) ptr)->solver->melt (lit);
}

int ccadical_frozen (CEvalMax_CaDiCaL * ptr, int lit) {
  return ((Wrapper*) ptr)->solver->frozen (lit);
}

}
