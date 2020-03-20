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

#include <vector>
#include "physics.h"
#include "physics_backend.h"

namespace djnn
{
  //using namespace std;

  class AbstractPhysicsBackend
  {
  public:
    AbstractPhysicsBackend () {
    }

    virtual
    ~AbstractPhysicsBackend ()
    {
    }
    
    virtual WorldImpl* create_world (World *world, double x, double y, double z) = 0;

    virtual void enable_physical_object (PhyObj *phy_obj) {}
    virtual void disable_physical_object (PhyObj *phy_obj) {}
    virtual void create_box (PhyObj* phy_obj, World *world, double posX, double posY, double posZ, double width, double height, double depth, double mass) = 0;
    virtual void create_sphere (PhyObj* phy_obj, World *world, double x, double y, double z, double radius, double mass) {}
    virtual void create_plane (PhyObj* phy_obj, World *world, double a, double b, double c, double d) {}
    virtual void destroy_plane (PhyObj* phy_obj, World *world) {}
    virtual void destroy_body (PhyObj* phy_obj) {}
    virtual void update_position (PhyObj* phy_obj) {}
    virtual void update_velocity (PhyObj* phy_obj) {}
    virtual void get_position (PhyObj* phy_obj, double &x, double &y, double &z) {}
    virtual void get_angle (PhyObj* phy_obj, double &ax, double &ay, double &az) {}
    virtual void get_velocity (PhyObj* phy_obj, double &dx, double &dy, double &dz) {}
  };
}
