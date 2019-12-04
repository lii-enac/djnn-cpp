#pragma once

#include "picking.h"

namespace djnn {

	class PickAnalyticalContext {
	public:
		double x;
		double y;
		double half_outline_width;
		bool filled;
		bool outlined;
	};

	class AnalyticalPicking : public Picking {
	public:
    	AnalyticalPicking (Window *win);
    	virtual ~AnalyticalPicking ();
    	virtual void init ();
    	virtual AbstractGShape* pick (double x, double y);
    	virtual void add_gobj (AbstractGShape* gobj);
	};

}
