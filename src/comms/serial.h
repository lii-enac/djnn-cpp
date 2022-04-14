/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/property/text_property.h"
#include "core/property/int_property.h"

#include "exec_env/global_mutex.h"
#include "exec_env/external_source.h"




namespace djnn
{

  class Serial : public FatProcess, public ExternalSource
  {

  /*** private Class Serial Out Actions ***/
  private:
     class SerialOutAction : public Action
    {
    public:
      SerialOutAction (ParentProcess* parent, const string& name) :
      Action (parent, name) { finalize_construction (parent, name); }
      virtual ~SerialOutAction () {}
      void impl_activate () override { ((Serial*)get_parent())->serial_write (); };
    };


  /*** Serial Class ***/

  public:
    Serial (ParentProcess* parent, const string& name,
      const string& port, int baudrate=9600, char eol='\n');
    
    virtual ~Serial ();
    void serial_write ();

    // make it public
    bool get_please_stop() const override { return ExternalSource::get_please_stop(); }

  protected:
    void impl_activate () override;
    void impl_deactivate () override;
    void set_parent (ParentProcess* parent) override;

  private:
    const string& _port;
    void init_comm ();
    TextProperty _out, _in;
    IntProperty _baudrate;
    SerialOutAction _out_a;
    Coupling _out_c;
    int _buf_max;
    int _fd;
    char _eol;

    // thread source
    void run () override;
  };

}
