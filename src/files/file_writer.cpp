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

#include <iostream>
#include <fstream>
#include <iomanip>
#include "file_writer.h"

#include "core/execution/graph.h"
#include "core/serializer/serializer.h"


namespace djnn
{
  void
  FileWriter::FileWriterAction::impl_activate ()
  {
    ((FileWriter*)get_parent ())->write ();
  }

  void
  FileWriter::FileNameAction::impl_activate ()
  {
    ((FileWriter*)get_parent ())->change_file ();
  }

  FileWriter::FileWriter (Process *parent, const std::string &name, const std::string& filename)
  : Process (name), _input (this, "input", ""), _filename (this, "filename", filename),
  _fn_action (this, "fn_action"), _action (this, "action"),
  _c_input (&_input, ACTIVATION, &_action, ACTIVATION, true),
  _c_filename (&_filename, ACTIVATION, &_fn_action, ACTIVATION, true),
  _fs (filename, std::ofstream::out | std::ofstream::trunc)
  {
    Process::finalize_construction (parent, name);
  }

  FileWriter::~FileWriter ()
  {
  }

  void
  FileWriter::impl_activate ()
  {
    _c_input.enable ();
    _c_filename.enable ();
    if (_fs.is_open ()) {
      _fs.close ();
    }
    _fs.open (_filename.get_value (), std::ofstream::out | std::ofstream::trunc);
  }

  void
  FileWriter::impl_deactivate ()
  {
    _c_input.disable ();
    _c_filename.disable ();
    _fs.close();
    
  }

  void
  FileWriter::change_file ()
  {
    if (_fs.is_open ()) {
      _fs.close ();
    }
    _fs.open (_filename.get_value (), std::ofstream::out | std::ofstream::trunc);
  }

  void
  FileWriter::write ()
  {
    _fs << _input.get_value ();
    _fs.flush ();
  }
}


