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

#include "abstract_physics_backend.h"
#include <ode/ode.h>

namespace djnn
{
  using namespace std;

  class ODEBackend : public AbstractPhysicsBackend
  {
  public:
    static ODEBackend* instance ();
    virtual ~ODEBackend ();
    WorldImpl* create_world (World *world, double x, double y, double z = 0) override;
    virtual void create_plane (PhyObj* phy_obj, World *world, double a, double b, double c, double d) override;
    virtual void destroy_plane (PhyObj* phy_obj, World *world) override;
    void create_box (PhyObj* phy_obj, World *world, double posX, double posY, double posZ, double lx, double ly, double lz, double mass) override;
    void create_sphere (PhyObj* phy_obj, World *world, double x, double y, double z, double radius, double mass) override;
    void destroy_body (PhyObj* phy_obj) override;
    void update_position (PhyObj* phy_obj) override;
    void update_velocity (PhyObj* phy_obj) override;
    void get_position (PhyObj* phy_obj, double &x, double &y, double &z) override;
    void get_angle (PhyObj* phy_obj, double &ax, double &ay, double &az) override;
    void get_velocity (PhyObj* phy_obj, double &dx, double &dy, double &dz) override;
  private:
    static ODEBackend *_instance;
    static std::once_flag onceFlag;
    ODEBackend ();
  };

  class ODEWorld : public WorldImpl
  {
  public:
    ODEWorld (double dx, double dy, double dz);
    virtual ~ODEWorld ();
    void step (double dt) override;
    dWorldID get_world () { return _world_id; }
    dSpaceID get_space () { return _space_id; }
    dJointGroupID get_joint_group () { return _joint_group_id; }
  private:
    dWorldID _world_id;
    dSpaceID _space_id;
    dJointGroupID _joint_group_id;
  };

  class ODEPhyObj : public PhyObjImpl
  {
  public:
    ODEPhyObj (dBodyID body, dGeomID geom) : _body (body), _geom (geom) {}
    ODEPhyObj () : _body (nullptr), _geom (nullptr) {}
    virtual ~ODEPhyObj () {}
    void set_body (dBodyID body) { _body = body; }
    dBodyID get_body () { return _body; }
    void set_geom (dGeomID geom) { _geom = geom; }
    dGeomID get_geom () { return _geom; }
  private:
    dBodyID _body;
    dGeomID _geom;
  };
} /* namespace djnn */
