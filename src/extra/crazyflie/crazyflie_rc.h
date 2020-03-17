#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/spike.h"
#include "core/tree/text_property.h"


#include "Crazyflie.h"

namespace djnn {

	class CrazyflieRC : public Process {
	public:
		CrazyflieRC (Process * parent, const string& name);

		class TakeoffAction : public Action
	    {
	    public:
	      TakeoffAction (Process* parent, const string &name) : Action (parent, name) {};
	      virtual ~TakeoffAction () {}
	      void impl_activate () override { ((CrazyflieRC*)get_parent())->takeoff (); }
	      void impl_deactivate () override {}
	    };

	    class LandAction : public Action
	    {
	    public:
	      LandAction (Process* parent, const string &name) : Action (parent, name) {};
	      virtual ~LandAction () {}
	      void impl_activate () override { ((CrazyflieRC*)get_parent())->land (); }
	      void impl_deactivate () override {}
	    };

	protected:
		void impl_activate() override;
		void impl_deactivate() override;

	private:
		Crazyflie * _crazyflie;

		TextProperty _url;
		//void update_url ();

		Spike _takeoff;
		TakeoffAction _takeoff_action;
		Coupling _c_takeoff;

		Spike _land;
		LandAction _land_action;
		Coupling _c_land;

		void takeoff();
		void land();

		friend class TakeoffAction;
		friend class LandAction;
	};
}