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
#include <boost/core/demangle.hpp>
#include <typeinfo>
#endif

#if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
#include <boost/core/demangle.hpp>
#include <boost/type_index.hpp>
#endif

namespace djnn
{
  using namespace std;

  #if _DEBUG_SEE_ACTIVATION_SEQUENCE
  std::vector<__stat_exec> __activation_order;
  #endif
  std::list<std::pair<CoreProcess*, long int>> __creation_stat_order;
  std::vector<string> __destruction_stat_order;
  long int __creation_num = 0;


  long int FatProcess::_nb_anonymous = 0;

#ifdef DJNN_NO_DEBUG
  CoreProcess::DebugInfo CoreProcess::_dbg_info{"no dbg info",0};
#endif

                    string CoreProcess::default_name = "noname";
  CoreProcess::couplings_t CoreProcess::default_couplings;
  CoreProcess::symtable_t  CoreProcess::default_symtable;

  static map<const ChildProcess*, string> parentless_names;


  CoreProcess::CoreProcess (bool model)
  : 
  #ifndef DJNN_NO_DEBUG
  _debug_parent (nullptr)
  , _debug_name (FatProcess::default_name)
  ,
  #endif
  _vertex (nullptr)
  {
    set_is_model (model);
    set_activation_flag (NONE_ACTIVATION);
    set_activation_state (DEACTIVATED);
  
#ifndef DJNN_NO_DEBUG
    if (Context::instance ()->line () != -1) {
      _dbg_info = {Context::instance ()->filename (), Context::instance ()->line ()};
    } else {
      _dbg_info = {"no dbg info",0};
    }
#endif

    #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
    __creation_stat_order.push_back (std::make_pair(this, __creation_num++));
    __position_in_creation = std::prev(__creation_stat_order.end ());
    #endif
  }

  const std::string&
  CoreProcess::get_name (ParentProcess* parent) const
  {
    return parent->find_child_name(this);
  }

#ifndef DJNN_NO_DEBUG
  FatProcess* CoreProcess::get_debug_parent ()
  {
    return dynamic_cast<FatProcess*>(_debug_parent);
  }
#endif

  FatProcess::FatProcess (const std::string& name, bool model)
  : ChildProcess(model),
  _data (nullptr)
  {
    #ifndef DJNN_NO_DEBUG
    if (name != "") set_debug_name (name);
    #endif
  }

  void
  CoreProcess::finalize_construction (ParentProcess* parent, const std::string& name, CoreProcess* state_dep)
  {
    if (parent) {
      parent->add_child ((ChildProcess*)this, name); // FIXME
    } else {
      parentless_names[(ChildProcess*)this] = name; // FIXME
    }

    #ifndef DJNN_NO_DEBUG
    set_debug_parent(parent);
    if (name != "") set_debug_name (name);
    #endif
  }

  void
  ChildProcess::finalize_construction (ParentProcess* parent, const std::string& name, CoreProcess* state_dep)
  {
    CoreProcess::finalize_construction (parent, name, state_dep);
    if (parent) {
      // by default state_dep is nullptr so _state_dependency depends on parent->state_dependenncy)
      if (state_dep == nullptr)
        _state_dependency = parent->state_dependency ();
      else
        _state_dependency = state_dep;
    }
  }

  void
  FatProcess::finalize_construction (ParentProcess* parent, const std::string& name, CoreProcess* state_dep) /* called by SubFatProcess to link to parent */
  {
    ChildProcess::finalize_construction (parent, name, state_dep);
    /*if (parent) {
      // by default state_dep is nullptr so _state_dependency depends on parent->state_dependenncy)
      if (state_dep == nullptr)
        _state_dependency = parent->state_dependency ();
      else
        _state_dependency = state_dep;

      parent->add_child (this, name);
    } else {
      parentless_names[this] = name;
      if (state_dep != nullptr)
        _state_dependency = state_dep;
    }*/
  }

