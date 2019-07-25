/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../ontology/process.h"
#include "../control/action.h"

namespace djnn {
  using namespace std;

class NativeExpressionAction : public Action {
  public:
    NativeExpressionAction (bool model = false) : Action (model), _src(nullptr) {}
    NativeExpressionAction (Process *p, const std::string &n, bool model = false) : Action (p, n, model), _src(nullptr) {}
    virtual ~NativeExpressionAction ();

    void add_native_edge (Process * src, Process * dst);

  private:
    Process *_src;
    std::vector<Process*> _dsts;
  };
}
