/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "../external_source.h"
#include "../../process.h"

namespace djnn {
	class IOFD : public Process, public ExternalSource {
	public:
		IOFD(int readfd);

		int readfd() const { return _readfd; }

	protected:
		// abstract_component
		virtual void activate () override;
    	virtual void deactivate () override;
	private:

		void run () override;

		shared_ptr <Process> _readable;
		int _readfd;
	};
}
