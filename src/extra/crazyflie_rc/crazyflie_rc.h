#include "core/control/action.h"
#include "core/control/spike.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/text_property.h"
#include "extra/crazyflie_rc/ext/crazyflie_cpp/include/crazyflie_cpp/Crazyflie.h"

namespace djnn {

class CrazyflieRC : public FatProcess
{
  public:
    CrazyflieRC (CoreProcess* parent, const string& name, const string& uri);

    class TakeoffAction : public Action
    {
      public:
        TakeoffAction (CoreProcess* parent, const string& name) : Action (parent, name){};
        virtual ~TakeoffAction () {}
        void impl_activate () override { ((CrazyflieRC*)get_parent ())->takeoff (); }
    };

    class LandAction : public Action
    {
      public:
        LandAction (CoreProcess* parent, const string& name) : Action (parent, name){};
        virtual ~LandAction () {}
        void impl_activate () override { ((CrazyflieRC*)get_parent ())->land (); }
    };

  protected:
    void impl_activate () override;
    void impl_deactivate () override;

  private:
    Crazyflie* _crazyflie;

    TextProperty _uri;
    // void update_url ();

    Spike         _takeoff;
    TakeoffAction _takeoff_action;
    Coupling      _c_takeoff;

    Spike      _land;
    LandAction _land_action;
    Coupling   _c_land;

    void takeoff ();
    void land ();

    friend class TakeoffAction;
    friend class LandAction;
};
} // namespace djnn