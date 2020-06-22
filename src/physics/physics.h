/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/control/action.h"
#include "core/tree/spike.h"
#include "core/tree/double_property.h"

//using namespace std;

namespace djnn
{

  extern std::vector<string> loadedModules; 

  void init_physics ();
  class PhyObj;
  class WorldImpl {
  public:
    WorldImpl () {}
    virtual  ~WorldImpl () {}
    virtual void step (double dt) = 0;
  };

  class PhyObjImpl {
  public:
    PhyObjImpl () {}
    virtual  ~PhyObjImpl () {}
  };

  class World : public FatProcess {
    typedef std::vector<PhyObj*> phy_obj_list;
    class StepAction : public Action {
    public:
      StepAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}

      virtual ~StepAction () {}
    private:
      void impl_activate () override;
      void impl_deactivate () override {};
    };

  public:
    World (ParentProcess* parent, const std::string& name, double x, double y, double z = 0);
    virtual ~World ();
    WorldImpl* get_impl () { return _world_impl; }
    void get_gravity (double &x, double &y);
    void get_dt (double &dt);
    FatProcess* find_child (const std::string& n) override;
    virtual process_type_e get_process_type () const override { return WORLD_T; }
    void add_phy_object (PhyObj* p) { _phy_objs.push_back (p); }
    void remove_phy_object (PhyObj* p) { _phy_objs.erase (std::remove (_phy_objs.begin (), _phy_objs.end (), p), _phy_objs.end ()); }
    phy_obj_list& get_phy_objects  () { return _phy_objs; }
   private:
    struct raw_props_t { double x, y, z, dt; };
    raw_props_t raw_props;
    Coupling *_cstep;
    FatProcess *_step, *_step_action, *_dt;
    void impl_activate () override;
    void impl_deactivate () override;
    WorldImpl *_world_impl;
    phy_obj_list _phy_objs;
  };

  class PhyObj : public FatProcess {
    class D3PhyObjUpdatePosition : public Action {
        public:
          D3PhyObjUpdatePosition (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          ~D3PhyObjUpdatePosition () {}
          void impl_activate () override;
          void impl_deactivate () override {}
        };
        class D3PhyObjUpdateVelocity : public Action {
        public:
          D3PhyObjUpdateVelocity (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          ~D3PhyObjUpdateVelocity () {}
          void impl_activate () override;
          void impl_deactivate () override {}
        };
  public:
    PhyObj (ParentProcess* parent, const std::string& name, double x, double y, double z, double mass);
    virtual ~PhyObj ();
    FatProcess* find_child (const std::string& name) override;
    void impl_activate () override;
    void impl_deactivate () override;
    void set_impl (PhyObjImpl* impl) { _impl = impl; }
    World* get_world () { return _world; }
    virtual void update ();
    PhyObjImpl* get_impl () { return _impl; }
    double density () { return raw_props.density; }
    double friction () { return raw_props.friction; }
    void collision () { _collision->activate (); }
    bool update_from_engine () { return _update_from_engine; }
    double x () { return raw_props.x; }
    double y () { return raw_props.y; }
    double z () { return raw_props.z; }
    double dx () { return raw_props.dx; }
    double dy () { return raw_props.dy; }
    double dz () { return raw_props.dz; }
  protected:
    struct raw_props_t { double x, y, z, dx, dy, dz, roll, pitch, yall, mass, density, friction; };
    raw_props_t raw_props;
    DoublePropertyProxy *_x, *_y, *_z, *_dx, *_dy, *_dz, *_roll, *_pitch, *_yall, *_mass, *_density, *_friction;
    D3PhyObjUpdatePosition *_position_action;
    D3PhyObjUpdateVelocity *_velocity_action;
    Coupling *_cx, *_cy, *_cz, *_cdx, *_cdy, *_cdz;
    bool _update_from_engine;
    World *_world;
    PhyObjImpl* _impl;
    Spike *_collision;
  };

  class Box : public PhyObj {
  public:
    Box (ParentProcess* parent, const std::string& name, double x, double y, double z, double w, double h, double d, double mass);
    virtual ~Box ();
    void impl_activate () override;
    void impl_deactivate () override;
    FatProcess* find_child (const std::string& n) override;
  private:
    double w, h, d;
    DoublePropertyProxy *_w, *_h, *_d;
  };

  class Plane : public PhyObj {
  public:
    Plane (ParentProcess* parent, const std::string& name, double a, double b, double c, double d);
    virtual ~Plane ();
    void impl_activate () override;
    void impl_deactivate () override;
    FatProcess* find_child (const std::string& n) override;
    void update () override {};
  protected:
    struct plane_raw_props_t { double a, b, c, d ;};
    plane_raw_props_t _plane_props;
    DoublePropertyProxy *_a, *_b, *_c, *_d;
  };

  class Sphere : public PhyObj {
  public:
    Sphere (ParentProcess* parent, const std::string& name, double x, double y, double z, double radius, double mass);
    virtual ~Sphere ();
    void impl_activate () override;
    void impl_deactivate () override;
    FatProcess* find_child (const std::string& n) override;
  private:
    double radius;
    DoublePropertyProxy *_radius;
  };
}
