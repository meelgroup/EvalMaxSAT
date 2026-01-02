#include "ipasir.h"
#include "ccadical.h"

extern "C" {

const char * ipasir_signature () {
  return ccadical_signature ();
}

void * ipasir_init () {
  return ccadical_init (); }

void ipasir_release (void * solver) {
  ccadical_release ((CEvalMax_CaDiCaL*) solver);
}

void ipasir_add (void * solver, int lit) {
  ccadical_add ((CEvalMax_CaDiCaL *) solver, lit);
}

void ipasir_assume (void * solver, int lit) {
  ccadical_assume ((CEvalMax_CaDiCaL *) solver, lit);
}

int ipasir_solve (void * solver) {
  return ccadical_solve ((CEvalMax_CaDiCaL *) solver);
}

int ipasir_val (void * solver, int lit) {
  return ccadical_val ((CEvalMax_CaDiCaL *) solver, lit);
}

int ipasir_failed (void * solver, int lit) {
  return ccadical_failed ((CEvalMax_CaDiCaL *) solver, lit);
}

void ipasir_set_terminate (void * solver,
                           void * state, int (*terminate)(void * state)) {
  ccadical_set_terminate ((CEvalMax_CaDiCaL *) solver, state, terminate);
}

void ipasir_set_learn (void * solver,
                         void * state, int max_length,
                         void (*learn)(void * state, int * clause)) {
  ccadical_set_learn ((CEvalMax_CaDiCaL*) solver, state, max_length, learn);
}

}
