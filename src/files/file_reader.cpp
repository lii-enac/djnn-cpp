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

#include <fstream>
#include "files.h"

#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"

namespace djnn
{
  void
  FileReader::FileReaderAction::impl_activate ()
  {
    ((FileReader*)_parent)->read ();
  }

  FileReader::FileReader (Process *p, const string &n, const string& filename) : Process (p, n)
  {
    _input = new TextProperty (this, "input", filename);
    _output = new TextProperty (this, "output", "");
    _action = new FileReaderAction (this, "action");
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION, true);
    Process::finalize_construction (p);
  }

  FileReader::~FileReader ()
  {
    delete _c_input;
    delete _input;
    delete _output;
    delete _action;
  }

  void
  FileReader::impl_activate ()
  {
    _c_input->enable ();
    read ();
  }

  void
  FileReader::impl_deactivate ()
  {
    _c_input->disable ();
  }

  void
  FileReader::read ()
  {
    string filename = _input->get_value ();
    ifstream file (filename);
    if (file.is_open()) {
      string line;
      while (getline(file, line)) {
        _output->set_value (line, true);
      }
      file.close();
    }
  }

  void
  FileReader::serialize (const string &type)
  {
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("files:file-reader");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
}

