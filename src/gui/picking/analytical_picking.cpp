#include "analytical_picking.h"
#include "analytical_picking_context.h"
#include "display/window.h"

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
   		FatProcess *p = _win->holder ();
      PickAnalyticalContext pac;
      pac.x = x;
      pac.y = y;
      pac.half_outline_width=0; // FIXME should be default value
      pac.filled = true; 
      pac.outlined = false;
      pac.clipped = false;
      
      AbstractGShape * picked = p->pick_analytical (pac);
      //std::cerr << picked << __FL__;
	    return picked;
   	}

    void
    AnalyticalPicking::add_gobj (AbstractGShape* gobj, bool cache)
    {
    }

}
