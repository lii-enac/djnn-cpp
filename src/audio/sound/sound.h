#pragma once

#include "audio/abstract_sobj.h"

namespace djnn {


	class Sound : public AbstractSObj
	{
	public:
		Sound (Process* parent, const std::string& name);
		virtual ~Sound ();
	protected:

	};

}