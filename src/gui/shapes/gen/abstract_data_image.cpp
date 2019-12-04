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

#include "abstract_data_image.h"

namespace djnn
{
  AbstractDataImage::AbstractDataImage (Process *parent, const std::string& name, std::string data, double x, double y, double width, double height) :
    AbstractImage (parent, name, x, y, width, height),
    raw_props{.data=data},
    _cdata (nullptr)
  {
    set_origin (x, y);
    
  }

  AbstractDataImage::AbstractDataImage (std::string data, double x, double y, double width, double height) :
    AbstractImage (x, y, width, height), 
    raw_props{.data=data},
    _cdata (nullptr)
  {
    set_origin (x, y);
  }

  AbstractDataImage::~AbstractDataImage ()
  {
    delete _cdata;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (symtable ().size () > 2) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("data");
			if (it != symtable ().end ())
				delete it->second;
    }
  }
 
  Process*
  AbstractDataImage::find_component (const string& name)
  {
    Process* res = AbstractImage::find_component(name);
    if(res) return res;

    bool prop_Double=false, prop_Int=false, prop_Text=false;
    Coupling ** coupling = nullptr;
    double* rawp_Double = nullptr;
    int* rawp_Int = nullptr;
    typedef string text;
    text* rawp_Text = nullptr;
    int notify_mask = notify_none;
    
    if(name=="data") {
      coupling=&_cdata;
      rawp_Text=&raw_props.data;
      notify_mask = notify_damaged_geometry;
      prop_Text=true;
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
  AbstractDataImage::get_properties_values (std::string& data, double& x, double& y, double& width, double& height)
  {
    data = raw_props.data;
  }

  void
  AbstractDataImage::impl_activate ()
  {
    AbstractImage::impl_activate ();
    auto _frame = frame ();
    if(_cdata) _cdata->enable (_frame);
  }

  void
  AbstractDataImage::impl_deactivate ()
  {
    if(_cdata) _cdata->disable ();
    AbstractImage::impl_deactivate ();
  }

  

  
  
} /* namespace djnn */
