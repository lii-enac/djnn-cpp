#pragma once

#include "picking.h"

namespace djnn {

	class AnalyticalPicking : public Picking {
	public:
    	AnalyticalPicking (Window *win);
    	virtual ~AnalyticalPicking ();
    	virtual void init ();
    	virtual AbstractGShape* pick (double x, double y);
    	virtual void add_gobj (AbstractGShape* gobj);
	};

}
