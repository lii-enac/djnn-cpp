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

#include "native_expression_action.h"

#include <cassert>

namespace djnn
{
  using namespace std;

  NativeExpressionAction::~NativeExpressionAction ()
  {
    if(_src) {
      for (auto dst: _dsts ) {
        Graph::instance ().remove_edge (_src, dst);
      }
    }
  }

  void
  NativeExpressionAction::add_native_edge (Process * src, Process * dst)
  {
    assert (src);
    assert (dst);
    // there may be multiple output to a native expression, but with a single _src
    if(_src) assert (src==_src);
    else _src = src;
    Graph::instance ().add_edge (src, dst);
    _dsts.push_back(dst);
  }
}
