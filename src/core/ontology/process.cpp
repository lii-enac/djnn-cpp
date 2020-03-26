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

#include "core/execution/graph.h"
#include "core/utils/uri.h"
#include "core/utils/error.h"
#include "core/utils/utils-dev.h"
#include "utils/debug.h"

#include <algorithm>

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif

namespace djnn
{
  using namespace std;

  long int Process::_nb_anonymous = 0;

#ifdef DJNN_NO_DEBUG
  Process::DebugInfo Process::_dbg_info{"no dbg info",0};
#endif

  string Process::default_name = "noname";
  static map<const Process*, string> parentless_names;

  Process::Process (const std::string& name, bool model) :
      _vertex (nullptr), _parent (nullptr), _state_dependency (nullptr), _data (nullptr)
  {
    set_is_model (model);
    set_activation_flag (NONE_ACTIVATION);
    set_activation_state (DEACTIVATED);
  
#ifdef DJNN_NO_DEBUG
#else
    if (Context::instance ()->line () != -1) {
      //_dbg_info = std::string ("File: ") + Context::instance ()->filename () + " line: " + std::to_string (Context::instance ()->line ());
      //_dbg_info = Context::instance ()->filename () + ":" + std::to_string (Context::instance ()->line ());
      _dbg_info = {Context::instance ()->filename (), Context::instance ()->line ()};
    } else {
      _dbg_info = {"no dbg info",0};
    }
#endif
    //_name = name.length () > 0 ? name : "anonymous_" + to_string (++_nb_anonymous);

    //debug
    //cerr << __PRETTY_FUNCTION__  << " - " << this << " - " << (get_parent () ? get_parent ()->get_name () + "/"  : "") << get_name ()  << endl;;
  }

  void
  Process::finalize_construction (Process* parent, const std::string& name, Process* state_dep) /* called by SubProcess to link to parent */
  {
    if (parent) {
      // by default state_dep is nullptr so _state_dependency depends on parent->state_dependenncy)
      if (state_dep == nullptr)
        _state_dependency = parent->state_dependency ();
      else
        _state_dependency = state_dep;

      parent->add_child (this, name);
    } else {
      parentless_names[this] = name;
    }
  }

