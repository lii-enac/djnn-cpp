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
#include <algorithm>
#include <functional>
#include <locale>

#include "utils.h"

#include "core/serializer/serializer.h"

using namespace std;

namespace djnn
{
  void
  Properties::PropertiesAction::impl_activate ()
  {
    ((Properties*)get_parent ())->add_property ();
  }

  Properties::Properties (Process *parent, const std::string& name, const std::string& filename)
  : Process (name)
  {
    _input = new TextProperty (this, "input", "");
    _action = new PropertiesAction (this, "action");
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION, true);
    if (!filename.empty()) {
      ifstream file (filename);
      if (file.is_open()) {
        string line;
        while (getline(file, line)) {
          _input->set_value (line, false);
          add_property ();
        }
      file.close();
      }
    }
    finalize_construction (parent, name);
  }

  Properties::~Properties ()
  {
    delete _c_input;
    delete _input;
    delete _action;
    symtable_t::iterator it;
    std::map<std::string, Process*>& map = children ();
    for (it = map.begin (); it != map.end (); ++it) {
      delete it->second;
    }
  }

  void
  Properties::impl_activate ()
  {
    _c_input->enable ();
  }

  void
  Properties::impl_deactivate ()
  {
    _c_input->disable ();
  }

  void
  Properties::add_property ()
  {
    string s = _input->get_value();
    size_t found = s.find_first_of ('=');
    if (found == string::npos)
      return;
    string key = strTrim (s.substr (0, found));
    string value = strTrim (s.substr (found + 1));
    new TextProperty (this, key, value);
  }

  void
  Properties::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("utils:properties");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  /* this code is adapted from
   * https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
   */
  typedef unsigned char BYTE;

  std::string
  strTrim (std::string s)
  {
    // convert all whitespace characters to a standard space
    std::replace_if (s.begin (), s.end (), (std::function<int (BYTE)>)::isspace, ' ');

    // remove leading and trailing spaces
    size_t f = s.find_first_not_of (' ');
    if (f == std::string::npos)
      return "";
    s = s.substr (f, s.find_last_not_of (' ') - f + 1);

    // remove consecutive spaces
    s = std::string (s.begin (), std::unique (s.begin (), s.end (), [](BYTE l, BYTE r) {return l == ' ' && r == ' ';}));

    return s;
  }
}

