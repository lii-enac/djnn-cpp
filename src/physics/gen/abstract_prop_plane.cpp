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


#include "gui/shapes/abstract_gshape.h"
#include "display/window.h"
#include "gui/style/abstract_style.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"


#include "abstract_prop_plane.h"

namespace djnn
{
  AbstractPropPlane::AbstractPropPlane (ParentProcess* parent, const std::string& name, double a, double b, double c, double d) :
    PhyObj (parent, name),
    raw_props{.a=a, .b=b, .c=c, .d=d},
    _ca (nullptr), _cb (nullptr), _cc (nullptr), _cd (nullptr)
  {
    
    
  }

  AbstractPropPlane::~AbstractPropPlane ()
  {
    remove_edge (_ca);
		remove_edge (_cb);
		remove_edge (_cc);
		remove_edge (_cd);
    delete _ca;
		delete _cb;
		delete _cc;
		delete _cd;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("a");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("b");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("c");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("d");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  AbstractPropPlane::find_child_impl (const std::string& name)
  {
    auto * res = PhyObj::find_child_impl(name);
    if (res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="a") {
      coupling=&_ca;
      rawp_Double=&raw_props.a;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="b") {
      coupling=&_cb;
      rawp_Double=&raw_props.b;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="c") {
      coupling=&_cc;
      rawp_Double=&raw_props.c;
      notify_mask = notify_damaged_geometry;
      prop_Double=true;
    } else
    if(name=="d") {
      coupling=&_cd;
      rawp_Double=&raw_props.d;
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

    return res;
  }

  void
  AbstractPropPlane::get_properties_values (double& a, double& b, double& c, double& d)
  {
    a = raw_props.a;
		b = raw_props.b;
		c = raw_props.c;
		d = raw_props.d;
    
  }

  void
  AbstractPropPlane::impl_activate ()
  {
    PhyObj::impl_activate ();
    auto _frame = get_frame ();
    enable(_ca, _frame->damaged ());
		enable(_cb, _frame->damaged ());
		enable(_cc, _frame->damaged ());
		enable(_cd, _frame->damaged ());
  }

  void
  AbstractPropPlane::impl_deactivate ()
  {
    disable(_ca);
		disable(_cb);
		disable(_cc);
		disable(_cd);
    PhyObj::impl_deactivate ();
  }

  

  

} /* namespace djnn */
