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
#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/uri.h"
#include "core/utils/error.h"
#include "core/utils/utils-dev.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/to_string.h"

#include <algorithm>
#include <cassert>

#include <iostream>

// #if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
// #include <boost/core/demangle.hpp>
// //#include <typeinfo>
// #endif

// #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
// #include <boost/core/demangle.hpp>
// //#include <boost/type_index.hpp>
// #endif

namespace djnn
{
  

  list<pair<CoreProcess*, long int>> __dbg_creation_stat_order; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  vector<string> __dbg_destruction_stat_order; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  long int __dbg_creation_num = 0; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)

#ifdef DJNN_NO_DEBUG
  CoreProcess::DebugInfo   CoreProcess::_dbg_info{"no dbg info",0};
#endif
                    string CoreProcess::default_name = "noname"; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  CoreProcess::couplings_t CoreProcess::default_couplings; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  CoreProcess::symtable_t  CoreProcess::default_symtable; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)
  vector<string> CoreProcess::default_properties_name;  // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)

  static int parentless_names_order = 0 ;
#if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER 
  map<const CoreProcess*, std::pair<string, long long>> parentless_names; 
#else
  static map<const CoreProcess*, std::pair<string, int>> parentless_names;
#endif

  CoreProcess::CoreProcess (bool model)
  : 
  _vertex (nullptr)
  , _bitset (0)
  #ifndef DJNN_NO_DEBUG
  , _debug_parent (nullptr)
  , _debug_name (default_name)
  #endif
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
    __dbg_creation_stat_order.push_back (std::make_pair(this, __dbg_creation_num++));
    __position_in_creation = std::prev(__dbg_creation_stat_order.end ());
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
      warning (nullptr, " CoreProcess::~CoreProcess - " +  (pp ? get_hierarchy_name (pp): "")  + " - _vertex is NOT NULL and it should\n");
      // get_activation_couplings is virtual: in a destructor only CoreProcess::get_activation_couplings will be called and it should return an empty container
      //for (auto &c: get_activation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (activation)" << __FL__;
      //for (auto &c: get_deactivation_couplings()) std::cerr << get_hierarchy_name (c->get_dst()) << " is still coupled (deactivation)" << __FL__;
      #endif
      _vertex->invalidate ();
    }

    remove_from_parentless_name (this);

    #if _DEBUG_SEE_CREATION_DESTRUCTION_ORDER
    string data_save = "DELETE [" + djnn::to_string (__position_in_creation->second) + "] - " + cpp_demangle(typeid(*this).name()) + \
      " - " + (this->get_debug_parent () ? this->get_debug_parent ()->get_name () : "") + "/" + this->get_debug_name ();

    __dbg_destruction_stat_order.push_back (data_save);
    __dbg_creation_stat_order.erase (__position_in_creation);
    #endif
  }

  void
  delete_parentless_processes ()
  {
    // //debug print before
    // std::cerr << "\033[1;35m" << std::endl;
    // std::cerr << "-- Before - Parentless_name map - "<< parentless_names.size() << " -- " << std::endl;
    // if (!parentless_names.empty()) {
    //   std::cerr << "Warning - parentless_names is not EMPTY !!" << std::endl;
    //   for (const auto& [key, pair_value] : parentless_names)
    //     std::cerr << '[' << key << "] = " << pair_value.second  << " - \"" << pair_value.first << "\"\n";
    // }
    // else
    //   std::cerr << "OK - Parentless_Name is EMPTY";
    // std::cerr << "\033[0m \n\n";

    // sort by building order
    std::map <int, const Process*> parentless_names_ordered;
    for (auto& [key, pair_value] : parentless_names)
      parentless_names_ordered[pair_value.second] = key;

    // delete by reverse order
    //C++20 : for (const auto& [key, value] : parentless_names_ordered | std::views::reverse)
    for (auto iter = parentless_names_ordered.rbegin(); iter != parentless_names_ordered.rend(); ++iter) {

      //debug: display parentless_names_ordered -- in reverse
      //std::cerr << '[' << iter->first << "] = " << iter->second  << " try to delete" << std::endl;
      //TODO 11.2022 : hack to not remove updateDrawing but it should NOT be here - it should be deleted in updateDrawing::clear ()
      if (iter->first) {
        delete iter->second;
        iter->second = nullptr;
      }
      
    }
    
    // print if Parentless_names is not empty
  #ifndef DJNN_NO_DEBUG
    if (!parentless_names.empty()) {
      std::cerr << "\033[1;35m" << std::endl;
      //std::cerr << "-- After - Parentless_name map - "<< parentless_names.size() << " -- " << std::endl;
      std::cerr << "Warning - parentless_names is not EMPTY !!" << std::endl;
      for (const auto& [key, pair_value] : parentless_names)
        std::cerr << "[" << key << "] = " << pair_value.second  << " - \"" << pair_value.first << "\"\n";
      std::cerr << "\033[0m \n\n";
    }
  #endif
  }

  const string&
  CoreProcess::get_name (ParentProcess* parent) const
  {
    return parent->find_child_name(this);
  }

  const vector<string>&
  CoreProcess::get_properties_name () const
  {
    return default_properties_name;
  }

  #ifndef DJNN_NO_DEBUG
  FatProcess*
  CoreProcess::get_debug_parent ()
  {
    return dynamic_cast<FatProcess*>(_debug_parent);
  }
  #endif

  FatChildProcess*
  CoreProcess::find_child (const string& path)
  {
    auto * found = find_child_impl (path);
#ifndef DJNN_NO_DEBUG
    if (!found) {
      if (Context::instance ()->line ()>0)
        warning (nullptr, Context::instance ()->filename () + ":" + __to_string (Context::instance ()->line ()) + ": symbol '" + path + "' in process '" + get_debug_name () + "' not found");
      else
        warning (this, "symbol '" + path + "' not found");
    }
#endif
    return found;
  }

  FatChildProcess*
  CoreProcess::find_optional_child (const string& path)
  {
    return find_child_impl (path);
  }
  
  FatChildProcess*
  CoreProcess::find_child (int index)
  {
    auto * found = find_child_impl (index);
#ifndef DJNN_NO_DEBUG
    if (!found) {
      if (Context::instance ()->line ()>0)
        warning(nullptr, Context::instance ()->filename () + ":" + __to_string (Context::instance ()->line ()) + ": index " + __to_string(index) + " in process '" + get_debug_name () + "' not found");
      else
        warning (this, "index '" + __to_string(index) + "' not found");
    }
#endif
    return found;
  }


  FatChildProcess*
  CoreProcess::find_child_impl (const string& _)
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
      for (auto &c: CouplingProcess::get_activation_couplings()) loginfo(get_hierarchy_name (c->get_dst()) + " is still coupled (activation)");
      for (auto &c: CouplingProcess::get_deactivation_couplings()) loginfo(get_hierarchy_name (c->get_dst()) + " is still coupled (deactivation)");
      #endif
    }
  }

  FatProcess::FatProcess (const string& name, bool model)
  : ChildProcess(model),
  _data (nullptr)
  {
    #ifndef DJNN_NO_DEBUG
    if (name != "") { set_debug_name (name); }
    #endif
  }


  // finalize_construction
  void
  CoreProcess::finalize_construction (ParentProcess* parent, const string& name, CoreProcess* state_dep)
  {
    if (parent) {
      parent->add_child (this, name);
    } else {
      parentless_names[this] = std::pair<string, int> {name, parentless_names_order++} ;
    }

    #ifndef DJNN_NO_DEBUG
    set_debug_parent(parent);
    if (name != "") { set_debug_name (name); }
    #endif
  }

  void
  ChildProcess::finalize_construction (ParentProcess* parent, const string& name, CoreProcess* state_dep)
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
  FatProcess::finalize_construction (ParentProcess* parent, const string& name, CoreProcess* state_dep) /* called by SubFatProcess to link to parent */
  {
    ChildProcess::finalize_construction (parent, name, state_dep);
  }


  // main activation API

  void
  CoreProcess::activate ()
  {
    if (pre_activate ()) {
      impl_activate ();
      post_activate ();
    }

  }

  void
  CoreProcess::deactivate ()
  {
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
  CoreProcess::schedule_activation ()
  {
    Graph::instance().schedule_activation (this);
  }

  void
  CoreProcess::schedule_delete ()
  {
    Graph::instance().schedule_delete (this);
  }

  inline
  void
  notify (const Process::couplings_t& couplings, void (Coupling::*propagate_activation_fun) ())
  {
    // optimizations for 0 and 1 coupling before protecting from auto disabling
    if (couplings.empty ()) return;
    if (couplings.size () == 1) { // no risk of disabling a coupling
      auto& coupling = *(couplings.begin());
      if (coupling->is_enabled ()) (*coupling.*propagate_activation_fun) ();
      return;
    }

    /* WARNING: disputable choice.
     * The immediate propagation could disable some sibling couplings.
     * We make the choice to register all the couplings associated with a source before propagating
     * the activation. Thus the disabling of a coupling will be effective only on the next run.
     * */

    // save enabled statuses
    size_t index = 0; // allocate before Variable Length Array
    bool enabled[couplings.size ()]; // allocate on stack
    for (auto& coupling : couplings) {
      enabled[index++] = coupling->is_enabled ();
    }
    // propagate
    index = 0;
    for (auto& coupling : couplings) {
      if (enabled[index++]) (*coupling.*propagate_activation_fun) ();
    }
  }

  void
  CoreProcess::notify_activation ()
  {
    auto const & couplings = get_activation_couplings ();
    auto const & propagate_fun = &Coupling::propagate_activation;
    notify (couplings, propagate_fun);
  }

  void
  CoreProcess::notify_deactivation ()
  {
    auto const & couplings = get_deactivation_couplings ();
    auto const & propagate_fun = &Coupling::propagate_deactivation;
    notify (couplings, propagate_fun);
  }

  void
  CoreProcess::set_activation_flag (activation_flag_e VALUE)
  {
    // only if flags are different
    if (static_cast<activation_flag_e>(get_bitset (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_SHIFT)) != VALUE) {
  #if !_EXEC_FULL_ORDERED_VERTICES
      // if the process has vertex and has something to do // could be simplify ?
      if (_vertex && (VALUE != NONE_ACTIVATION)) {
        Graph::instance ().add_in_activation (_vertex);
      }
  #endif
      set_bitset (ACTIVATION_FLAG_MASK, ACTIVATION_FLAG_SHIFT, static_cast<unsigned int>(VALUE));
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
  #ifdef DJNN_LOCAL_DEBUG 
      std::cerr << "!!!???  _vertex " << _vertex << " /= " << " v " << v << std::endl;
  #endif
    }
    //print_set_vertex_err_msg (v);
    _vertex = v; 
  }
  #endif


  // tree, component, symtable

  void
  ChildProcess::set_parent (ParentProcess* parent)
  {
    /* note: 
    *  do not try to remove parentless process from parentless_name if only set_parent has been called
    *  remove from parentless_name only through add_symbol else the parent will not recognise his children.
    */
    _parent = djnn_dynamic_cast<FatProcess*>(parent);
    #if !DJNN_NO_DEBUG
    set_debug_parent (parent);
    #endif
  }

  void
  remove_from_parentless_name (CoreProcess* child) {

    if(child && child->get_parent ()==nullptr) {
      auto it = parentless_names.find(child);
      if (it != parentless_names.end())
        parentless_names.erase (it);
    }
  }

  void
  FatProcess::add_child (FatChildProcess* child, const string& name)
  {
    if (child == nullptr) { return; }

    // see note from ChildProcess::set_parent
    //remove_from_parentless_name (child); // is now directly made in add_symbol

    child->set_parent (this);
    add_symbol (name, child);
  }

  void
  FatProcess::remove_child (FatChildProcess* c)
  {
    for (auto & name_proc_pair: symtable ()) {
      if (name_proc_pair.second == c) {
        symtable ().erase (name_proc_pair.first);
        return;
      }
    }
  }

  void
  FatProcess::remove_child (const string& name)
  {
    remove_symbol (name);
  }

  FatChildProcess*
  FatProcess::find_child_impl (const string& key)
  {
    //DEBUG
    //std::cout << "key: " << key << endl;

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
        //std::cout << "root found: " << current_cpnt->_name << endl;
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
  FatProcess::find_child_impl (FatChildProcess *p, const string& path)
  {
    if (p == nullptr) { return URI::find_by_uri (path); }
    return p->find_child_impl (path);
  }

  const string&
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
  FatProcess::notify_change ( unsigned int notify_mask_ ) // pseudo, graph-less coupling for efficiency reasons in gui
  {
    auto * p = get_parent();
    if(p) {
      p->notify_change(notify_mask_);
    }
  }

  CoreProcess::symtable_t::iterator FatProcess::find_child_iterator (const string& name) { return _symtable.find (name); }
  CoreProcess::symtable_t::iterator FatProcess::children_end () { return _symtable.end (); }
  bool FatProcess::children_empty () { return _symtable.empty (); }
  size_t FatProcess::children_size () const { return _symtable.size(); }

  void
  FatProcess::add_symbol (const string& name, FatChildProcess* c)
  {
    /* 
      if ((symtable ().insert (pair<string, FatProcess*> (name, c))).second == false) {
        cerr << "Duplicate name " << name << " in component " << get_name () << endl;
      }
    */

    // see note from ChildProcess::set_parent
    // remove c from parentless_names if necessarry 
    // if c is here, now, he's should have a parent
    remove_from_parentless_name (c);

    _symtable[name] = c;
  }

  void
  FatProcess::remove_symbol (const string& name)
  {
    auto it = find_child_iterator (name);
    if (it != children_end ())
      symtable ().erase (it);
    else
      warning (nullptr, "Warning: symbol '" + name + "' not found in FatProcess '" + name + "'\n");
  }

  const string&
  FatProcess::get_name () const
  {
    return (_parent ? _parent->find_child_name(this) : parentless_names[this].first);
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
  alias (ParentProcess* parent, const string& name, FatChildProcess* from)
  {
    parent->add_symbol (name, from);
  }

  void
  merge_children (ParentProcess *p1, const string& sy1, ParentProcess* p2, const string& sy2)
  {
    //debug
    //std::cout << "process::merge_children " << p1->get_debug_name () << " - " << sy1 << " - "  << p2->get_debug_name () << " - " << sy2 << std::endl ;

    auto * x1 = p1->find_child_impl (sy1);
    if (x1 == nullptr) {
      error (p1, "trying to merge unknown child " + sy1);
      return;
    }
    auto * x2 = p2->find_child_impl (sy2);
    if (x2 == nullptr) {
      error(p2, "trying to merge unknown child " + sy2);
      return;
    }

    for (auto& c : x2->get_activation_couplings ()) {
      // we have to change the src of the coupling 
      // the set_src will remove from and add
      c->set_src (x1);
    }
    for (auto& c : x2->get_deactivation_couplings ()) {
      // we have to change the src of the coupling 
      // the set_src will remove from and add
      c->set_src (x1);
    }
    p2->remove_child (sy2);
    p2->add_symbol (sy2, x1);
    delete x2;
  }

  void
  add_state_dependency (ParentProcess* parent, CoreProcess *p)
  {
    if (p && parent && parent->state_dependency () != nullptr)
      graph_add_edge (parent->state_dependency (), p); 
  }

  void
  remove_state_dependency (ParentProcess* parent, CoreProcess *p)
  {
    if (p && parent && parent->state_dependency () != nullptr)
      graph_remove_edge (parent->state_dependency (), p);
  }


  #ifndef DJNN_NO_SERIALIZE
  void
  CoreProcess::serialize (const string& format) {
    #ifndef DJNN_NO_DEBUG
    auto * pp = this;
    warning (this, "serialize is not yet implemented for " + cpp_demangle(typeid(*this).name()) + " '" + (pp? pp->get_debug_name ():"") + "'");
    #endif
  }
  #endif
  
  CoreProcess*
  CoreProcess::clone ()
  {
    map<CoreProcess*, CoreProcess*> origs_clones;
    return impl_clone (origs_clones);
  };

  CoreProcess*
  CoreProcess::impl_clone (std::map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    #ifndef DJNN_NO_DEBUG
    auto * pp = this;
    warning (this, "clone is not yet implemented for " + cpp_demangle(typeid(*this).name()) + " '" + (pp? pp->get_debug_name ():"") + "'");
    #endif
  
    return nullptr;
  }

  static int indent = -1; // NOLINT (cppcoreguidelines-avoid-non-const-global-variables)

  void
  CoreProcess::dump (int level)
  {
    loginfonofl ("CoreProcess " + __to_string(this));
  }

  void
  FatProcess::dump (int level)
  {
    loginfonocr ((get_parent () ? get_parent ()->find_child_name(this) : get_name ()) + ": ");

    /* check if the component is empty - should be ?*/
    if (children_empty ()) {
      loginfonofl ("<EMPTY>");
      return;
    }

    /* check if the level is reached */
    if ((level != 0) && (indent == level - 1)) { return; }

    loginfonofl("");
    indent++;

    int i = 0;
    for (symtable_t::iterator it = symtable ().begin (); it != symtable ().end (); ++it) {
      for (int j = 0; j < indent; j++)
        loginfonocr ("|\t");
      loginfonocr (" +" + __to_string(i++) + " ");
      it->second->dump (level);
      if (it->second->get_process_type() != CONTAINER_T || indent == level - 1)
        loginfonofl("");
    }
    indent--;
  }
}
