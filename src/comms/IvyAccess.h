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
#include "core/tree/text_property.h"

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
      IvyOutAction (ParentProcess* parent, const std::string& name, TextProperty* out) :
      Action (parent, name), _out (out) { finalize_construction (parent, name); } 
      virtual ~IvyOutAction () {}
      //void coupling_activation_hook () override;
      void impl_activate () override;
    private:
      TextProperty* _out;
    };


  /*** Ivy Access Class ***/

  public:
    typedef map<std::string, vector<pair<int, TextProperty*>>> in_map_t;
    typedef pair<std::string, in_map_t*> regexp_keypair_t;

    struct msg_callback_user_data {
      IvyAccess* access;
      regexp_keypair_t* keypair;
    };

    IvyAccess (ParentProcess* parent, const std::string& name, 
      const std::string& bus="224.1.2.3:2010", const std::string& appname="NO_NAME", const std::string& ready="READY", bool isModel=false);
    
    virtual ~IvyAccess ();
    void set_arriving(const std::string& v);
    void set_leaving(const std::string& v);

    // make it public
    bool get_please_stop() const override { return ExternalSource::get_please_stop(); }

    FatChildProcess* find_child_impl (const std::string&) override;

  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    //void set_parent (ParentProcess* parent) override;

  private:
    std::string _bus;
    std::string _appname;
    std::string _ready_message;

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
