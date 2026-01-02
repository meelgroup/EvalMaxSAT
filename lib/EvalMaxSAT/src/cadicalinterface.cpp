#include "cadicalinterface.h"
#include "../../evalmax_cadical/src/cadical.hpp"

MySolver::MySolver() {
    solver = new EvalMax_CaDiCaL::Solver();
}

MySolver::~MySolver() {
    delete (EvalMax_CaDiCaL::Solver*)solver;
}

void MySolver::add(int lit) {
    ((EvalMax_CaDiCaL::Solver*)solver)->add(lit);
}

int MySolver::solve() {
    return ((EvalMax_CaDiCaL::Solver*)solver)->solve();
}

void MySolver::assume(int lit) {
    ((EvalMax_CaDiCaL::Solver*)solver)->assume(lit);
}

bool MySolver::val(int lit) {
    return ((EvalMax_CaDiCaL::Solver*)solver)->val(lit);
}

void MySolver::simplify() {
    ((EvalMax_CaDiCaL::Solver*)solver)->simplify();
}

void MySolver::limit(const char* what, int limit) {
    ((EvalMax_CaDiCaL::Solver*)solver)->limit(what, limit);
}

void MySolver::setTimeout(double seconds) {
    ((EvalMax_CaDiCaL::Solver*)solver)->limit("decisions", (int)(seconds * 1000000));
}

std::vector<bool> MySolver::getSolution() {
    EvalMax_CaDiCaL::Solver* s = (EvalMax_CaDiCaL::Solver*)solver;
    return s->getSolution();
}

void MySolver::reset_assumptions() {
    ((EvalMax_CaDiCaL::Solver*)solver)->reset_assumptions();
}

bool MySolver::find_up_implicants(const std::vector<int> &assum, std::vector<int> &result) {
    EvalMax_CaDiCaL::Solver* s = (EvalMax_CaDiCaL::Solver*)solver;
    return s->find_up_implicants(assum, result);
}

bool MySolver::failed(int lit) {
    return ((EvalMax_CaDiCaL::Solver*)solver)->failed(lit);
}

Solver_cadical::Solver_cadical() : solver(new MySolver) {}
Solver_cadical::~Solver_cadical() { delete solver; }

