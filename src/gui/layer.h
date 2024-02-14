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
#include "core/property/double_property.h"
#include "display/window.h"

namespace djnn {

    struct LayerCache {
      virtual ~LayerCache() {}
    };
    class Layer : public Container
    {
      class LayerDamagedAction : public Action
      {
      public:
        LayerDamagedAction (CoreProcess* parent, const string& name) :
          Action (parent, name) {}
        virtual ~LayerDamagedAction () {}
        void impl_activate () override { ((Layer*)get_parent())->set_invalid_cache (true); }
      };
    public:
      Layer (CoreProcess* parent, const string& name, double pad=0);
      Layer (CoreProcess* parent, const string& name, double x, double y, double w, double h, double pad=0);
      virtual ~Layer () override;
      auto get_frame () { return _frame;}
      void impl_activate () override;
      void impl_deactivate () override;
      void draw () override;
      LayerCache* cache () { return _cache;}
      LayerCache* pick_cache () { return _pick_cache;}
      void set_cache (LayerCache* cache) { _cache = cache; }
      void set_pick_cache (LayerCache* cache) { _pick_cache = cache; }
      bool invalid_cache () { return _invalid_cache; }
      void set_invalid_cache (bool v);
      Layer* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
      process_type_e get_process_type () const override { return LAYER_T; }

      Spike* damaged () { return &_damaged; }
      virtual void notify_change ( unsigned int nm ) override { 
        if (_auto_redisplay.get_value()) set_invalid_cache(true);
        FatProcess::notify_change (nm);
      }

      void get_xywhp(int& x, int& y, int& w, int& h, int& pad) { x=_x->get_value(); y=_y->get_value(); w=_w->get_value(); h=_h->get_value(); pad=_pad->get_value(); }
      void set_xy(int x, int y) { _x->set_value(x, true); _y->set_value(y,true); }

    protected:
      Window *_frame;
      Spike _damaged;
      BoolProperty _auto_redisplay;
      bool _invalid_cache;
      LayerCache *_cache, *_pick_cache;
      LayerDamagedAction _damaged_action;
      IntProperty *_x, *_y, *_w, *_h, *_pad;
      Coupling _c_damaged, *_c_x, *_c_y, *_c_w, *_c_h, *_c_pad;
    };
}
