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


#include "outline_width.h"

namespace djnn
{
  OutlineWidth::OutlineWidth (FatProcess *parent, const std::string& name, double width) :
    AbstractStyle (parent, name),
    raw_props{.width=width},
    _cwidth (nullptr)
  {
    
    finalize_construction (parent, name);
  }

  OutlineWidth::~OutlineWidth ()
  {
    remove_edge (_cwidth);
    delete _cwidth;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("width");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  ChildProcess*
  OutlineWidth::find_child (const std::string& name)
  {
    auto * res = AbstractStyle::find_child(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    CouplingWithData ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef std::string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="width") {
      coupling=&_cwidth;
      rawp_Double=&raw_props.width;
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
  OutlineWidth::get_properties_values (double& width)
  {
    width = raw_props.width;
    
  }

  void
  OutlineWidth::impl_activate ()
  {
    AbstractStyle::impl_activate ();
    auto _frame = frame ();
    enable(_cwidth, _frame->damaged ());
  }

  void
  OutlineWidth::impl_deactivate ()
  {
    disable(_cwidth);
    AbstractStyle::impl_deactivate ();
  }

  
  void
  OutlineWidth::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_width (this);
    }
  }


  
  OutlineWidth*
  OutlineWidth::clone ()
  {
    return new OutlineWidth (nullptr, get_name (), raw_props.width);
  }


} /* namespace djnn */
