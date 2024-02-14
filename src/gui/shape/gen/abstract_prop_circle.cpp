/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 *  !! this file has been automatically generated - do NOT modify !!
 *
 */


#include "gui/shape/abstract_gshape.h"
#include "display/window.h"
#include "gui/style/abstract_style.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"


#include "abstract_prop_circle.h"

namespace djnn
{
  AbstractPropCircle::AbstractPropCircle (CoreProcess* parent, const string& name, double cx, double cy, double r) :
    AbstractGShape (parent, name),
    raw_props{.cx=cx, .cy=cy, .r=r},
    _ccx (nullptr), _ccy (nullptr), _cr (nullptr)
  {
    set_origin (cx,cy);
    
  }

  AbstractPropCircle::~AbstractPropCircle ()
  {
    remove_edge (_ccx);
		remove_edge (_ccy);
		remove_edge (_cr);
    delete _ccx;
		delete _ccy;
		delete _cr;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 2) {
      symtable_t::iterator it;

      it = find_child_iterator ("cx");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("cy");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("r");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  CoreProcess*
  AbstractPropCircle::find_child_impl (const string& name)
  {
    auto * res = AbstractGShape::find_child_impl(name);
    if (res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false, prop_Textp=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    typedef string* textp;
    textp* rawp_Textp = nullptr;
    int notify_mask = notify_none;
    
    if(name=="cx") {
      coupling=&_ccx;
      rawp_Double=&raw_props.cx;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp_Double=&raw_props.cy;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="r") {
      coupling=&_cr;
      rawp_Double=&raw_props.r;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    return nullptr;

    if(prop_Double) {
      DoublePropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Double, name, notify_mask);
    }
    else if(prop_Int) {
      IntPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Int, name, notify_mask);
    }
    else if(prop_Text) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Text, name, notify_mask);
    }
    else if(prop_Textp) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Textp, name, notify_mask);
    }

    return res;
  }

  const djnnstl::vector<djnnstl::string>&
  AbstractPropCircle::get_properties_name () const
  {
    static const vector<string> res = {
    "cx",
			"cy",
			"r",
    };
    return res;
  }

  void
  AbstractPropCircle::get_properties_values (double& cx, double& cy, double& r)
  {
    cx = raw_props.cx;
		cy = raw_props.cy;
		r = raw_props.r;
    
  }

  void
  AbstractPropCircle::impl_activate ()
  {
    AbstractGShape::impl_activate ();
    auto _frame = get_frame ();
    enable(_ccx, _frame->damaged ());
		enable(_ccy, _frame->damaged ());
		enable(_cr, _frame->damaged ());
  }

  void
  AbstractPropCircle::impl_deactivate ()
  {
    disable(_ccx);
		disable(_ccy);
		disable(_cr);
    AbstractGShape::impl_deactivate ();
  }

  

  

} /* namespace djnn */
