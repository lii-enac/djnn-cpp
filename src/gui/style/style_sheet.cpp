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

#include "gui/style/style_sheet.h"

#include "core/utils/error.h"

namespace djnn
{
  static map <string, int> name_to_id;
  static vector<StyleSheet*> style_array;

  StyleSheet::StyleSheet (ParentProcess* parent, const string& n) :
      Container (parent, n), _classname (n)
  {
    map<string,int>::iterator it = name_to_id.find(n);
    if (it != name_to_id.end()) {
      error(this, "Style " + n + " already defined");
    }
    style_array.push_back (this);
    _id = style_array.size () - 1;
    name_to_id.insert (pair<string, int> (n, _id));
    finalize_construction (parent, n);
  }

  StyleSheet*
  StyleSheet::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    /*StyleSheet* newd = new StyleSheet (nullptr, get_name ());

    for (auto c : _children) {
      newd->add_child (c->clone (), this->find_child_name(c));
    }

    return newd;*/

    auto * clone = new StyleSheet (nullptr, get_name ());
    for (auto c : _children) {
      auto cclone = c->impl_clone (origs_clones);
      //origs_clones[c] = cclone;
      clone->add_child (cclone , this->find_child_name(c));
    }
    origs_clones[this] = clone;
    return clone;
  }

  int
  StyleSheet::get_id (const string& classname)
  {
    map<string,int>::iterator it = name_to_id.find(classname);
    if (it != name_to_id.end()) {
      return it->second;
    }
    return -1;
  }

  void
  StyleSheet::draw_style (const vector<int>& classes)
  {
    for (auto i: classes) {
      StyleSheet *s = style_array.at (i);
      for (auto c: s->children ()) {
        c->draw ();
      }
    }
  }
}
