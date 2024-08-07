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

#include <fstream>
#include <iomanip>

#include "file_writer.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
#include "core/utils/iostream.h"

using namespace djnnstl;

namespace djnn {
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

struct FileWriterImpl
{
    FileWriterImpl (const string& filename)
        : _fs (std::ofstream (filename.c_str (), std::ofstream::out | std::ofstream::trunc)) {}
    std::ofstream _fs;
};

FileWriter::FileWriter (CoreProcess* parent, const string& name, const string& filename)
    : FatProcess (name), _input (this, "input", ""), _filename (this, "filename", filename),
      _fn_action (this, "fn_action"), _action (this, "action"),
      _c_input (&_input, ACTIVATION, &_action, ACTIVATION),
      _c_filename (&_filename, ACTIVATION, &_fn_action, ACTIVATION),
      _impl (new FileWriterImpl (filename.c_str ()))
{
    finalize_construction (parent, name);
}

FileWriter::~FileWriter ()
{
    delete _impl;
}

void
FileWriter::impl_activate ()
{
    _c_input.enable ();
    _c_filename.enable ();
    if (_impl->_fs.is_open ()) {
        _impl->_fs.close ();
    }
    _impl->_fs.open (_filename.get_value ().c_str (), std::ofstream::out | std::ofstream::trunc);
}

void
FileWriter::impl_deactivate ()
{
    _c_input.disable ();
    _c_filename.disable ();
    _impl->_fs.close ();
}

void
FileWriter::change_file ()
{
    if (_impl->_fs.is_open ()) {
        _impl->_fs.close ();
    }
    _impl->_fs.open (_filename.get_value ().c_str (), std::ofstream::out | std::ofstream::trunc);
}

void
FileWriter::write ()
{
    _impl->_fs << _input.get_value ().c_str ();
    _impl->_fs.flush ();
}
} // namespace djnn
