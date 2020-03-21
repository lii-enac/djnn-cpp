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
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/process.h"
#include "core/ontology/coupling.h"

#include "audio/style/audio_style.h"


#include "outline_miter_limit.h"

namespace djnn
{
  OutlineMiterLimit::OutlineMiterLimit (Process *parent, const std::string& name, int limit) :
    AbstractStyle (parent, name),
    raw_props{.limit=limit},
    _climit (nullptr)
  {
    
    Process::finalize_construction (parent, name);
  }

  OutlineMiterLimit::~OutlineMiterLimit ()
  {
    delete _climit;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("limit");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  OutlineMiterLimit::find_child (const std::string& name)
  {
    Process* res = AbstractStyle::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="limit") {
      coupling=&_climit;
      rawp_Int=&raw_props.limit;
      notify_mask = notify_damaged_geometry;
      prop_Int=true;
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
  OutlineMiterLimit::get_properties_values (int& limit)
  {
    limit = raw_props.limit;
    
  }

  void
  OutlineMiterLimit::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_climit) _climit->enable (_frame);
  }

  void
  OutlineMiterLimit::impl_deactivate ()
  {
    if(_climit) _climit->disable ();
    AbstractStyle::impl_deactivate ();
  }

  
  void
  OutlineMiterLimit::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_miter_limit (this);
    }
  }


  
  Process*
  OutlineMiterLimit::clone ()
  {
    return new OutlineMiterLimit (nullptr, get_name (), raw_props.limit);
  }


} /* namespace djnn */
