/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "core/core-dev.h"
#include "graph_edge_adder.h"
#include "core/utils/error.h"
#include "core/serializer/serializer.h"

namespace djnn
{
  

  GraphEdgeAdder::GraphEdgeAdder (ParentProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst) :
      FatProcess (name), _src (src), _dst (dst)
  {
    if (_src == nullptr || _dst == nullptr) {
      error  (this, "source or destination cannot be null in GraphEdgeAdder ");
      return;
    }
    graph_add_edge (_src, _dst);
    finalize_construction (parent, name);
  }

  GraphEdgeAdder::~GraphEdgeAdder ()
  {
    graph_remove_edge (_src, _dst);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  GraphEdgeAdder::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:graph-edge-adder");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("src", buf);
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("dst", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

}
