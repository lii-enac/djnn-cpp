/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/property/text_property.h"

#include "exec_env/global_mutex.h"
#include "exec_env/external_source.h"



//#define __IVY_DEBUG__

namespace djnn
{
 
  class IvyAccess : public FatProcess, public ExternalSource
  {

  /*** private Class Ivy Out Actions ***/ 
  private:
     class IvyOutAction : public Action
    {
    public:
      IvyOutAction (CoreProcess* parent, const string& name, TextProperty* out) :
      Action (parent, name), _out (out) { finalize_construction (parent, name); } 
      virtual ~IvyOutAction () {}
      //void coupling_activation_hook () override;
      void impl_activate () override;
    private:
      TextProperty* _out;
    };


  /*** Ivy Access Class ***/

  public:
    using string = CoreProcess::string; 
    typedef map<string, vector<djnnstl::pair<int, TextProperty*>>> in_map_t;
    typedef djnnstl::pair<string, in_map_t*> regexp_keypair_t;

    struct msg_callback_user_data {
      IvyAccess* access;
      regexp_keypair_t* keypair;
    };

    IvyAccess (CoreProcess* parent, const string& name, 
      const string& bus="224.1.2.3:2010", const string& appname="NO_NAME", const string& ready="READY", bool isModel=false);
    
    virtual ~IvyAccess ();
    void set_arriving(const string& v);
    void set_leaving(const string& v);

    // make it public
    bool get_please_stop() const override { return ExternalSource::get_please_stop(); }

    CoreProcess* find_child_impl (const string&) override;

  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    //void set_parent (CoreProcess* parent) override;

  private:
    string _bus;
    string _appname;
    string _ready_message;

    in_map_t _in_map; 
    vector<djnn::IvyAccess::msg_callback_user_data*> _cb_regex_vector;
      
    TextProperty _out;
    IvyOutAction _out_a;
    Coupling _out_c;
    TextProperty _arriving;
    TextProperty _leaving;

    // thread source
    void run () override;
  };

}
