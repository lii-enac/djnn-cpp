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

#include "syshook/global_mutex.h"
#include "core/execution/graph.h"
#include "core/ontology/process.h"
#include "syshook/external_source.h"

//#define __IVY_DEBUG__

namespace djnn
{

  using namespace std;
  

  class IvyAccess : public Process, public ExternalSource
  {

  /*** private Class Ivy Out Actions ***/ 
  private:
     class IvyOutAction : public Action
    {
    public:
      IvyOutAction (Process* parent, const string &name, TextProperty* out) :
      Action (parent, name), _out (out) { Process::finalize_construction (parent, name); } 
      virtual ~IvyOutAction () {}
      void coupling_activation_hook () override;
      void impl_activate () override {};
      void impl_deactivate () override {}
    private:
      TextProperty* _out;
    };


  /*** Ivy Access Class ***/

  public:
    IvyAccess (Process *parent, const std::string& name, 
      const std::string& bus="224.1.2.3:2010", const std::string& appname="NO_NAME", const std::string& ready="READY", bool isModel=false);
    
    virtual ~IvyAccess ();
    void set_arriving(string v);
    void set_leaving(string v);

    typedef map<string, vector<pair<int, TextProperty*>>> in_map_t;
    typedef pair<string, in_map_t*> regexp_keypair_t;

    // make it public
    bool get_please_stop() const override { return ExternalSource::get_please_stop(); }

  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    Process* find_component (const string&) override;
    void set_parent (Process* p) override;

  private:
    string _bus;
    string _appname;
    string _ready_message;

    in_map_t _in_map; 
      
    TextProperty _out;
    IvyOutAction _out_a;
    Coupling _out_c;
    TextProperty _arriving;
    TextProperty _leaving;

    // thread source
    void run () override;
  };

}
