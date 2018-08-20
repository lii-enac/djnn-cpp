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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "../core/tree/process.h"
#include "../core/tree/abstract_property.h"
#include "../core/control/coupling.h"
#include "../core/control/assignment.h"

#include <iostream>

namespace djnn {
  using namespace std;

  class Connector : public Process
  {
    friend class PausedConnector;
   
  private:
    class ConnectorAction : public Process
    {
    public:
      ConnectorAction (Process* p, const string &n, AbstractProperty* src, AbstractProperty* dst, bool propagate) :
	Process (p, n), _src (src), _dst (dst), _propagate (propagate) {};
      virtual ~ConnectorAction () {};
      void activate ();
      void deactivate () {};
    private:
      AbstractProperty* _src;
      AbstractProperty* _dst;
      bool _propagate;
    };

  public:
    Connector (Process *p, string n, Process *src, string ispec, Process *dst, string dspec);
    Connector (Process *src, string ispec, Process *dst, string dspec);
    void activate () override { _c_src->enable (); _action->activation ();}
    void deactivate () override { _c_src->disable(); _action->deactivation ();}
    void serialize (const string& type) override;
    virtual ~Connector ();

  protected:
    void init_connector (Process *src, string ispec, Process *dst, string dspec);
    AbstractProperty* _src;
    AbstractProperty* _dst;
    unique_ptr<Coupling>_c_src;
    shared_ptr<Process> _action;
  };

  class PausedConnector : public Process
  {
  public:
    PausedConnector (Process *p, string n, Process *src, string ispec, Process *dst, string dspec);
    PausedConnector (Process *src, string ispec, Process *dst, string dspec);
    void activate () override { _c_src->enable (); _action->activation ();}
    void deactivate () override { _c_src->disable(); _action->deactivation ();}
    void serialize (const string& type) override;
    virtual ~PausedConnector ();

  protected:
    void init_pausedconnector (Process *src, string ispec, Process *dst, string dspec);
    AbstractProperty* _src;
    AbstractProperty* _dst;
    unique_ptr<Coupling>_c_src;
    shared_ptr<Process> _action;
  };
}
