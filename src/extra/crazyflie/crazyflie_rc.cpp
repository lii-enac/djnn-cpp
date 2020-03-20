#include "crazyflie_rc.h"

#include <iostream>
#include "utils/debug.h"

namespace djnn {

	class DjnnLogger : public Logger
	{
	public:
	  DjnnLogger() {}
	  virtual ~DjnnLogger() {}

	  virtual void info(const std::string& msg) override { std::cerr << msg << __FL__; }
	  virtual void warning(const std::string& msg) override { std::cerr << msg << __FL__; }
	  virtual void error(const std::string& msg) override { std::cerr << msg << __FL__; }
	};

	CrazyflieRC::CrazyflieRC(Process *parent, const std::string& name, const std::string& uri)
	: Process(name),
	_uri(this, "uri", uri),
	_takeoff(this, "takeoff"),
	_takeoff_action(this, "takeoff_action"),
	_c_takeoff (&_takeoff, ACTIVATION, &_takeoff_action, ACTIVATION, true),
	_land(this,"land"),
	_land_action(this, "land_action"),
	_c_land (&_land, ACTIVATION, &_land_action, ACTIVATION, true)
	{
		finalize_construction(parent, name);
	}

	DjnnLogger djnnLogger;

	void
	CrazyflieRC::impl_activate()
	{ DBG;
		_crazyflie = new Crazyflie (_uri.get_string_value (), djnnLogger);
		std::cout << "firmware version: " << _crazyflie->getFirmwareVersion() << std::endl;
    	std::cout << "protocol version: " << _crazyflie->getProtocolVersion() << std::endl;
    	//_crazyflie->sendSetpoint(0,0,0,0); // unlock
    	_crazyflie->setParamByName("commander", "enHighLevel", (unsigned char)1);
    	//_crazyflie->syson();
    	//std::cout << "device type name: " << _crazyflie->getDeviceTypeName() << std::endl;
	}

	void
	CrazyflieRC::impl_deactivate()
	{
		_crazyflie->stop();
	}

	void
	CrazyflieRC::takeoff()
	{ //DBG;
		_crazyflie->takeoff(0.3, 1);
		//_crazyflie->sendSetpoint(0,0,0,2);
	}

	void
	CrazyflieRC::land()
	{ //DBG;
		_crazyflie->land(0.0, 2);
		//_crazyflie->sendSetpoint(0,0,0,0);
	}

	/*
	void
	CrazyflieRC::update_url ()
	{
		_url.get_string_value ();
	}*/

}


