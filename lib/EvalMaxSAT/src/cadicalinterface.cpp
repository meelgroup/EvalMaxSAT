#include "cadicalinterface.h"
#include "cadical.hpp"

MySolver::MySolver() {
    solver = new CaDiCaL::Solver();
}

MySolver::~MySolver() {
    delete (CaDiCaL::Solver*)solver;
}

void MySolver::add(int lit) {
    ((CaDiCaL::Solver*)solver)->add(lit);
}

int MySolver::solve() {
    return ((CaDiCaL::Solver*)solver)->solve();
}

void MySolver::assume(int lit) {
    ((CaDiCaL::Solver*)solver)->assume(lit);
}

bool MySolver::val(int lit) {
    return ((CaDiCaL::Solver*)solver)->val(lit);
}

void MySolver::simplify() {
    ((CaDiCaL::Solver*)solver)->simplify();
}

void MySolver::limit(const char* what, int limit) {
    ((CaDiCaL::Solver*)solver)->limit(what, limit);
}

void MySolver::setTimeout(double seconds) {
    ((CaDiCaL::Solver*)solver)->limit("decisions", (int)(seconds * 1000000));
}

std::vector<bool> MySolver::getSolution() {
    CaDiCaL::Solver* s = (CaDiCaL::Solver*)solver;
    return s->getSolution();
}

void MySolver::reset_assumptions() {
    ((CaDiCaL::Solver*)solver)->reset_assumptions();
}

bool MySolver::find_up_implicants(const std::vector<int> &assum, std::vector<int> &result) {
    CaDiCaL::Solver* s = (CaDiCaL::Solver*)solver;
    return s->find_up_implicants(assum, result);
}

bool MySolver::failed(int lit) {
    return ((CaDiCaL::Solver*)solver)->failed(lit);
}

Solver_cadical::Solver_cadical() : solver(new MySolver) {}
Solver_cadical::~Solver_cadical() { delete solver; }

