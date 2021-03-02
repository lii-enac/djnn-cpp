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
#include "gui/abstract_backend.h" // DisplayBackend::instance
#include "display/display-dev.h"
#include "display/update_drawing.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/error.h"


namespace djnn
{
  Layer::Layer (ParentProcess* parent, const std::string& n) :
      Container (parent, n), _frame (nullptr), _damaged (this, "damaged"),
     _invalid_cache (true), _cache (nullptr), _damaged_action (this, "damaged_action"), _c_damaged (&_damaged, ACTIVATION, &_damaged_action, ACTIVATION)
  {
    finalize_construction (parent, n);
  }

  Layer::~Layer ()
  {
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
    UpdateDrawing::instance ()->add_window_for_refresh (_frame);
    UpdateDrawing::instance ()->get_redraw ()->activate ();
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

  void
  Layer::set_invalid_cache (bool v)
  {
    _invalid_cache = v;
    if (_frame) {
      UpdateDrawing::instance ()->add_window_for_refresh (_frame);
      UpdateDrawing::instance ()->get_redraw ()->activate ();
    }
  }
}
