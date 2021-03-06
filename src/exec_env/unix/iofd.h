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

#include "core/ontology/process.h"
#include "exec_env/external_source.h"
#include "core/tree/spike.h"

namespace djnn {
	class IOFD : public FatProcess, public ExternalSource {
	public:
		IOFD(ParentProcess* parent, const std::string& name, int readfd);
		virtual ~IOFD() override;

		int readfd() const { return _readfd; }

	protected:
		// FatProcess
		virtual void impl_activate () override;
    	virtual void impl_deactivate () override;
	private:
		// ExternalSource
		void run () override;
		
		int _readfd;
		Spike _readable;
	};
}
