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
#include "core/execution/graph.h"

#include <algorithm>
#include <cassert>

namespace djnn
{
  using namespace std;

  NativeExpressionAction::~NativeExpressionAction ()
  {
      remove_all_native_edges ();
  }

  void
  NativeExpressionAction::add_native_edge (CoreProcess * src, CoreProcess * dst)
  {
    assert (src);
    assert (dst);
    // there may be multiple output to a native expression, but with a single _src
    if(_src) assert (src==_src);
    else _src = src;
    Graph::instance ().add_edge (src, dst);
    _dsts.push_back(dst);
  }

  void
  NativeExpressionAction::remove_all_native_edges ()
  {
    if(_src) {
      for (auto dst: _dsts ) {
        Graph::instance ().remove_edge (_src, dst);
      }
      _dsts.clear();
    }
  }

  void
  NativeExpressionAction::remove_native_edge (CoreProcess * src, CoreProcess * dst)
  {
    assert (src);
    assert (dst);
    // there may be multiple output to a native expression, but with a single _src
    //if(_src) assert (src==_src);
    //else _src = src;
    Graph::instance ().remove_edge (src, dst);
    _dsts.erase(std::find(_dsts.begin(), _dsts.end(), dst));
  }
}
