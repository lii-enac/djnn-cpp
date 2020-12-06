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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "gui/layer.h"
#include "gui/backend.h"
#include "gui/abstract_backend.h"
//#include "display/display.h"
//#include "display/abstract_display.h"
#include "display/display-dev.h"


#include "core/execution/graph.h"
#include "core/utils/error.h"


namespace djnn
{
  Layer::Layer (ParentProcess* parent, const std::string& n) :
      Container (parent, n), _frame (nullptr), _damaged (this, "damaged"),
     _invalid_cache (true), _cache (nullptr), _damaged_action (this, "damaged_action"), _c_damaged (&_damaged, ACTIVATION, &_damaged_action, ACTIVATION)
  {
    Graph::instance().add_edge (&_damaged, &_damaged_action);
    finalize_construction (parent, n);
  }

  Layer::~Layer ()
  {
    Graph::instance().remove_edge (&_damaged, &_damaged_action);
  }

  void
  Layer::impl_activate ()
  {
    _frame = find_frame (this);
    if (!_frame) {
      warning ( nullptr, " no running frame found for layer " + get_name () + "\n" );
      return;
    }
    Container::impl_activate ();
    _frame->damaged ()->activate ();
  }

  void
  Layer::impl_deactivate ()
  {
    Container::impl_deactivate ();
  }

  void
  Layer::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      if (Backend::instance ()->pre_draw_layer (this)) {
        Container::draw ();
      }
      Backend::instance ()->post_draw_layer (this);
    }
  }

  Layer*
  Layer::clone ()
  {
    Layer* newg = new Layer (nullptr, get_name ());

    for (auto c : _children) {
      auto * child = c->clone ();
      if (child != nullptr)
        newg->add_child (child, this->find_child_name(c));
    }

    return newg;
  }
}
