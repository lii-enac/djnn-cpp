#include "src_to_dst_link.h"
#include "core/utils/error.h"

namespace djnn
{
  using namespace std;


  UpdateSrcOrDst::UpdateSrcOrDst (FatProcess* parent, const std::string& n) : 
    Action (parent, n), _to_update (nullptr), _prop (nullptr), _spec ("") 
  {
  }

  UpdateSrcOrDst::UpdateSrcOrDst (FatProcess* parent, const std::string& n, RefProperty* prop, const std::string& spec, FatProcess** to_update) : 
    Action (parent, n), _to_update (to_update), _prop (prop), _spec (spec) 
  {
  }

  void
  UpdateSrcOrDst::impl_activate ()
  { 
    ((SrcToDstLink*) get_parent ())->about_to_update_graph();
    auto* v = _prop->get_value ();
    if (!v) {
      *_to_update = nullptr;
      ((SrcToDstLink*) get_parent ())->update_graph ();
      return;
    }
    auto * res = dynamic_cast<FatProcess*>(v->find_child (_spec));
    if (!res) {
      warning (this, "Source or destination not found dynamic link structure");
    }
    *_to_update = res;
   ((SrcToDstLink*) get_parent ())->update_graph ();
  }
}
