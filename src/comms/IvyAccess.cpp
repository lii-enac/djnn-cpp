/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "IvyAccess.h"

#include "ivy.h" //TODO add Ivy/ after confinement
#include "ivyloop.h" //TODO add Ivy/ after confinement
#include "core/utils/error.h"
#include "core/core-dev.h" // graph exec
#include "exec_env/main_loop.h"
#include "utils/debug.h"
#include "core/utils/to_string.h"

#include "core/utils/iostream.h"
#include <unistd.h>


//#define __IVY_DEBUG__

/** regexp function **/

static const char* _skim_regex (const char* p, int* nb)
{
  enum states {NORMAL, QUOTED, SUBEXP, BRACKETED};
  const char* q = p;
  enum states state = NORMAL;
  enum states quoted_state = NORMAL;
  *nb = 0;
  while (*q) {
    switch (state) {
      case NORMAL:
      switch (*q) {
        case '\\':
        quoted_state = NORMAL;
        state = QUOTED;
        break;
        case '/':
        goto done;
        case '(':
        state = SUBEXP;
        (*nb)++;
        break;
        case '[':
        state = BRACKETED;
        break;
      }
      break;
      case QUOTED:
      state = quoted_state;
      break;
      case SUBEXP:
      switch (*q) {
        case '\\':
        quoted_state = SUBEXP;
        state = QUOTED;
        break;
        case ')':
        state = NORMAL;
        break;
      }
      break;
      case BRACKETED:
      switch (*q) {
        case '\\':
        quoted_state = BRACKETED;
        state = QUOTED;
        break;
        case ']':
        state = NORMAL;
        break;
      }
      break;
    }
    ++q;
  }

  done:
  return q;
}

/** IVY DEBUG MAPPING **/

#ifdef __IVY_DEBUG__
static void
_ivy_debug_mapping (map<string, vector<pair<int, djnn::TextProperty*>>> inmap){

  map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
  std::cout << endl << "MAP _in_map:" << endl;
  for (mit = inmap.begin(); mit != inmap.end(); ++mit) {
    std::cout << mit->first << " => (" ;
    /* vector */
    vector<pair<int, djnn::TextProperty*>>::iterator vit;
    for (vit = mit->second.begin (); vit != mit->second.end(); ++vit) {
      /* pair */
      std::cout << "[" << (*vit).first << ", " << (*vit).second << "] " ; 
    }
    std::cout << ")" << endl;
  }
  std::cout << endl;
}
#endif

/** IVY CALLBACK **/

#define STOP_IVY djnn::MainLoop::instance().is_stopping() || access->should_i_stop ()

static void _on_ivy_message ( IvyClientPtr app, void *user_data, int argc, char **argv )
{
  djnn::IvyAccess::msg_callback_user_data * mcud = reinterpret_cast<djnn::IvyAccess::msg_callback_user_data *>(user_data);

  djnn::IvyAccess * access = mcud->access; //reinterpret_cast<djnn::IvyAccess*>(user_data);
  if(STOP_IVY) return;

  djnn::get_exclusive_access (DBG_GET);

  if(STOP_IVY) {
    djnn::release_exclusive_access (DBG_REL);
    return;
  }

  //pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*>* keypair = 
    //(pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*> *) user_data;
  djnn::IvyAccess::regexp_keypair_t * keypair = mcud->keypair; //reinterpret_cast<djnn::IvyAccess::regexp_keypair_t*>(user_data);
  djnn::string regexp = keypair->first;
  //map<string, vector<pair<int, djnn::TextProperty*>>>*
  djnn::IvyAccess::in_map_t*
    in_map =  keypair->second;

#ifdef __IVY_DEBUG__
  std::cout <<  endl <<"_on_ivy_message" << endl;
  std::cout <<  "regexp: '" << regexp << "'" << endl;
  _ivy_debug_mapping (*in_map);
#endif

  //map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
  //::iterator mit;
  auto mit = in_map->find(regexp);

  if (mit != in_map->end ()){
    /* the regexp exist in map */
    //vector<pair<int, djnn::TextProperty*>>::iterator vit;
    for (auto vit = mit->second.begin (); vit != mit->second.end(); ++vit) {
      /* pair */
      djnn::TextProperty* txtprop = (*vit).second;
      if((*vit).first - 1 >= argc) {
        std::cerr << "Ivy err: regexp position is greater or equal to argc " << (*vit).first << " " << argc << std::endl;
        continue;
      }
      djnn::string msg = argv[(*vit).first - 1] ; // index shift is -1 between regexp and argv
      txtprop->set_value(msg, true);
      //GRAPH_EXEC; // methode 1 : per value
    }

  }
  
#ifdef __IVY_DEBUG__
  std::cout << "---------------------" << endl;
  std::cout << "_on_ivy_message - "  << endl;
  std::cout << "argc " << argc  << endl ;
  for (int i=0; i < argc ; i++){
    std::cout << "argv[" << i << "] - " << djnn::string(argv[i]) << endl;
  } 
  std::cout << "user_data (pair->first - regexp) : \"" << regexp  << "\""<< endl;
  std::cout << "---------------------" << endl << endl;
#endif
  GRAPH_EXEC; // methode 2 : per message
  djnn::release_exclusive_access (DBG_REL);
}

