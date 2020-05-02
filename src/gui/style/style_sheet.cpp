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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include <gui/style/style.h>
#include "core/utils/error.h"

namespace djnn
{
  static std::map <std::string, int> name_to_id;
  static std::vector<StyleSheet*> style_array;

  StyleSheet::StyleSheet (FatProcess* parent, const std::string& n) :
      Container (parent, n), _classname (n)
  {
    std::map<std::string,int>::iterator it = name_to_id.find(n);
    if (it != name_to_id.end()) {
      error(this, "Style " + n + " already defined");
    }
    style_array.push_back (this);
    _id = style_array.size () - 1;
    name_to_id.insert (std::pair<std::string, int> (n, _id));
    finalize_construction (parent, n);
  }

  StyleSheet*
  StyleSheet::clone ()
  {
    StyleSheet* newd = new StyleSheet (nullptr, get_name ());

    for (auto c : _children) {
      newd->add_child (c->clone (), this->find_child_name(c));
    }

    return newd;
  }

  int
  StyleSheet::get_id (const std::string& classname)
  {
    std::map<std::string,int>::iterator it = name_to_id.find(classname);
    if (it != name_to_id.end()) {
      return it->second;
    }
    return -1;
  }

  void
  StyleSheet::draw_style (const std::vector<int>& classes)
  {
    for (auto i: classes) {
      StyleSheet *s = style_array.at (i);
      for (auto c: s->children ()) {
        c->draw ();
      }
    }
  }
}
