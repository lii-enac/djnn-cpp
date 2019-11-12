#include "src_to_dst_link.h"

namespace djnn
{
  using namespace std;

  UpdateSrcOrDst::UpdateSrcOrDst (Process* parent, const string &n, RefProperty* prop, const string &spec, Process** to_update) : 
    Action (parent, n), _to_update (to_update), _prop (prop), _spec (spec) 
  { 
  }

  void
  UpdateSrcOrDst::impl_activate ()
  { 
    ((SrcToDstLink*) get_parent ())->about_to_update_graph();
    Process* v = _prop->get_value ();
    if (!v) {
      *_to_update = nullptr;
      ((SrcToDstLink*) get_parent ())->update_graph ();
      return;
    }
    Process *res = v->find_component (_spec);
    if (!res) {
      warning (this, "Source or destination not found dynamic link structure");
    }
    *_to_update = res;
   ((SrcToDstLink*) get_parent ())->update_graph ();
  }
}
