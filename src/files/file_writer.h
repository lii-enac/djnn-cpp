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
 *      Alice Martin <alice.mona.martin@outlook.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/property/text_property.h"
#include "core/control/action.h"
#include <fstream>

namespace djnn
{
  class FileWriter : public FatProcess
  {
  private:
    class FileWriterAction : public Action
    {
    public:
      FileWriterAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
      void impl_activate () override;
    };
    class FileNameAction : public Action
    {
    public:
      FileNameAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
      void impl_activate () override;
    };
  public:
    FileWriter (CoreProcess* parent, const string& name, const string& filename);
    virtual ~FileWriter ();
    void impl_activate () override;
    void impl_deactivate () override;
    void write ();
    void change_file ();
   
  private:
    TextProperty _input, _filename;
    FileNameAction _fn_action;
    FileWriterAction _action;
    Coupling _c_input, _c_filename;
    std::ofstream _fs;

  };
}