static void _on_ivy_arriving_leaving_agent ( IvyClientPtr app, void *user_data, IvyApplicationEvent event )
{
  djnn::IvyAccess* access = reinterpret_cast<djnn::IvyAccess*>(user_data);
  if(STOP_IVY) return;

  if (event == IvyApplicationConnected)
    access->set_arriving (IvyGetApplicationName(app));
  else if (event == IvyApplicationDisconnected)
    access->set_leaving (IvyGetApplicationName(app));
}


namespace djnn
{

  /****  IVY OUT ACTIONS ****/

void
IvyAccess::IvyOutAction::impl_activate () // coupling_activation_hook ()
{
  IvySendMsg ("%s", _out->get_value ().c_str ());
}


  /**** IVY ACCESS ****/

 IvyAccess::IvyAccess (ParentProcess* parent, const string& name, 
  const string& bus, const string& appname, const string& ready, bool isModel)
 : FatProcess (name, isModel),
 ExternalSource(name),
 _out ( this, "out", ""),
 _out_a (this, "out_action", &_out),
 _out_c ( &_out , ACTIVATION, &_out_a, ACTIVATION, true ),
 _arriving ( this,  "arriving", ""),
 _leaving ( this,  "leaving", "")
 {
  _bus =  bus;
  _appname =  appname;
  _ready_message = ready;

  _out_c.disable ();
  
  /* note:
   * c_out in now immediat : 07.2020 - to react each time is activated
   * so we removed the remove_edge (to graph)
   * it works because Ivy can't be stoped
   */
  //graph_add_edge (&_out, &_out_a);
  
  /* IN is a special child build in IvyAccess::find_child */

  finalize_construction (parent, name);
}

IvyAccess::~IvyAccess ()
{

  /* remove edges */

  /* note:
   * c_out in now immediat : 07.2020 - to react each time is activated
   * so we removed the remove_edge (to graph)
   * it works because Ivy can't be stoped
   */
  //graph_remove_edge (&_out, &_out_a);

  remove_state_dependency (get_parent (), &_out_a);


  /* erase _cb_regex_vector */


  //std::cerr << "_cb_regex_vector.size: " << _cb_regex_vector.size () << std::endl;
  while (!_cb_regex_vector.empty ()) {
    auto st = _cb_regex_vector.back ();
    _cb_regex_vector.pop_back ();
    delete st->keypair; // delete the regexp_keypair_t
    delete st; // delete the pointer on struct djnn::IvyAccess::msg_callback_user_data
  }
  //std::cerr << "_cb_regex_vector.size: " << _cb_regex_vector.size () << std::endl;


  /* erase _in_map */


  // _ivy_debug_mapping (_in_map);
  auto it = _in_map.begin ();
  while (it != _in_map.end ()) {
    //note it->second is a vector of pair <index, *textProperty> 
    auto v = it->second;
    while (!v.empty ()){
      // note v.back () is a pair <index, *textProperty>
      auto p = v.back ();
      // remove and delete the Textproperty
      this->remove_child (p.second);
      delete p.second;
      v.pop_back ();
    }
    
    _in_map.erase (it);
    it = _in_map.begin ();
  }
  // _ivy_debug_mapping (_in_map);

  /* cleaning symtable */

  /* note :
   * - 4 children (plain object) should stay in the symtable: out, out_action, arriving, leaving
   * - IvyAccess is not a Container so we have to take care of the symtable
   */
  auto it_s = symtable ().begin ();
  while (symtable ().size () != 4) {
    if ( it_s->first.compare ("out") == 0
      || it_s->first.compare ("out_action") == 0
      || it_s->first.compare ("arriving") == 0
      || it_s->first.compare ("leaving") == 0 ) {
      it_s ++;
    }
    else {
      delete it_s->second;
      this->remove_symbol (it_s->first);
      it_s = symtable ().begin ();
    }
  }

  //DEBUG
  //std::cerr << "remaining children after symtable cleaning :" << children_size () << std::endl ;
  //for (auto s : this->symtable ()) {
  //  std::cerr << s.first << std::endl;
  //}
}

// note : see impl_activate on stoping Ivy
// void
// IvyAccess::set_parent (ParentProcess* parent)
// { 
//   /* in case of re-parenting remove edge dependency in graph */
//   if (get_parent ()) {
//       remove_state_dependency (get_parent (), &_out_a);
//   }

//   add_state_dependency (parent, &_out_a);
    
//   FatProcess::set_parent (parent); 
// }

void IvyAccess::set_arriving(const string& v) {
  djnn::IvyAccess* access = this;
  djnn::get_exclusive_access (DBG_GET);
  if(STOP_IVY) {
    djnn::release_exclusive_access (DBG_REL);
    return;
  }
  _arriving.set_value (v, true);
  GRAPH_EXEC;
  djnn::release_exclusive_access (DBG_REL);
}

void IvyAccess::set_leaving(const string& v) {
  djnn::IvyAccess* access = this;
  djnn::get_exclusive_access (DBG_GET);
  if(STOP_IVY) {
    djnn::release_exclusive_access (DBG_REL);
    return;
  }
  _leaving.set_value (v, true);
  GRAPH_EXEC;
  djnn::release_exclusive_access (DBG_REL);
}

void
IvyAccess::impl_activate ()
{
  //if ( somehow_activating() )
    /* should never happen for IvyAccess */
  //  return;
  //DBG;
  //MainLoop::instance().add_external_source(this); // FIXME TODO
  ExternalSource::start ();
  ///* launch thread */
  //start_thread ();
}

void
IvyAccess::impl_deactivate ()
{
  please_stop();
  //MainLoop::instance().remove_external_source(this); // FIXME TODO

  /* requeste Ivy to stop */
  /* note:
  *  IvyStop has never been implemented : 
  *  so Ivy won't stop if you insert into your main root and delete it 
  */
  warning (nullptr, " IvyAcess::impl_deactivate is not working correctly yet because IvyStop has never been implemented ! \n");
  //IvyStop();
  IvySendMsg ("%s", ""); // should eventually call _after_select and raise 1;
}

static void  _before_select (void *data) {
  IvyAccess * access = reinterpret_cast<IvyAccess*>(data);
  if(STOP_IVY) {
    return;
  }
  djnn::get_exclusive_access (DBG_GET);
  if(access->should_i_stop ()) return;
  GRAPH_EXEC;
  djnn::release_exclusive_access (DBG_REL);
}

static void  _after_select (void *data) {
  IvyAccess * access = reinterpret_cast<IvyAccess*>(data);
  if(STOP_IVY) {
    //throw 1; thread leak, to be continued...
  }
}

void
IvyAccess::run ()
{
  set_please_stop (false);
  try {
    IvyInit (_appname.c_str(), _ready_message.c_str(), _on_ivy_arriving_leaving_agent, this, 0, 0);

    /* get exclusive_access - before select */
    IvySetBeforeSelectHook(_before_select, this);
    /* release exclusive_access - after select */
    IvySetAfterSelectHook(_after_select, this);

    IvyStart(_bus.c_str());

    /* enable coupling */
    djnn::get_exclusive_access (DBG_GET);
    _out_c.enable();
    djnn::release_exclusive_access (DBG_REL);
    
    /* note :
    * for now,
    * We can NOT get out of IvyMainLoop:
    * IvyStop has never been implemented...
    * 
    */
    try {
      /* start Ivy mainloop */
      IvyMainLoop ();
    }
    catch (int) {
      DBG;
    }

    // ... hence we will never reach this point...
    
    /* disable coupling */
    djnn::get_exclusive_access (DBG_GET);
    _out_c.disable();
    djnn::release_exclusive_access (DBG_REL);

  } catch (std::exception& e) {
    warning (nullptr, e.what());
  }
}

FatChildProcess*
IvyAccess::find_child_impl (const string& key)
{
  
#ifdef __IVY_DEBUG__
  std::cout << "---- Ivy find_child key : " << key << "------" <<  endl ;
#endif

  if (key.at(0) == 'i' && key.at(1) == 'n' && key.at(2) == '/'){

    symtable_t::iterator it = find_child_iterator (key.substr (3));
    if (it != children_end ()) {
        /* key exist  - return */
      return it->second;
    }
    
    /* key do not exist - maybe use internal string as regexp */

    /* 
       get sub_string = ...... : in/....../1
       and save index : 1
    */
    int nb_subexp, len = 0;
    string full_exp = key.substr (3);
    const char* re_end = _skim_regex (full_exp.c_str(), &nb_subexp);
      
    string regexp = full_exp;
    if(re_end && *re_end != '\0'){
      len = key.find (re_end, 3);
       regexp = key.substr (3, len-3);
    }
    int index = std::stoi (re_end+1);

#ifdef __IVY_DEBUG__
    std::cout << "regexp : \"" << regexp << "\" - full : \"" << full_exp << "\"" << endl;
#endif

    TextProperty* tmp = dynamic_cast<TextProperty*>(FatProcess::find_child_impl (regexp));
    if (tmp){

      string new_regexp_to_found = "in/" + tmp->get_value () + "/" + djnn::to_string(index);

#ifdef __IVY_DEBUG__
      std::cout << "REPLACE : " << regexp << " -> " << tmp->get_value () << endl;
      std::cout << "new_regexp:  " << new_regexp_to_found << endl << endl;
#endif

      /* key do not exist - but use internal string as regexp --- return*/
      return find_child_impl (new_regexp_to_found);
    }
    else {
      /* key don't exist at all - create it */

      /* add as a new _in child */
      /* and keep track of "/number" */
      TextProperty* newin = new TextProperty ( this, full_exp, "");

      /* if it is the first binding on this regexp we had  the callback else nothing */
      map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
      mit = _in_map.find(regexp);
      if (mit == _in_map.end ()) {
        /* the only way for now is to save in a pair <regexp, in_map*>* to keep track on cb */
        auto * regexp_keypair = new regexp_keypair_t (regexp, &_in_map);
        msg_callback_user_data * mcud = new djnn::IvyAccess::msg_callback_user_data{this, regexp_keypair};
        _cb_regex_vector.push_back (mcud);
        IvyBindMsg(_on_ivy_message, mcud, "%s", regexp.c_str() );
      }

      /* register in _in_map */  
      _in_map[regexp].push_back (djnnstl::make_pair(index, newin));
      
     
#ifdef __IVY_DEBUG__
       _ivy_debug_mapping (_in_map);
      std::cout << "nb sub : " << nb_subexp <<  " endl : \"" <<  re_end << "\" len : " << len << " index : " << index << endl ;
      std::cout << " regexp : \"" << regexp << "\" - full : \"" << full_exp << "\"" << endl << endl;
#endif

      /* key don't exist at all - return */
      return newin;
    }
  }

  else if (key.compare ("out") == 0)
    return &_out;

  else if (key.compare ("arriving") == 0)
    return &_arriving;

  else if (key.compare ("leaving") == 0)
    return &_leaving; 

  else
    return 0;
}

}

