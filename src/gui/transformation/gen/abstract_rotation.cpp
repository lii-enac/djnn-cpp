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


#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h"
#include "display/window.h"

#include "gui/shapes/abstract_gshape.h"
#include "gui/style/style.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"


#include "abstract_rotation.h"

namespace djnn
{
  AbstractRotation::AbstractRotation (ParentProcess* parent, const std::string& name, double a, double cx, double cy) :
    AbstractTransformation (parent, name),
    raw_props{.a=a, .cx=cx, .cy=cy},
    _ca (nullptr), _ccx (nullptr), _ccy (nullptr)
  {
    
    
  }

  AbstractRotation::~AbstractRotation ()
  {
    remove_edge (_ca);
		remove_edge (_ccx);
		remove_edge (_ccy);
    delete _ca;
		delete _ccx;
		delete _ccy;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("a");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("cx");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("cy");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  AbstractRotation::find_child_impl (const std::string& name)
  {
    auto * res = AbstractTransformation::find_child_impl(name);
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
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
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
  AbstractRotation::get_properties_values (double& a, double& cx, double& cy)
  {
    a = raw_props.a;
		cx = raw_props.cx;
		cy = raw_props.cy;
    
  }

  void
  AbstractRotation::impl_activate ()
  {
    AbstractTransformation::impl_activate ();
    auto _frame = get_frame ();
    enable(_ca, _frame->damaged ());
		enable(_ccx, _frame->damaged ());
		enable(_ccy, _frame->damaged ());
  }

  void
  AbstractRotation::impl_deactivate ()
  {
    disable(_ca);
		disable(_ccx);
		disable(_ccy);
    AbstractTransformation::impl_deactivate ();
  }

  

  

} /* namespace djnn */
