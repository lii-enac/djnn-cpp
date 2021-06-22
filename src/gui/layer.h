/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "gui/abstract_gobj.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/component.h"
#include "display/window.h"

namespace djnn {
    class Layer : public Container
    {
      class LayerDamagedAction : public Action
      {
      public:
        LayerDamagedAction (ParentProcess* parent, const djnn::string& name) :
          Action (parent, name) {}
        virtual ~LayerDamagedAction () {}
        void impl_activate () override { ((Layer*)get_parent())->set_invalid_cache (true); }
      };
    public:
      Layer (ParentProcess* parent, const djnn::string& name);
      Layer (ParentProcess* parent, const djnn::string& name, double x, double y, double w, double h);
      virtual ~Layer () override;
      auto
        get_frame () { return _frame;}
      void impl_activate () override;
      void impl_deactivate () override;
      void draw () override;
      void* cache () { return _cache;}
      void* pick_cache () { return _pick_cache;}
      void set_cache (void* cache) { _cache = cache; }
      void set_pick_cache (void* cache) { _pick_cache = cache; }
      bool invalid_cache () { return _invalid_cache; }
      void set_invalid_cache (bool v);
      Layer* clone () override;
      FatProcess* damaged () { return &_damaged; }
      process_type_e get_process_type () const override { return LAYER_T; }

      void get_xywh(double& x, double& y, double& w, double& h) { x=_x; y=_y; w=_w; h=_h; }

    protected:
      Window *_frame;
      Spike _damaged;
      bool _invalid_cache;
      void *_cache, *_pick_cache;
      LayerDamagedAction _damaged_action;
      Coupling _c_damaged;
      double _x, _y, _w, _h;
    };
}
