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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "process.h"
#include "coupling.h"

#include "../execution/graph.h"
#include "../utils/uri.h"
#include "../utils/error.h"

#include <algorithm>
#include <iostream>

namespace djnn
{
  using namespace std;

  int Process::_nb_anonymous = 0;

  void
  alias_children (Process* p, Process* from)
  {
    map<string, Process*>& symtable = from->symtable ();
    for (auto& sym : symtable) {
      p->add_symbol (sym.first, sym.second);
    }
  }

  void
  alias (Process *p, const string &name, Process* from)
  {
    p->add_symbol (name, from);
  }

  void
  merge_children (Process *p1, const string &sy1, Process* p2, const string &sy2)
  {
    Process* x2 = p2->find_component (sy2);
    if (x2 == nullptr) {
      cerr << "trying to merge unknown child " << sy2 << endl;
      return;
    }
    Process* x1 = p1->find_component (sy1);
    if (x1 == nullptr) {
      cerr << "trying to merge unknown child " << sy1 << endl;
      return;
    }
    for (auto& c : x2->get_activation_couplings ()) {
      x1->add_activation_coupling (c);
    }
    for (auto& c : x2->get_deactivation_couplings ()) {
      x1->add_deactivation_coupling (c);
    }
    p2->remove_child (sy2);
    p2->add_symbol (sy2, x1);
    //delete (x2); // hum, are we really sure about this?
  }

  void
  Process::finalize_construction ()
  {
    if (_parent != nullptr)
      _parent->add_child (this, _name);
  }

  Process::Process (Process* parent, const string& name, bool model) :
      _vertex (nullptr), _parent (parent), _state_dependency (nullptr), _data (nullptr)//, _activation_state (deactivated)
  {
    set_is_model (model);
    set_activation_flag (NONE_ACTIVATION);
    set_activation_state (DEACTIVATED);
    _name = name.length () > 0 ? name : "anonymous_" + to_string (++_nb_anonymous);
    if (_parent != nullptr)
      _state_dependency = _parent->_state_dependency;
    if (Context::instance ()->line () != -1) {
      _dbg_info = std::string ("File: ") + Context::instance ()->filename () + " line: " + std::to_string (Context::instance ()->line ());
    } else
      _dbg_info = "no debug info";
  }

  Process::Process (bool model) :
      _vertex (nullptr), _parent (nullptr), _state_dependency (nullptr), _data (nullptr)//, _activation_state (deactivated)
  {
    set_is_model (model);
    set_activation_flag (NONE_ACTIVATION);
    set_activation_state (DEACTIVATED);
    _name = "anonymous_" + to_string (++_nb_anonymous);
    if (Context::instance ()->line () != -1) {
      _dbg_info = std::string ("File: ") + Context::instance ()->filename () + " line: " + std::to_string (Context::instance ()->line ());
    } else
      _dbg_info = "no debug info";
  }

  Process::~Process ()
  {
    if (_vertex != nullptr)
      _vertex->invalidate ();
  }

  void
  Process::activation ()
  {
    if (pre_activate ()) {
      activate ();
      post_activate ();
    }
  }

  void
  Process::deactivation ()
  {
    if (pre_deactivate ()) {
      deactivate ();
      post_deactivate ();
    }
  }

  void
  Process::remove_activation_coupling (Coupling* c)
  {
    _activation_couplings.erase (std::remove (_activation_couplings.begin (), _activation_couplings.end (), c),
                                 _activation_couplings.end ());
  }

  void
  Process::remove_deactivation_coupling (Coupling* c)
  {
    _deactivation_couplings.erase (std::remove (_deactivation_couplings.begin (), _deactivation_couplings.end (), c),
                                   _deactivation_couplings.end ());
  }

  Process*
  Process::find_component (const string& key)
  {
    //DEBUG
    //cout << "key: " << key << endl;

    if (key.length () == 0)
      return this;

    /* special case find '*' */ 
    if (key[0] == '*') {
      Process* found = find_component (key.substr(2)); // without "/*""
      if (!found) {
        /* we iterate in depth on each child and stop on the first 'key' found*/
        auto it = _symtable.begin ();
        while ( it != _symtable.end ()) {
          found = it->second->find_component (key); // with "/*""
          if (found) return found;
          ++it;
        }
      }
      return found;
    }

    /* special case find from root - using find_component ("//johndoe") */
    //FIXME: improved with c++20 if (key.starts_with("//")
    if (key.length () > 2 && key[0] == '/' && key[1] == '/') {
        Process* current_cpnt = this;
        Process* current_parent = current_cpnt->_parent;
        while (current_parent != nullptr) {
          current_cpnt = current_parent;
          current_parent = current_cpnt->_parent;
        }
        //DEBUG
        //cout << "root found: " << current_cpnt->_name << endl;
        return current_cpnt->find_component (key.substr(2));
      }

    size_t found = key.find_first_of ('/');
    if (found != string::npos) {
      string newKey = key.substr (0, found);
      string path = key.substr (found + 1);
      if (newKey[0] == '.' && newKey[1] == '.') {
        if (_parent)
          return _parent->find_component (path);
        else
          return nullptr;
      }
      map<string, Process*>::iterator it = _symtable.find (newKey);
      if (it != _symtable.end ()) {
        return (it->second)->find_component (path);
      }
    }
    if (key[0] == '.' && key[1] == '.')
      return _parent;
    map<string, Process*>::iterator it = _symtable.find (key);
    if (it != _symtable.end ()) {
      return it->second;
    }
    return 0;
  }

