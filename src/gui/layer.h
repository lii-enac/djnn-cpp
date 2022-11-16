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

    struct LayerCache {
      virtual ~LayerCache() {}
    };
    class Layer : public Container
    {
      class LayerDamagedAction : public Action
      {
      public:
        LayerDamagedAction (ParentProcess* parent, const string& name) :
          Action (parent, name) {}
        virtual ~LayerDamagedAction () {}
        void impl_activate () override { ((Layer*)get_parent())->set_invalid_cache (true); }
      };
    public:
      Layer (ParentProcess* parent, const string& name, double pad=0);
      Layer (ParentProcess* parent, const string& name, double x, double y, double w, double h, double pad=0);
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
      Layer* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
      process_type_e get_process_type () const override { return LAYER_T; }

      Spike* damaged () { return &_damaged; }
      virtual void notify_change ( unsigned int nm ) override { 
        if (_auto_redisplay.get_value()) set_invalid_cache(true);
        FatProcess::notify_change (nm);
      }

      void get_xywhp(double& x, double& y, double& w, double& h, double& pad) { x=_x; y=_y; w=_w; h=_h; pad=_pad; }
      void set_xy(double x, double y) { _x=x; _y=y; }

    protected:
      Window *_frame;
      Spike _damaged;
      BoolProperty _auto_redisplay;
      bool _invalid_cache;
      LayerCache *_cache, *_pick_cache;
      LayerDamagedAction _damaged_action;
      Coupling _c_damaged;
      double _x, _y, _w, _h, _pad;
    };
}
