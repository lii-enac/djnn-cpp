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
//#include "gui/shapes/shapes.h"
#include "gui/shapes/abstract_gshape.h"
#include "gui/style/style.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"


#include "abstract_prop_world.h"

namespace djnn
{
  AbstractPropWorld::AbstractPropWorld (ParentProcess* parent, const std::string& name, double x, double y, double z, double dt) :
    AbstractPObj (parent, name),
    raw_props{.x=x, .y=y, .z=z, .dt=dt},
    _cx (nullptr), _cy (nullptr), _cz (nullptr), _cdt (nullptr)
  {
    
    
  }

  AbstractPropWorld::~AbstractPropWorld ()
  {
    remove_edge (_cx);
		remove_edge (_cy);
		remove_edge (_cz);
		remove_edge (_cdt);
    delete _cx;
		delete _cy;
		delete _cz;
		delete _cdt;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("x");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("y");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("z");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("dt");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  AbstractPropWorld::find_child_impl (const std::string& name)
  {
    auto * res = AbstractPObj::find_child_impl(name);
    if (res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="x") {
      coupling=&_cx;
      rawp_Double=&raw_props.x;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp_Double=&raw_props.y;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="z") {
      coupling=&_cz;
      rawp_Double=&raw_props.z;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="dt") {
      coupling=&_cdt;
      rawp_Double=&raw_props.dt;
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
  AbstractPropWorld::get_properties_values (double& x, double& y, double& z, double& dt)
  {
    x = raw_props.x;
		y = raw_props.y;
		z = raw_props.z;
		dt = raw_props.dt;
    
  }

  void
  AbstractPropWorld::impl_activate ()
  {
    AbstractPObj::impl_activate ();
    auto _frame = get_frame ();
    enable(_cx, _frame->damaged ());
		enable(_cy, _frame->damaged ());
		enable(_cz, _frame->damaged ());
		enable(_cdt, _frame->damaged ());
  }

  void
  AbstractPropWorld::impl_deactivate ()
  {
    disable(_cx);
		disable(_cy);
		disable(_cz);
		disable(_cdt);
    AbstractPObj::impl_deactivate ();
  }

  

  

} /* namespace djnn */
