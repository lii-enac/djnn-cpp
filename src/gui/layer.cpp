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
#include "core/tree/component_observer.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/error.h"
#include "core/utils/ext/remotery/Remotery.h"

namespace djnn
{
  Layer::Layer (CoreProcess* parent, const string& n, double x, double y, double w, double h, double pad) :
      Container (parent, n), _frame (nullptr), _damaged (this, "damaged"), _auto_redisplay (this, "auto_redisplay", true),
      _invalid_cache (true), _cache (nullptr), _pick_cache (nullptr), _damaged_action (this, "damaged_action"), 
      _c_damaged (&_damaged, ACTIVATION, &_damaged_action, ACTIVATION)
  {
    finalize_construction (parent, n);

    _x = new IntProperty (this, "x", x);
    _y = new IntProperty (this, "y", y);
    _w = new IntProperty (this, "w", w);
    _h = new IntProperty (this, "h", h);
    _pad = new IntProperty (this, "pad", pad);

    _c_x = new Coupling (_x, ACTIVATION, &_damaged_action, ACTIVATION);
    _c_y = new Coupling (_y, ACTIVATION, &_damaged_action, ACTIVATION);
    _c_w = new Coupling (_w, ACTIVATION, &_damaged_action, ACTIVATION);
    _c_h = new Coupling (_h, ACTIVATION, &_damaged_action, ACTIVATION);
    _c_pad = new Coupling (_h, ACTIVATION, &_damaged_action, ACTIVATION);

  }

  Layer::Layer (CoreProcess* parent, const string& n, double pad) : Layer(parent, n, 0,0,-1,0,pad)
  {
  }

  Layer::~Layer ()
  {
    // remove _damaged and _auto_redisplay from _children
    // because they are also plain object and will be deleted twice.
    remove_child ("damaged");
    remove_child ("auto_redisplay");

    delete _c_pad;
    delete _c_h;
    delete _c_w;
    delete _c_y;
    delete _c_x;

    Container::clean_up_content ();

    delete _cache;
    delete _pick_cache;
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
    warning (this, string ("DEPRECATED - Layer::draw - Should not be using - use GUIStructureHolder::draw instead\n Is Layer only composed of GUI Component ?"));
    // if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
    //   //rmt_BeginCPUSample(Layer_draw, RMTSF_Recursive);
    //   ComponentObserver::instance ().start_draw ();
    //   if (Backend::instance ()->pre_draw_layer (this)) {
    //     //Container::draw ();
    //     for (auto c : _children) {
    //       c->draw ();
    //     }  
    //   }
    //   Backend::instance ()->post_draw_layer (this);
    //   ComponentObserver::instance ().end_draw ();
    //   //rmt_EndCPUSample();
    //}
  }

  Layer*
  Layer::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    /*Layer* newg = new Layer (nullptr, get_name ());

    for (auto c : _children) {
      auto * child = c->clone ();
      if (child != nullptr)
        newg->add_child (child, this->find_child_name(c));
    }

    return newg;*/

    auto * clone = new Layer (nullptr, get_name ());
    origs_clones[this] = clone;
    //impl_clone_properties (clone, origs_clones);
    for (auto c : _children) {
      auto cclone = c->impl_clone (origs_clones);
      //origs_clones[c] = cclone;
      clone->add_child (cclone , this->find_child_name(c));
    }
    
    return clone;
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
