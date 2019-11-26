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

#include "outline_join_style.h"

namespace djnn
{
  OutlineJoinStyle::OutlineJoinStyle (Process *parent, const std::string& name, int join) :
    AbstractStyle (parent, name),
    raw_props{.join=join},
    _cjoin (nullptr)
  {
    
    Process::finalize_construction (parent, name);
  }

  OutlineJoinStyle::OutlineJoinStyle (int join) :
    AbstractStyle (), 
    raw_props{.join=join},
    _cjoin (nullptr)
  {
    
  }

  OutlineJoinStyle::~OutlineJoinStyle ()
  {
    delete _cjoin;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 0) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("join");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  OutlineJoinStyle::find_component (const string& name)
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
    
    if(name=="join") {
      coupling=&_cjoin;
      rawp_Int=&raw_props.join;
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
  OutlineJoinStyle::get_properties_values (int& join)
  {
    join = raw_props.join;
  }

  void
  OutlineJoinStyle::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    if(_cjoin) _cjoin->enable (_frame);
  }

  void
  OutlineJoinStyle::impl_deactivate ()
  {
    if(_cjoin) _cjoin->disable ();
    AbstractStyle::impl_deactivate ();
  }

  
  void
  OutlineJoinStyle::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_join_style (this);
    }
  }


  
  Process*
  OutlineJoinStyle::clone ()
  {
    return new OutlineJoinStyle (raw_props.join);
  }

  
} /* namespace djnn */
