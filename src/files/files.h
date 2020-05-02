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
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/text_property.h"
#include "core/control/action.h"
#include "files/file_writer.h"

namespace djnn
{

  extern std::vector<std::string> loadedModules; 

  void init_files ();

  class FileReader : public FatProcess
  {
  private:
    class FileReaderAction : public Action
    {
    public:
      FileReaderAction (FatProcess *parent, const std::string& name) : Action (parent, name) {}
      void impl_activate () override;
      void impl_deactivate () override {}
    };
  public:
    FileReader (FatProcess *parent, const std::string& name, const std::string& filename);
    virtual ~FileReader ();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const std::string& type) override;
    void read ();
  private:
    TextProperty *_input, *_output;
    Coupling* _c_input;
    FileReaderAction* _action;
  };
}
