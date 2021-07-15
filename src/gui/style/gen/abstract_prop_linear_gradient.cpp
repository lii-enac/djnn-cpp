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

#include "gui/style/gradient.h"

#include "abstract_prop_linear_gradient.h"

namespace djnn
{
  AbstractPropLinearGradient::AbstractPropLinearGradient (ParentProcess* parent, const string& name, double x1, double y1, double x2, double y2, int spread, int coords) :
    AbstractGradient (parent, name, spread, coords),
    raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
    _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    
    
  }

  AbstractPropLinearGradient::~AbstractPropLinearGradient ()
  {
    remove_edge (_cx1);
		remove_edge (_cy1);
		remove_edge (_cx2);
		remove_edge (_cy2);
    delete _cx1;
		delete _cy1;
		delete _cx2;
		delete _cy2;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("x1");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("y1");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("x2");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("y2");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  AbstractPropLinearGradient::find_child_impl (const string& name)
  {
    auto * res = AbstractGradient::find_child_impl(name);
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
    
    if(name=="x1") {
      coupling=&_cx1;
      rawp_Double=&raw_props.x1;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp_Double=&raw_props.y1;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="x2") {
      coupling=&_cx2;
      rawp_Double=&raw_props.x2;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="y2") {
      coupling=&_cy2;
      rawp_Double=&raw_props.y2;
      notify_mask = notify_damaged_style;
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

  const vector<string>&
  AbstractPropLinearGradient::get_properties_name () const
  {
    static const vector<string> res = {
    "x1",
			"y1",
			"x2",
			"y2",
    };
    return res;
  }

  void
  AbstractPropLinearGradient::get_properties_values (double& x1, double& y1, double& x2, double& y2, int& spread, int& coords)
  {
    x1 = raw_props.x1;
		y1 = raw_props.y1;
		x2 = raw_props.x2;
		y2 = raw_props.y2;
    AbstractGradient::get_properties_values(spread, coords);
  }

  void
  AbstractPropLinearGradient::impl_activate ()
  {
    AbstractGradient::impl_activate ();
    auto _frame = get_frame ();
    enable(_cx1, _frame->damaged ());
		enable(_cy1, _frame->damaged ());
		enable(_cx2, _frame->damaged ());
		enable(_cy2, _frame->damaged ());
  }

  void
  AbstractPropLinearGradient::impl_deactivate ()
  {
    disable(_cx1);
		disable(_cy1);
		disable(_cx2);
		disable(_cy2);
    AbstractGradient::impl_deactivate ();
  }

  

  

} /* namespace djnn */