  Process::~Process ()
  {
    //debug
    //cerr << __PRETTY_FUNCTION__  << " - " << this << " - " << (get_parent () ? get_parent ()->get_name () + "/"  : "") << get_name ()  << endl;

    /* note: 
       this code is to prevent bugs 
       this should NEVER happen
       _vertex should be nullptr at this place
       if not, something (Process)  IS NOT well deleted
    */
    for (auto c : _activation_couplings) {
      c->about_to_delete_src ();
    }
    for (auto c : _deactivation_couplings) {
      c->about_to_delete_src ();
    }
    if (_vertex != nullptr){
#ifndef DJNN_NO_DEBUG
       warning ( nullptr, " Process::~Process - " +  get_hierarchy_name (this)  + " - _vertex is NOT NULL and it should\n");
       for (auto &c: get_activation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (activation)" << __FL__;
       for (auto &c: get_deactivation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (deactivation)" << __FL__;
#endif
       _vertex->invalidate ();
    }

    /* make sure everything is wiped out the symtable */
    children ().clear ();
  }


  // main activation API

  void
  Process::activate ()
  {
    if (pre_activate ()) {
       impl_activate ();
       post_activate ();
    }
  }

  void
  Process::deactivate ()
  {
    if (pre_deactivate ()) {
      impl_deactivate ();
      post_deactivate ();
    }
  }

  bool
  Process::pre_activate ()
  {
    /* no activation if :
     * 1 - already activated
     * 2 - is activating
     * 3 - the parent exists and is stopped
     */
    /*if (
           get_activation_state () != DEACTIVATED
        || (
             get_parent () != nullptr
             && !get_parent ()->somehow_activating()
           )
       )
      return false;
    set_activation_state (ACTIVATING);*/
    return true;
  }

  void
  Process::post_activate ()
  {
    notify_activation ();
    set_activation_state (ACTIVATED);
    // TOCHECK: why not set_activation_flag (NONE_ACTIVATION); like post_deactivate ?
  }

  bool
  Process::pre_deactivate ()
  {
    if (get_activation_state() != ACTIVATED) // TOCHECK: why not || (get_parent () != 0 && !get_parent ()->somehow_deactivating() )) like pre_activate ? => OK
      return false;
    set_activation_state (DEACTIVATING);
    return true;
  }

  void
  Process::post_deactivate ()
  {
    notify_deactivation ();
    set_activation_state (DEACTIVATED);
    set_activation_flag (NONE_ACTIVATION); // already done in graph
  }

  void
  Process::notify_activation ()
  {
    /* WARNING: disputable choice.
     * The immediate propagation could disable some couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the activation. Thus the disabling of a coupling will be effective only on the next run.
     * */
    std::vector<Coupling*> to_propagate;
    for (auto& coupling : _activation_couplings) {
      if (coupling->is_enabled ())
        to_propagate.push_back (coupling);
    }
    for (auto& coupling : to_propagate) {
      coupling->propagate_activation ();
    }
  }

  void
  Process::schedule_activation ()
  {
    Graph::instance().schedule_activation (this);
  }

  void
  Process::schedule_delete ()
  {
    Graph::instance().schedule_delete (this);
  }

  void
  Process::notify_deactivation ()
  {
    /* WARNING: disputable choice.
     * The immediate propagation could disable some couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the deactivation. Thus the disabling of a coupling will be effective only on the next run.
     * */
    std::vector<Coupling*> to_propagate;
    for (auto& coupling : _deactivation_couplings) {
      if (coupling->is_enabled ())
        to_propagate.push_back (coupling);
    }
    for (auto& coupling : to_propagate) {
      coupling->propagate_deactivation ();
    }
  }


  // coupling

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
  Process::remove_activation_coupling (Coupling* c)
  {
    _activation_couplings.erase (
      std::remove (_activation_couplings.begin (), _activation_couplings.end (), c),
      _activation_couplings.end ()
    );
  }

  void
  Process::remove_deactivation_coupling (Coupling* c)
  {
    _deactivation_couplings.erase (
      std::remove (_deactivation_couplings.begin (), _deactivation_couplings.end (), c),
      _deactivation_couplings.end ()
    );
  }

  // tree, component, symtable

  void
  Process::set_parent (Process* p)
  {
    if (p == nullptr) {
      if(_parent)
          parentless_names[this] = _parent->find_child_name (this);
    }
    _parent = p;
  }

  void
  Process::add_child (Process* child, const std::string& name)
  {
    if (child == nullptr)
      return;

    if(child->get_parent ()==nullptr) {
      parentless_names.erase(this);
    }

    child->set_parent (this);
    add_symbol (name, child);
  }

  void
  Process::remove_child (Process* c)
  {
    symtable_t::iterator it;
    for (it = children ().begin (); it != children ().end (); ++it) {
      if (it->second == c) {
        children ().erase (it->first);
        return;
      }
    }
  }

  void
  Process::remove_child (const std::string& name)
  {
    remove_symbol (name);
  }

  Process*
  Process::find_child (const std::string& key)
  {
    //DEBUG
    //cout << "key: " << key << endl;

    if (key.empty ())
      return this;

    /* special case find '*' */ 
    if (key[0] == '*') {
      Process* found = find_child (key.substr(2)); // without "/*""
      if (!found) {
        /* we iterate in depth on each child and stop on the first 'key' found*/
        auto it = children ().begin ();
        while ( it != children ().end ()) {
          found = it->second->find_child (key); // with "/*""
          if (found) return found;
          ++it;
        }
      }
      return found;
    }

    /* special case find from root - using find_child ("//johndoe") */
    //FIXME: improved with c++20 if (key.starts_with("//")
    if (key.length () >= 2 && key[0] == '/' && key[1] == '/') {
        Process* current_cpnt = this;
        Process* current_parent = current_cpnt->get_parent ();
        while (current_parent != nullptr) {
          current_cpnt = current_parent;
          current_parent = current_cpnt->get_parent ();
        }
        //DEBUG
        //cout << "root found: " << current_cpnt->_name << endl;
        return current_cpnt->find_child (key.substr(2));
      }

    size_t found = key.find_first_of ('/');
    if (found != string::npos) {
      string newKey = key.substr (0, found);
      string path = key.substr (found + 1);
      if (newKey[0] == '.' && newKey[1] == '.') {
        if (get_parent ())
          return get_parent ()->find_child (path);
        else
          return nullptr;
      }
      map<string, Process*>::iterator it = find_child_iterator (newKey);
      if (it != children_end ()) {
        return (it->second)->find_child (path);
      }
    }
    if (key[0] == '.' && key[1] == '.')
      return get_parent ();
    map<string, Process*>::iterator it = find_child_iterator (key);
    if (it != children_end ()) {
      return it->second;
    }
    return 0;
  }

  Process*
  Process::find_child (Process *p, const std::string& path)
  {
    if (p == nullptr)
      return URI::find_by_uri (path);
    return p->find_child (path);
  }

  const std::string&
  Process::find_child_name (const Process* symbol) const
  {
    // FIXME : low efficiency function cause by linear search. use with care !

    symtable_t::const_iterator it;

    for (it = children ().begin(); it != children ().end(); ++it)
    {
      if (it->second == symbol)
      {
        //debug
        //cerr << "key found : " << it->first << endl;
        return it->first;
      }
    }

    //string key = "name_not_found";
    //return key;
    return default_name;
  }

  void
  Process::add_symbol (const std::string& name, Process* c)
  {
    /* if ((children ().insert (std::pair<string, Process*> (name, c))).second == false) {
     cerr << "Duplicate name " << name << " in component " << get_name () << endl;
     }*/
    _symtable[name] = c;
  }

  void
  Process::remove_symbol (const std::string& name)
  {
    symtable_t::iterator it = find_child_iterator (name);
    if (it != children_end ())
      children ().erase (it);
    else
      warning (nullptr,   "Warning: symbol " + name + " not found in Process " + name + "\n");
  }

  const std::string&
  Process::get_name () const
  {
    return (_parent ? _parent->find_child_name(this) : parentless_names[this]);
  }


  // data

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
  Process::print_set_vertex_err_msg (Vertex* v)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << "!!!???  _vertex " << _vertex << " /= " << " v " << v << std::endl ;
#endif
  }

  void
  alias_children (Process* p, Process* from)
  {
    Process::symtable_t& symtable = from->children ();
    for (auto& sym : symtable) {
      p->add_symbol (sym.first, sym.second);
    }
  }

  void
  alias (Process *p, const std::string& name, Process* from)
  {
    p->add_symbol (name, from);
  }

  void
  merge_children (Process *p1, const std::string& sy1, Process* p2, const std::string& sy2)
  {
    Process* x2 = p2->find_child (sy2);
    if (x2 == nullptr) {
#ifndef DJNN_NO_DEBUG
      cerr << "trying to merge unknown child " << sy2 << endl;
#endif
      return;
    }
    Process* x1 = p1->find_child (sy1);
    if (x1 == nullptr) {
#ifndef DJNN_NO_DEBUG
      cerr << "trying to merge unknown child " << sy1 << endl;
#endif
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
  add_state_dependency (Process *parent, Process *p)
  {
    if (p && parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), p); 
  }

  void
  remove_state_dependency (Process *parent, Process *p)
  {
    if (p && parent && parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (parent->state_dependency (), p);
  }


#ifndef DJNN_NO_SERIALIZE
  void
  Process::serialize (const std::string& format) {
    cout << "serialize is not yet implemented for '" << get_name () << "'" << endl;
  }
#endif
  
  Process*
  Process::clone () {
#ifndef DJNN_NO_DEBUG
    cout << "clone not implemented for " << get_name () << "\n";
#endif
    return nullptr;
  };

#ifndef DJNN_NO_DEBUG
  static int indent = -1;


  void
  Process::dump (int level)
  {
    cout << (get_parent () ? get_parent ()->find_child_name(this) : get_name ()) << ": ";

    /* check if the component is empty - should be ?*/
    if (children_empty ()) {
      cout << "<EMPTY>" << endl;
      return;
    }

    /* check if the level is reached */
    if ((level != 0) && (indent == level - 1))
      return;

    cout << endl;
    indent++;
    symtable_t::iterator it;
    int i = 0;
    for (it = children ().begin (); it != children ().end (); ++it) {
      for (int j = 0; j < indent; j++)
        cout << "|\t";
      cout << " +" << i++ << " ";
      it->second->dump (level);
      if (it->second->get_process_type() != COMPONENT_T || indent == level - 1)
        cout << endl;
    }
    indent--;
  }
#endif
}
