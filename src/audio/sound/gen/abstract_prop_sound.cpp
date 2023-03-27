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


#include "abstract_prop_sound.h"

namespace djnn
{
  AbstractPropSound::AbstractPropSound (CoreProcess* parent, const string& name, double gain, double lowpass_gain, double lowpass_freq, double x, double y, double z, double pitch_mul, int loop) :
    AbstractSObj (parent, name),
    raw_props{.gain=gain, .lowpass_gain=lowpass_gain, .lowpass_freq=lowpass_freq, .x=x, .y=y, .z=z, .pitch_mul=pitch_mul, .loop=loop},
    _cgain (nullptr), _clowpass_gain (nullptr), _clowpass_freq (nullptr), _cx (nullptr), _cy (nullptr), _cz (nullptr), _cpitch_mul (nullptr), _cloop (nullptr)
  {
    
    
  }

  AbstractPropSound::~AbstractPropSound ()
  {
    remove_edge (_cgain);
		remove_edge (_clowpass_gain);
		remove_edge (_clowpass_freq);
		remove_edge (_cx);
		remove_edge (_cy);
		remove_edge (_cz);
		remove_edge (_cpitch_mul);
		remove_edge (_cloop);
    delete _cgain;
		delete _clowpass_gain;
		delete _clowpass_freq;
		delete _cx;
		delete _cy;
		delete _cz;
		delete _cpitch_mul;
		delete _cloop;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    if (children_size () > 0) {
      symtable_t::iterator it;

      it = find_child_iterator ("gain");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("lowpass_gain");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("lowpass_freq");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("x");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("y");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("z");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("pitch_mul");
			if (it != children_end ())
				delete it->second;

			it = find_child_iterator ("loop");
			if (it != children_end ())
				delete it->second;
    }
  }
 
  CoreProcess*
  AbstractPropSound::find_child_impl (const string& name)
  {
    auto * res = AbstractSObj::find_child_impl(name);
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
    
    if(name=="gain") {
      coupling=&_cgain;
      rawp_Double=&raw_props.gain;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="lowpass_gain") {
      coupling=&_clowpass_gain;
      rawp_Double=&raw_props.lowpass_gain;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
    if(name=="lowpass_freq") {
      coupling=&_clowpass_freq;
      rawp_Double=&raw_props.lowpass_freq;
      notify_mask = notify_damaged_style;
      prop_Double=true;
    } else
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
    if(name=="pitch_mul") {
      coupling=&_cpitch_mul;
      rawp_Double=&raw_props.pitch_mul;
      notify_mask = notify_damaged_transform;
      prop_Double=true;
    } else
    if(name=="loop") {
      coupling=&_cloop;
      rawp_Int=&raw_props.loop;
      notify_mask = notify_damaged_style;
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
    else if(prop_Textp) {
      TextPropertyProxy* prop = nullptr; // do not cache
      res = create_GObj_prop(&prop, coupling, rawp_Textp, name, notify_mask);
    }

    return res;
  }

  const vector<string>&
  AbstractPropSound::get_properties_name () const
  {
    static const vector<string> res = {
    "gain",
			"lowpass_gain",
			"lowpass_freq",
			"x",
			"y",
			"z",
			"pitch_mul",
			"loop",
    };
    return res;
  }

  void
  AbstractPropSound::get_properties_values (double& gain, double& lowpass_gain, double& lowpass_freq, double& x, double& y, double& z, double& pitch_mul, int& loop)
  {
    gain = raw_props.gain;
		lowpass_gain = raw_props.lowpass_gain;
		lowpass_freq = raw_props.lowpass_freq;
		x = raw_props.x;
		y = raw_props.y;
		z = raw_props.z;
		pitch_mul = raw_props.pitch_mul;
		loop = raw_props.loop;
    
  }

  void
  AbstractPropSound::impl_activate ()
  {
    AbstractSObj::impl_activate ();
    auto _frame = get_frame ();
    enable(_cgain, _frame->damaged ());
		enable(_clowpass_gain, _frame->damaged ());
		enable(_clowpass_freq, _frame->damaged ());
		enable(_cx, _frame->damaged ());
		enable(_cy, _frame->damaged ());
		enable(_cz, _frame->damaged ());
		enable(_cpitch_mul, _frame->damaged ());
		enable(_cloop, _frame->damaged ());
  }

  void
  AbstractPropSound::impl_deactivate ()
  {
    disable(_cgain);
		disable(_clowpass_gain);
		disable(_clowpass_freq);
		disable(_cx);
		disable(_cy);
		disable(_cz);
		disable(_cpitch_mul);
		disable(_cloop);
    AbstractSObj::impl_deactivate ();
  }

  

  

} /* namespace djnn */
