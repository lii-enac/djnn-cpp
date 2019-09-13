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
 */


#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/coupling.h"

#include "abstract_prop_gradient_stop.h"

namespace djnn
{
  AbstractPropGradientStop::AbstractPropGradientStop (Process *p, const std::string& n, double r, double g, double b, double a, double offset) :
    AbstractStyle (p, n),
    raw_props{.r=r, .g=g, .b=b, .a=a, .offset=offset},
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _ca (nullptr), _coffset (nullptr)
  {
    
    
  }

  AbstractPropGradientStop::AbstractPropGradientStop (double r, double g, double b, double a, double offset) :
    AbstractStyle (), 
    raw_props{.r=r, .g=g, .b=b, .a=a, .offset=offset},
    _cr (nullptr), _cg (nullptr), _cb (nullptr), _ca (nullptr), _coffset (nullptr)
  {
    
  }

  AbstractPropGradientStop::~AbstractPropGradientStop ()
  {
    delete _cr;
		delete _cg;
		delete _cb;
		delete _ca;
		delete _coffset;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (_symtable.size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("r");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("g");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("b");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("a");
			if (it != _symtable.end ())
				delete it->second;

			it = _symtable.find ("offset");
			if (it != _symtable.end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractPropGradientStop::find_component (const string& name)
  {
    Process* res = AbstractStyle::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="r") {
      coupling=&_cr;
      rawp_Double=&raw_props.r;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="g") {
      coupling=&_cg;
      rawp_Double=&raw_props.g;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="b") {
      coupling=&_cb;
      rawp_Double=&raw_props.b;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="a") {
      coupling=&_ca;
      rawp_Double=&raw_props.a;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="offset") {
      coupling=&_coffset;
      rawp_Double=&raw_props.offset;
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

    return res;
  }

  void
  AbstractPropGradientStop::get_properties_values (double& r, double& g, double& b, double& a, double& offset)
  {
    r = raw_props.r;
		g = raw_props.g;
		b = raw_props.b;
		a = raw_props.a;
		offset = raw_props.offset;
  }

  void
  AbstractPropGradientStop::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cr) _cr->enable (_frame);
		if(_cg) _cg->enable (_frame);
		if(_cb) _cb->enable (_frame);
		if(_ca) _ca->enable (_frame);
		if(_coffset) _coffset->enable (_frame);
  }

  void
  AbstractPropGradientStop::impl_deactivate ()
  {
    if(_cr) _cr->disable ();
		if(_cg) _cg->disable ();
		if(_cb) _cb->disable ();
		if(_ca) _ca->disable ();
		if(_coffset) _coffset->disable ();
    AbstractStyle::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
