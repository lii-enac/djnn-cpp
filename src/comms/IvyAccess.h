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
#include "../core/ontology/process.h"
#include "../core/syshook/external_source.h"

#include <iostream>

//#define __IVY_DEBUG__

namespace djnn
{

  using namespace std;
  

  class IvyAccess : public Process, public ExternalSource
  {

  /*** private Class Ivy Out Actions ***/ 
  private:
     class IvyOutAction : public Process
    {
    public:
      IvyOutAction (Process* parent, const string &name, TextProperty* out) :
      Process (parent, name), _out (out) { Process::finalize_construction (); }
      virtual ~IvyOutAction () {}
      void coupling_activation_hook () override;
      void impl_activate () override {};
      void impl_deactivate () override {}
    private:
      TextProperty* _out;
    };


  /*** Ivy Access Class ***/

  public:
    IvyAccess (Process *p, const std::string& n, 
      const std::string& bus="224.1.2.3:2010", const std::string& appname="NO_NAME", const std::string& ready="READY", bool isModel=false);
    
    virtual ~IvyAccess ();

    void set_arriving(string v);
    void set_leaving(string v);
  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    Process* find_component (const string&) override;

  private:
    string _bus;
    string _appname;
    string _ready_message;

    map<string, vector<pair<int, TextProperty*>>> _in_map;   
    TextProperty* _out;
    Coupling*  _out_c;
    IvyOutAction*    _out_a;
    TextProperty* _arriving;
    TextProperty* _leaving;

    // thread source
    void run () override;
  };

}
