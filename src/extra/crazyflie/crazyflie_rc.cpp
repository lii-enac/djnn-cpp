#include "crazyflie_rc.h"

namespace djnn {

	CrazyflieRC::CrazyflieRC(Process *parent, const string& name)
	: Process(name), _url(this, "url", ""),
	_takeoff(this, "takeoff"),
	_takeoff_action(this, "takeoff_action"),
	_c_takeoff (&_takeoff, ACTIVATION, &_takeoff_action, ACTIVATION, true),
	 _land(this,"takeoff"),
	 _land_action(this, "takeoff_action"),
	_c_land (&_land, ACTIVATION, &_land_action, ACTIVATION, true)
	{
		finalize_construction(parent, name);
	}

	void
	CrazyflieRC::impl_activate()
	{
		_crazyflie = new Crazyflie (_url.get_string_value ());
	}

	void
	CrazyflieRC::impl_deactivate()
	{
		_crazyflie->stop();
	}

	void
	CrazyflieRC::takeoff()
	{
		_crazyflie->takeoff(0.1, 2);
	}

	void
	CrazyflieRC::land()
	{
		_crazyflie->land(0.1, 2);
	}

	/*
	void
	CrazyflieRC::update_url ()
	{
		_url.get_string_value ();
	}*/

}


