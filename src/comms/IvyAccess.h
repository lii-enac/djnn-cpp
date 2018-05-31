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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "../core/syshook/syshook.h"
#include "../core/execution/graph.h"
#include "../core/process.h"
#include "../core/syshook/external_source.h"

#include <iostream>

//#define __IVY_DEBUG__

namespace djnn
{


  

  class IvyAccess : public Process, public ExternalSource
  {

  /*** private Class Ivy Actions ***/
  private:
     class IvyAction : public Process
    {
    private:
      TextProperty *  _out;
    public:
      IvyAction (Process* parent, const string &name, TextProperty* out) :
      Process (parent, name), _out (out) {}
      virtual ~IvyAction () {}
      void activate () override;
      void deactivate () override {}
    };


  /*** Ivy Access Class ***/

  public:
    IvyAccess (Process *p, const std::string& n, 
      const std::string& bus="224.1.2.3:2010", const std::string& appname="NO_NAME", const std::string& ready="READY");
    
    virtual ~IvyAccess ();

    void set_arriving(string v) {
      _arriving->set_value (v, true);
      Graph::instance().exec();
    }

    void set_leaving(string v) {
      _leaving->set_value (v, true);
      Graph::instance().exec();
    }

  protected:
    void activate () override;
    void deactivate () override;
    Process* find_component (const string&) override;

  private:
    std::string _bus;
    std::string _appname;
    std::string _ready_message;

    std::vector<TextProperty*> _in;
    TextProperty* _out;
    Coupling*  _out_c;
    IvyAction*    _out_a;
    TextProperty* _arriving;
    TextProperty* _leaving;

    // thread source
    void run () override;
  };

}
