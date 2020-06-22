#pragma once

#include "audio/abstract_sobj.h"

namespace djnn {


	class Sound : public AbstractSObj
	{
	public:
		Sound (ParentProcess* parent, const std::string& name);
		virtual ~Sound ();
	protected:

	};

}