  Process*
  Process::find_component (Process *p, const string &path)
  {
    if (p == nullptr)
      return URI::find_by_uri (path);
    return p->find_component (path);
  }

  string
  Process::find_component_name (Process* symbol)
  {

    // FIXME : low efficiency function cause by linear search. use with care !

    map<string, Process*>::iterator it;

    for (it = _symtable.begin(); it != _symtable.end(); ++it)
    {
      if (it->second == symbol)
      {
        //debug
        //cerr << "key found : " << it->first << endl;
        return it->first;
      }
    }

    string key = "name_not_found";
    return key;
;
  }

  void
  Process::remove_symbol (const string& name)
  {
    map<string, Process*>::iterator it = _symtable.find (name);
    if (it != _symtable.end ())
      _symtable.erase (it);
    else
      cerr << "Warning: symbol " << name << " not found in component " << name << "\n";
  }

  void
  Process::remove_child (Process* c)
  {
    std::map<std::string, Process*>::iterator it;
    for (it = _symtable.begin (); it != _symtable.end (); ++it)
      if (it->second == c) {
        remove_symbol (it->first);
        return;
      }
  }

  void
  Process::remove_child (const string& name)
  {
    remove_symbol (name);
  }

  void
  Process::notify_activation ()
  {
    couplings_t couplings_cpy = _activation_couplings;
    /* WARNING: disputable choice.
     * The immediate propagation could disable some couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the activation. Thus the disabling of a coupling will be effective only on the next run.
     * */
    std::vector<Coupling*> to_propagate;
    for (auto& coupling : couplings_cpy) {
      if (coupling->is_enabled ())
        to_propagate.push_back (coupling);
    }
    for (auto& coupling : to_propagate) {
      coupling->propagate_activation ();
    }
  }

  void
  Process::notify_deactivation ()
  {
    couplings_t couplings_cpy = _deactivation_couplings;
    /* WARNING: disputable choice.
     * The immediate propagation could disable some couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the deactivation. Thus the disabling of a coupling will be effective only on the next run.
     * */
    std::vector<Coupling*> to_propagate;
    for (auto& coupling : couplings_cpy) {
      if (coupling->is_enabled ())
        to_propagate.push_back (coupling);
    }
    for (auto& coupling : to_propagate) {
      coupling->propagate_deactivation ();
    }
  }

  void
  Process::add_activation_coupling (Coupling* c)
  {
    _activation_couplings.push_back (c);
  }

  void
  Process::add_deactivation_coupling (Coupling* c)
  {
    _deactivation_couplings.push_back (c);
  }

  void
  Process::add_symbol (const string &name, Process* c)
  {
    /* if ((_symtable.insert (std::pair<string, Process*> (name, c))).second == false) {
     cerr << "Duplicate name " << name << " in component " << _name << endl;
     }*/
    _symtable[name] = c;
  }

  void
  Process::add_child (Process* c, const string& name)
  {
    if (c == nullptr)
      return;
    add_symbol (name, c);
  }

  bool
  Process::pre_activate ()
  {
    /* no activation if :
     * 1 - already activated
     * 2 - is activating
     * 3 - the parent exists and is stopped
     */
    if (get_activation_state () != DEACTIVATED || (_parent != 0 && !_parent->somehow_activating() ))
      return false;
    set_activation_state (ACTIVATING);
    return true;
  }

  void
  Process::post_activate ()
  {
    notify_activation ();
    set_activation_state (ACTIVATED);
  }

  bool
  Process::pre_deactivate ()
  {
    if (get_activation_state() != ACTIVATED)
      return false;
    set_activation_state (DEACTIVATING);
    return true;
  }

  void
  Process::post_deactivate ()
  {
    notify_deactivation ();
    set_activation_state (DEACTIVATED);
    set_activation_flag (NONE_ACTIVATION);
  }

  Process*
  Process::get_parent ()
  {
    return _parent;
  }

  const string&
  Process::get_name () const
  {
    return _name;
  }

  const Process::couplings_t&
  Process::get_activation_couplings () const
  {
    return _activation_couplings;
  }

  const Process::couplings_t&
  Process::get_deactivation_couplings () const
  {
    return _deactivation_couplings;
  }

  void
  Process::set_data (Process* data)
  {
    _data = data;
  }

  Process*
  Process::get_data ()
  {
    return _data;
  }

  void
  Process::serialize (const string& format) { cout << "serialize is not yet implemented for '" << _name << "'" << endl; }
  
  Process*
  Process::clone () { cout << "clone not implemented for " << _name << "\n"; return nullptr; };

  static int indent = -1;
  void
  Process::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name) << ": ";

    /* check if the component is empty - should be ?*/
    if (_symtable.empty ()) {
      cout << "<EMPTY>" << endl;
      return;
    }

    /* check if the level is reached */
    if ((level != 0) && (indent == level - 1))
      return;

    cout << endl;
    indent++;
    std::map<string, Process*>::iterator it;
    int i = 0;
    for (it = _symtable.begin (); it != _symtable.end (); ++it) {
      for (int j = 0; j < indent; j++)
        cout << "|\t";
      cout << " +" << i++ << " ";
      it->second->dump (level);
      if (it->second->get_cpnt_type() != COMPONENT_T || indent == level - 1)
        cout << endl;
    }
    indent--;
  }
}