  CoreProcess::~CoreProcess ()
  {
    /* note: 
       this code is to prevent bugs 
       this should NEVER happen
       _vertex should be nullptr at this place
       if not, something IS NOT deleted correctly
    */
    if (_vertex != nullptr) {
#ifndef DJNN_NO_DEBUG
       auto * pp = dynamic_cast<FatProcess*>(this);
       warning ( nullptr, " FatProcess::~FatProcess - " +  (pp ? get_hierarchy_name (pp): "")  + " - _vertex is NOT NULL and it should\n");
       for (auto &c: get_activation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (activation)" << __FL__;
       for (auto &c: get_deactivation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (deactivation)" << __FL__;
#endif
       _vertex->invalidate ();
    }

#if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
    string data_save = "DELETE [" + to_string (__position_in_creation->second) + "] - " + boost::core::demangle(typeid(*this).name()) + \
       " - " + (this->get_debug_parent () ? this->get_debug_parent ()->get_name () : "") + "/" + this->get_debug_name ();

    __destruction_stat_order.push_back (data_save);
    __creation_stat_order.erase (__position_in_creation);
#endif
  }

  CouplingProcess::~CouplingProcess ()
  {
    for (auto * c : get_activation_couplings ()) {
      c->about_to_delete_src ();
    }
    for (auto * c : get_deactivation_couplings ()) {
      c->about_to_delete_src ();
    }
  }

  FatProcess::~FatProcess ()
  {
    /* make sure everything is wiped out the symtable */
    children ().clear ();
  }


  // main activation API

  void
  CoreProcess::activate ()
  {
    #if _DEBUG_SEE_ACTIVATION_SEQUENCE
    __nb_activation.first++;
    __activation_order.push_back(std::make_pair(true, this));
    #endif

    if (pre_activate ()) {
      impl_activate ();
      post_activate ();
    }

  }

  void
  CoreProcess::deactivate ()
  {
    #if _DEBUG_SEE_ACTIVATION_SEQUENCE
    __nb_activation.second++;
    __activation_order.push_back(std::make_pair(false, this));
    #endif

    if (pre_deactivate ()) {
      impl_deactivate ();
      post_deactivate ();
    }
  }

  bool
  CoreProcess::pre_activate ()
  {
    /* no activation if :
     * 1 - already activated
     * 2 - is activating
     * 3 - the parent exists and is stopped
     */
    if (get_activation_state () != DEACTIVATED)
      return false;
    set_activation_state (ACTIVATING);
    return true;
  }

  bool
  FatProcess::pre_activate ()
  {
    /* no activation if :
     * 1 - already activated
     * 2 - is activating
     * 3 - the parent exists and is stopped
     */
    if (get_parent () != nullptr && !get_parent ()->somehow_activating())
       return false;
    return CoreProcess::pre_activate ();
  }

  void
  CoreProcess::post_activate ()
  {
    notify_activation ();
    set_activation_state (ACTIVATED);
  }

  void 
  CoreProcess::post_activate_auto_deactivate ()
  {
    //TODO: inline function in process.h
    CoreProcess::post_activate ();
    deactivate ();
  }

  bool
  CoreProcess::pre_deactivate ()
  {
    if (get_activation_state() != ACTIVATED)
      return false;
    set_activation_state (DEACTIVATING);
    return true;
  }

  void
  CoreProcess::post_deactivate ()
  {
    notify_deactivation ();
    set_activation_state (DEACTIVATED);
    set_activation_flag (NONE_ACTIVATION); // already done in graph
  }

  void
  CoreProcess::notify_activation ()
  {
    /* WARNING: disputable choice.
     * The immediate propagation could disable some couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the activation. Thus the disabling of a coupling will be effective only on the next run.
     * */
    std::vector<Coupling*> to_propagate;
    for (auto& coupling : get_activation_couplings ()) {
      if (coupling->is_enabled ())
        to_propagate.push_back (coupling);
    }
    for (auto& coupling : to_propagate) {
      coupling->propagate_activation ();
    }
  }

  void
  CoreProcess::schedule_activation ()
  {
    Graph::instance().schedule_activation (this);
  }

  void
  CoreProcess::schedule_delete ()
  {
    Graph::instance().schedule_delete (this);
  }

  void
  CoreProcess::notify_deactivation ()
  {
    /* WARNING: disputable choice.
     * The immediate propagation could disable some couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the deactivation. Thus the disabling of a coupling will be effective only on the next run.
     * */
    std::vector<Coupling*> to_propagate;
    for (auto& coupling : get_deactivation_couplings ()) {
      if (coupling->is_enabled ())
        to_propagate.push_back (coupling);
    }
    for (auto& coupling : to_propagate) {
      coupling->propagate_deactivation ();
    }
  }


  // coupling

  void
  CouplingProcess::add_activation_coupling (Coupling* c)
  {
    _activation_couplings.push_back (c);
  }

  void
  CouplingProcess::add_deactivation_coupling (Coupling* c)
  {
    _deactivation_couplings.push_back (c);
  }

  void
  CouplingProcess::remove_activation_coupling (Coupling* c)
  {
    _activation_couplings.erase (
      std::remove (_activation_couplings.begin (), _activation_couplings.end (), c),
      _activation_couplings.end ()
    );
  }

  void
  CouplingProcess::remove_deactivation_coupling (Coupling* c)
  {
    _deactivation_couplings.erase (
      std::remove (_deactivation_couplings.begin (), _deactivation_couplings.end (), c),
      _deactivation_couplings.end ()
    );
  }

#ifndef DJNN_NO_DEBUG
  void
  CoreProcess::set_vertex (Vertex *v) 
  { 
    if (_vertex && v && _vertex != v)
      std::cerr << "!!!???  _vertex " << _vertex << " /= " << " v " << v << std::endl ;
    //print_set_vertex_err_msg (v);
    _vertex = v; 
  }
  /*void
  FatProcess::print_set_vertex_err_msg (Vertex* v)
  {
    
  }*/
#endif


  // tree, component, symtable

  void
  ChildProcess::set_parent (ParentProcess* parent)
  {
    if (parent == nullptr) {
      if(_parent)
          parentless_names[this] = _parent->find_child_name (this);
    }
    _parent = dynamic_cast<FatProcess*>(parent);
    #if !DJNN_NO_DEBUG
    set_debug_parent (parent);
    #endif
  }

  void
  FatProcess::add_child (FatChildProcess* child, const std::string& name)
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
  FatProcess::remove_child (FatChildProcess* c)
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
  FatProcess::remove_child (const std::string& name)
  {
    remove_symbol (name);
  }

  FatChildProcess*
  FatProcess::find_child (const std::string& key)
  {
    //DEBUG
    //cout << "key: " << key << endl;

    if (key.empty ())
      return this;

    /* special case find '*' */ 
    if (key[0] == '*') {
      auto * found = find_child (key.substr(2)); // without "/*""
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
        FatProcess* current_cpnt = this;
        FatProcess* current_parent = current_cpnt->get_parent ();
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
      symtable_t::iterator it = find_child_iterator (newKey);
      if (it != children_end ()) {
        return (it->second)->find_child (path);
      }
    }
    if (key[0] == '.' && key[1] == '.')
      return get_parent ();
    symtable_t::iterator it = find_child_iterator (key);
    if (it != children_end ()) {
      return it->second;
    }
    return 0;
  }

  FatChildProcess*
  FatProcess::find_child (FatChildProcess *p, const std::string& path)
  {
    if (p == nullptr)
      return URI::find_by_uri (path);
    return p->find_child (path);
  }

  const std::string&
  FatProcess::find_child_name (const CoreProcess* symbol) const
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
  FatProcess::add_symbol (const std::string& name, FatChildProcess* c)
  {
    /* if ((children ().insert (std::pair<string, FatProcess*> (name, c))).second == false) {
     cerr << "Duplicate name " << name << " in component " << get_name () << endl;
     }*/
    _symtable[name] = c;
  }

  void
  FatProcess::remove_symbol (const std::string& name)
  {
    symtable_t::iterator it = find_child_iterator (name);
    if (it != children_end ())
      children ().erase (it);
    else
      warning (nullptr,   "Warning: symbol " + name + " not found in FatProcess " + name + "\n");
  }

  const std::string&
  FatProcess::get_name () const
  {
    return (_parent ? _parent->find_child_name(this) : parentless_names[this]);
  }


  // data

  void
  FatProcess::set_data (CoreProcess* data)
  {
    _data = data;
  }

  CoreProcess*
  FatProcess::get_data ()
  {
    return _data;
  }

  void
  alias_children (ParentProcess* parent, FatProcess* from)
  {
    FatProcess::symtable_t& symtable = from->children ();
    for (auto& sym : symtable) {
      parent->add_symbol (sym.first, sym.second);
    }
  }

  void
  alias (ParentProcess* parent, const std::string& name, FatChildProcess* from)
  {
    parent->add_symbol (name, from);
  }

  void
  merge_children (ParentProcess *p1, const std::string& sy1, ParentProcess* p2, const std::string& sy2)
  {
    auto * x2 = p2->find_child (sy2);
    if (x2 == nullptr) {
#ifndef DJNN_NO_DEBUG
      cerr << "trying to merge unknown child " << sy2 << endl;
#endif
      return;
    }
    auto * x1 = p1->find_child (sy1);
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
  add_state_dependency (ParentProcess* parent, CoreProcess *p)
  {
    if (p && parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), p); 
  }

  void
  remove_state_dependency (ParentProcess* parent, CoreProcess *p)
  {
    if (p && parent && parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (parent->state_dependency (), p);
  }


#ifndef DJNN_NO_SERIALIZE
  void
  CoreProcess::serialize (const std::string& format) {
    auto * pp = dynamic_cast<FatProcess*>(this);
    cout << "serialize is not yet implemented for " << boost::core::demangle(typeid(*this).name()) << " '" << (pp? pp->get_name ():"") << "'" << endl;
  }
#endif
  
  CoreProcess*
  CoreProcess::clone () {
#ifndef DJNN_NO_DEBUG
    auto * pp = dynamic_cast<FatProcess*>(this);
    cout << "clone is not yet implemented for " << boost::core::demangle(typeid(*this).name()) << " '" << (pp? pp->get_name ():"") << "'" << endl;

#endif
    return nullptr;
  };

#ifndef DJNN_NO_DEBUG
  static int indent = -1;

  void
  CoreProcess::dump (int level)
  {
      cout << "CoreProcess " << this << endl;
  }

  void
  FatProcess::dump (int level)
  {
    //FatProcess *pp = dynamic_cast<FatProcess*>(this);
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
      if (it->second->get_process_type() != CONTAINER_T || indent == level - 1)
        cout << endl;
    }
    indent--;
  }
#endif
}
