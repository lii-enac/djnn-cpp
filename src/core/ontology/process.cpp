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
#include "core/utils/djnn_dynamic_cast.h"

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
  std::list<std::pair<CoreProcess*, long int>> __dbg_creation_stat_order; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  std::vector<string> __dbg_destruction_stat_order; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  long int __dbg_creation_num = 0; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)

#ifdef DJNN_NO_DEBUG
  CoreProcess::DebugInfo   CoreProcess::_dbg_info{"no dbg info",0};
#endif
                    string CoreProcess::default_name = "noname"; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  CoreProcess::couplings_t CoreProcess::default_couplings; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  CoreProcess::symtable_t  CoreProcess::default_symtable; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)

  static map<const ChildProcess*, string> parentless_names; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)


  CoreProcess::CoreProcess (bool model)
  : 
  #ifndef DJNN_NO_DEBUG
  _debug_parent (nullptr)
  , _debug_name (FatProcess::default_name)
  ,
  #endif
  _vertex (nullptr),
  _bitset (0)
  {
    set_is_model (model);
    CoreProcess::set_activation_flag (NONE_ACTIVATION);
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
      auto * pp = this;
      warning ( nullptr, " CoreProcess::~CoreProcess - " +  (pp ? get_hierarchy_name (pp): "")  + " - _vertex is NOT NULL and it should\n");
      // get_activation_couplings is virtual: in a destructor only CoreProcess::get_activation_couplings will be called and it should return an empty container
      //for (auto &c: get_activation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (activation)" << __FL__;
      //for (auto &c: get_deactivation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (deactivation)" << __FL__;
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

  const std::string&
  CoreProcess::get_name (ParentProcess* parent) const
  {
    return parent->find_child_name(this);
  }

  #ifndef DJNN_NO_DEBUG
  FatProcess*
  CoreProcess::get_debug_parent ()
  {
    return dynamic_cast<FatProcess*>(_debug_parent);
  }
  #endif

  FatChildProcess*
  CoreProcess::find_child (const std::string& path)
  {
    auto * found = find_child_impl (path);
#ifndef DJNN_NO_DEBUG
    if (!found) {
      if (Context::instance ()->line ()>0)
        warning (nullptr, Context::instance ()->filename () + ":" + std::to_string (Context::instance ()->line ()) + ": symbol '" + path + "' in process '" + get_debug_name () + "' not found");
      else
        warning (this, "symbol '" + path + "' not found");
    }
#endif
    return found;
  }
  
  FatChildProcess*
  CoreProcess::find_child (int index)
  {
    auto * found = find_child_impl (index);
#ifndef DJNN_NO_DEBUG
    if (!found) {
      if (Context::instance ()->line ()>0)
        warning(nullptr, Context::instance ()->filename () + ":" + std::to_string (Context::instance ()->line ()) + ": index " + std::to_string(index) + " in process '" + get_debug_name () + "' not found");
      else
        warning (this, "index '" + std::to_string(index) + "' not found");
    }
#endif
    return found;
  }


  FatChildProcess*
  CoreProcess::find_child_impl (const std::string& _)
  {
    return nullptr;
  }
  
  FatChildProcess*
  CoreProcess::find_child_impl (int /*index*/)
  {
    return nullptr;
  }

  CouplingProcess::~CouplingProcess ()
  {
    for (auto * c : CouplingProcess::get_activation_couplings ()) {
      c->about_to_delete_src ();
    }
    for (auto * c : CouplingProcess::get_deactivation_couplings ()) {
      c->about_to_delete_src ();
    }
    if (vertex () != nullptr) {
      #ifndef DJNN_NO_DEBUG
      auto * pp = this;
      warning ( nullptr, " CouplingProcess::~CouplingProcess - " +  (pp ? get_hierarchy_name (pp): "")  + "\n"); //" - _vertex is NOT NULL and it should\n");
      for (auto &c: CouplingProcess::get_activation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (activation)" << __FL__;
      for (auto &c: CouplingProcess::get_deactivation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (deactivation)" << __FL__;
      #endif
    }
  }

  FatProcess::FatProcess (const std::string& name, bool model)
  : ChildProcess(model),
  _data (nullptr)
  {
    #ifndef DJNN_NO_DEBUG
    if (name != "") { set_debug_name (name); }
    #endif
  }


  // finalize_construction
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
    if (name != "") { set_debug_name (name); }
    #endif
  }

  void
  ChildProcess::finalize_construction (ParentProcess* parent, const std::string& name, CoreProcess* state_dep)
  {
    CoreProcess::finalize_construction (parent, name, state_dep);
    if (parent) {
      // by default state_dep is nullptr so _state_dependency depends on parent->state_dependenncy)
      if (state_dep == nullptr) {
        _state_dependency = parent->state_dependency ();
      } else {
        _state_dependency = state_dep;
        add_state_dependency (parent, _state_dependency);
      }
    }
  }

  void
  FatProcess::finalize_construction (ParentProcess* parent, const std::string& name, CoreProcess* state_dep) /* called by SubFatProcess to link to parent */
  {
    ChildProcess::finalize_construction (parent, name, state_dep);
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

    if (get_activation_state () != DEACTIVATED) { return false; }
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
    if (get_parent () != nullptr && !get_parent ()->somehow_activating()) {
       return false;
    }
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
    if (get_activation_state() != ACTIVATED) { return false; }
    set_activation_state (DEACTIVATING);
    return true;
  }

  void
  CoreProcess::post_deactivate ()
  {
    notify_deactivation ();
    set_activation_state (DEACTIVATED);
    //set_activation_flag (NONE_ACTIVATION); // handled in Graph::exec ()
  }

  void
  CoreProcess::notify_activation ()
  {
    /* WARNING: disputable choice.
     * The immediate propagation could disable some couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the activation. Thus the disabling of a coupling will be effective only on the next run.
     * */
    std::vector<Coupling*> to_propagate; // NOLINT (cppcoreguidelines-init-variables)
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
    std::vector<Coupling*> to_propagate; // NOLINT (cppcoreguidelines-init-variables)
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
    if (_vertex && v && _vertex != v) {
      std::cerr << "!!!???  _vertex " << _vertex << " /= " << " v " << v << std::endl;
    }
    //print_set_vertex_err_msg (v);
    _vertex = v; 
  }
  #endif


  // tree, component, symtable

  void
  ChildProcess::set_parent (ParentProcess* parent)
  {
    if (parent == nullptr) {
      if (_parent) {
          parentless_names[this] = _parent->find_child_name (this);
      }
    }
    _parent = djnn_dynamic_cast<FatProcess*>(parent);
    #if !DJNN_NO_DEBUG
    set_debug_parent (parent);
    #endif
  }

  void
  FatProcess::add_child (FatChildProcess* child, const std::string& name)
  {
    if (child == nullptr) { return; }

    if(child->get_parent ()==nullptr) {
      parentless_names.erase(this);
    }

    child->set_parent (this);
    add_symbol (name, child);
  }

  void
  FatProcess::remove_child (FatChildProcess* c)
  {
    //symtable_t::iterator it;
    //for (it = symtable ().begin (); it != symtable ().end (); ++it) {
    for (auto & name_proc_pair: symtable ()) {
      if (name_proc_pair.second == c) {
        symtable ().erase (name_proc_pair.first);
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
  FatProcess::find_child_impl (const std::string& key)
  {
    //DEBUG
    //cout << "key: " << key << endl;

    if (key.empty ()) {
      return this;
    }

    /* special case find '*' */ 
    if (key[0] == '*') {
      auto * found = find_child_impl (key.substr(2)); // without "/*""
      if (!found) {
        /* we iterate in depth on each child and stop on the first 'key' found*/
        auto it = symtable ().begin ();
        while ( it != symtable ().end ()) {
          found = it->second->find_child_impl (key); // with "/*""
          //if (found) return found;
          if (found) break;
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
        return current_cpnt->find_child_impl (key.substr(2));
      }

    size_t found = key.find_first_of ('/');
    if (found != string::npos) {
      string newKey = key.substr (0, found);
      string path = key.substr (found + 1);
      if (newKey[0] == '.' && newKey[1] == '.') {
        if (get_parent ()) {
          return get_parent ()->find_child_impl (path);
        } else {
          return nullptr;
        }
      }
      symtable_t::iterator it = find_child_iterator (newKey);
      if (it != children_end ()) {
        return (it->second)->find_child_impl (path);
      }
    }
    if (key[0] == '.' && key[1] == '.')
      return get_parent ();

    symtable_t::iterator it = find_child_iterator (key);
    if (it != children_end ()) {
      return it->second;
    }

    return nullptr;
  }

  FatChildProcess*
  FatProcess::find_child_impl (FatChildProcess *p, const std::string& path)
  {
    if (p == nullptr) { return URI::find_by_uri (path); }
    return p->find_child_impl (path);
  }

  const std::string&
  FatProcess::find_child_name (const CoreProcess* symbol) const
  {
    // FIXME : low efficiency function cause by linear search. use with care !
    for (auto & name_proc_pair: symtable ()) {
      if (name_proc_pair.second == symbol) {
        //cerr << "key found : " << it->first << endl;
        return name_proc_pair.first;
      }
    }
  
    return default_name;
  }

  void
  FatProcess::add_symbol (const std::string& name, FatChildProcess* c)
  {
    /* if ((symtable ().insert (std::pair<string, FatProcess*> (name, c))).second == false) {
     cerr << "Duplicate name " << name << " in component " << get_name () << endl;
     }*/
    _symtable[name] = c;
  }

  void
  FatProcess::remove_symbol (const std::string& name)
  {
    auto it = find_child_iterator (name);
    if (it != children_end ())
      symtable ().erase (it);
    else
      warning (nullptr, "Warning: symbol '" + name + "' not found in FatProcess '" + name + "'\n");
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
    FatProcess::symtable_t& symtable = from->symtable ();
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
    auto * x2 = p2->find_child_impl (sy2);
    if (x2 == nullptr) {
      #ifndef DJNN_NO_DEBUG
      cerr << "trying to merge unknown child " << sy2 << endl;
      #endif
      return;
    }
    auto * x1 = p1->find_child_impl (sy1);
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
    delete x2; // hum, are we really sure about this? // maybe used in for merging gradient
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
    #ifndef DJNN_NO_DEBUG
    auto * pp = this;
    cout << "serialize is not yet implemented for " << boost::core::demangle(typeid(*this).name()) << " '" << (pp? pp->get_debug_name ():"") << "'" << endl;
    #endif
  }
  #endif
  
  CoreProcess*
  CoreProcess::clone () {
    #ifndef DJNN_NO_DEBUG
    auto * pp = this;
    cout << "clone is not yet implemented for " << boost::core::demangle(typeid(*this).name()) << " '" << (pp? pp->get_debug_name ():"") << "'" << endl;
    #endif
  
    return nullptr;
  };

#ifndef DJNN_NO_DEBUG
  static int indent = -1; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)

  void
  CoreProcess::dump (int level)
  {
    cout << "CoreProcess " << this << endl;
  }

  void
  FatProcess::dump (int level)
  {
    cout << (get_parent () ? get_parent ()->find_child_name(this) : get_name ()) << ": ";

    /* check if the component is empty - should be ?*/
    if (children_empty ()) {
      cout << "<EMPTY>" << endl;
      return;
    }

    /* check if the level is reached */
    if ((level != 0) && (indent == level - 1)) { return; }

    cout << endl;
    indent++;

    int i = 0;
    for (symtable_t::iterator it = symtable ().begin (); it != symtable ().end (); ++it) {
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
