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


#include "outline_join_style.h"

namespace djnn
{
  OutlineJoinStyle::OutlineJoinStyle (ParentProcess* parent, const std::string& name, int join) :
    AbstractStyle (parent, name),
    raw_props{.join=join},
    _cjoin (nullptr)
  {
    
    finalize_construction (parent, name);
  }

  OutlineJoinStyle::~OutlineJoinStyle ()
  {
    remove_edge (_cjoin);
    delete _cjoin;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("join");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  FatChildProcess*
  OutlineJoinStyle::find_child (const std::string& name)
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
    auto _frame = get_frame ();
    enable(_cjoin, _frame->damaged ());
  }

  void
  OutlineJoinStyle::impl_deactivate ()
  {
    disable(_cjoin);
    AbstractStyle::impl_deactivate ();
  }

  
  void
  OutlineJoinStyle::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->load_outline_join_style (this);
    }
  }


  
  OutlineJoinStyle*
  OutlineJoinStyle::clone ()
  {
    return new OutlineJoinStyle (nullptr, get_name (), raw_props.join);
  }


} /* namespace djnn */
