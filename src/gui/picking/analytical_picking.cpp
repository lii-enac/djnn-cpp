#include "analytical_picking.h"
#include "analytical_picking_context.h"

#include <iostream>

namespace djnn {
  	AnalyticalPicking::AnalyticalPicking (Window *win)
  	: Picking(win)
  	{
  	}

    AnalyticalPicking::~AnalyticalPicking ()
    {
    }
	
  	void
  	AnalyticalPicking::init ()
  	{
  	}
   	
   	AbstractGShape*
   	AnalyticalPicking::pick (double x, double y)
   	{
   		Process *p = _win->holder ();
      PickAnalyticalContext pac;
      pac.x = x;
      pac.y = y;
      pac.half_outline_width=0; // FIXME should be default value
      pac.filled = true; 
      pac.outlined = false;
      pac.clipped = false;
      
	    return p->pick_analytical (pac);
   	}

    void
    AnalyticalPicking::add_gobj (AbstractGShape* gobj)
    {
    }

}