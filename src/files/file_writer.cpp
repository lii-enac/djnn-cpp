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

#include "core/utils/iostream.h"
#include <fstream>
#include <iomanip>
#include "file_writer.h"

#include "core/core-dev.h" // graph add/remove edge
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

  FileWriter::FileWriter (ParentProcess* parent, const string& name, const string& filename)
  : FatProcess (name), _input (this, "input", ""), _filename (this, "filename", filename),
  _fn_action (this, "fn_action"), _action (this, "action"),
  _c_input (&_input, ACTIVATION, &_action, ACTIVATION),
  _c_filename (&_filename, ACTIVATION, &_fn_action, ACTIVATION),
  _fs (filename.c_str(), std::ofstream::out | std::ofstream::trunc)
  {
    finalize_construction (parent, name);
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
    _fs.open (_filename.get_value ().c_str (), std::ofstream::out | std::ofstream::trunc);
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
    _fs.open (_filename.get_value ().c_str (), std::ofstream::out | std::ofstream::trunc);
  }

  void
  FileWriter::write ()
  {
    _fs << _input.get_value ();
    _fs.flush ();
  }
}


