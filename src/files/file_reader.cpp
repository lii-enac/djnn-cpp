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

#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
#include "files.h"

namespace djnn {
void
FileReader::FileReaderAction::impl_activate ()
{
    ((FileReader*)get_parent ())->read ();
}

FileReader::FileReader (CoreProcess* parent, const string& name, const string& filename)
    : FatProcess (name)
{
    _input   = new TextProperty (this, "input", filename);
    _output  = new TextProperty (this, "output", "");
    _action  = new FileReaderAction (this, "action");
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION);
    finalize_construction (parent, name);
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
    string        filename = _input->get_value ();
    std::ifstream file (filename.c_str ());

    // https://stackoverflow.com/a/2602258
    if (file.is_open ()) {
        file.seekg (0, std::ios::end);
        size_t size = file.tellg ();
        string buffer (size, ' ');
        file.seekg (0);
        file.read (&buffer[0], size);
        _output->set_value (buffer, true);
    }

    /*if (file.is_open()) {
      string buff;
      string line;
      while (getline(file, line)) {
        buff.append (line + "\n");
      }
      _output->set_value (buff, true);
      file.close();
    }*/
}

#ifndef DJNN_NO_SERIALIZE
void
FileReader::serialize (const string& type)
{
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("files:file-reader");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif
} // namespace djnn
