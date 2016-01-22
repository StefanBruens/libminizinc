/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
 *  Main authors:
 *     Guido Tack <guido.tack@monash.edu>
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <minizinc/solver_instance_base.hh>

namespace MiniZinc {

  SolverInstanceBase::Status
  SolverInstanceBase::solve(void) { return SolverInstance::ERROR; }
  
  void
  SolverInstanceBase::reset(void) { assert(false); }
  
  void
  SolverInstanceBase::resetWithConstraints(Model::iterator begin, Model::iterator end) {
    assert(false);
  }

  void
  SolverInstanceBase::processPermanentConstraints(Model::iterator begin, Model::iterator end) {
    assert(false);
  }
  
  void
  SolverInstanceBase::Registry::add(const ASTString& name, poster p) {
    _registry.insert(std::make_pair(name, p));
  }
  void
  SolverInstanceBase::Registry::post(Call* c) {
    ASTStringMap<poster>::t::iterator it = _registry.find(c->id());
    if (it == _registry.end()) {
      std::cerr << "Error: constraint not found: " << c->id() << "\n";
      exit(EXIT_FAILURE);
    }
    it->second(_base, c);
  }

  void
  SolverInstanceBase::assignSolutionToOutput(void) {
    for (VarDeclIterator it = getEnv()->output()->begin_vardecls(); it != getEnv()->output()->end_vardecls(); ++it) {
      if (it->e()->e() == NULL) {
        it->e()->e(getSolutionValue(it->e()->id()));
      }
    }
  }

 void 
  SolverInstanceBase::flattenSearchAnnotations(const Annotation& ann, std::vector<Expression*>& out) {
    for(ExpressionSetIter i = ann.begin(); i != ann.end(); ++i) {
        Expression* e = *i;
        if(e->isa<Call>() && e->cast<Call>()->id().str() == "seq_search") {
            Call* c = e->cast<Call>();
            ArrayLit* anns = c->args()[0]->cast<ArrayLit>();
            for(unsigned int i=0; i<anns->v().size(); i++) {
                Annotation subann;
                subann.add(anns->v()[i]);
                flattenSearchAnnotations(subann, out);
            }
        } else {
            out.push_back(*i);
        }
    }
  }


}  // namespace MiniZinc