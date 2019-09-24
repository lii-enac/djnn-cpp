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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "IvyAccess.h"

#include "Ivy/ivy.h"
#include "Ivy/ivyloop.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

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
  cout << endl << "MAP _in_map:" << endl;
  for (mit = inmap.begin(); mit != inmap.end(); ++mit) {
    cout << mit->first << " => (" ;
    /* vector */
    vector<pair<int, djnn::TextProperty*>>::iterator vit;
    for (vit = mit->second.begin (); vit != mit->second.end(); ++vit) {
      /* pair */
      cout << "[" << (*vit).first << ", " << (*vit).second << "] " ; 
    }
    cout << ")" << endl;
  }
  cout << endl;
}
#endif

/** IVY CALLBACK **/

static void _on_ivy_message ( IvyClientPtr app, void *user_data, int argc, char **argv )
{
  djnn::get_exclusive_access (DBG_GET);

  pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*>* keypair = (pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*>*) user_data;
  string regexp = keypair->first;
  map<string, vector<pair<int, djnn::TextProperty*>>>* in_map =  keypair->second;

#ifdef __IVY_DEBUG__
  cout <<  endl <<"_on_ivy_message" << endl;
  cout <<  "regexp: '" << regexp << "'" << endl;
  _ivy_debug_mapping (*in_map);
#endif

  map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
  mit = in_map->find(regexp);

  if (mit != in_map->end ()){
    /* the regexp exist in map */
    vector<pair<int, djnn::TextProperty*>>::iterator vit;
    for (vit = mit->second.begin (); vit != mit->second.end(); ++vit) {
      /* pair */
      djnn::TextProperty* txtprop = (*vit).second;
      string msg =  argv[(*vit).first - 1] ; // index shift is -1 between regexp and argv
      txtprop->set_value(msg, true);
    }

  }
  
#ifdef __IVY_DEBUG__
  cout << "---------------------" << endl;
  cout << "_on_ivy_message - "  << endl;
  cout << "argc " << argc  << endl ;
  for (int i=0; i < argc ; i++){
    cout << "argv[" << i << "] - " << string(argv[i]) << endl;
  } 
  cout << "user_data (pair->first - regexp) : \"" << regexp  << "\""<< endl;
  cout << "---------------------" << endl << endl;
#endif
 
  //GRAPH_EXEC;

  djnn::release_exclusive_access (DBG_REL);
}

static void _on_ivy_arriving_leaving_agent ( IvyClientPtr app, void *user_data, IvyApplicationEvent event )
{
  djnn::IvyAccess* ivy = (djnn::IvyAccess*) user_data;

  if (event == IvyApplicationConnected)
    ivy->set_arriving (IvyGetApplicationName(app));
  else if (event == IvyApplicationDisconnected)
    ivy->set_leaving (IvyGetApplicationName(app));
}


namespace djnn
{

  /****  IVY OUT ACTIONS ****/

 void
  IvyAccess::IvyOutAction::coupling_activation_hook ()
  {  
     IvySendMsg ("%s", _out->get_value ().c_str ());
  }


  /**** IVY ACCESS ****/

 IvyAccess::IvyAccess (Process *p, const std::string& n, 
  const std::string& bus, const std::string& appname, const std::string& ready, bool isModel) :
 Process (p, n, isModel)
 {
  _bus =  bus;
  _appname =  appname;
  _ready_message = ready;

    /* OUT child */
  _out = new TextProperty ( this, "out", "");
  _out_a = new IvyOutAction (this, "out_action", _out);
  _out_c = new Coupling (_out , ACTIVATION, _out_a, ACTIVATION );
  _out_c->disable();
  Graph::instance().add_edge(_out, _out_a);
  if (_parent && _parent->state_dependency () != nullptr)
    Graph::instance ().add_edge (_parent->state_dependency (), _out_a);

    /* ARRIVING child */
  _arriving = new TextProperty ( this,  "arriving", "");

    /* LEAVING child */
  _leaving = new TextProperty ( this,  "leaving", "");

    /* IN is a special child build in IvyAccess::find_component */

  Process::finalize_construction ();
}

IvyAccess::~IvyAccess ()
{
  Graph::instance().remove_edge(_out, _out_a);
  if (_parent && _parent->state_dependency () != nullptr)
   Graph::instance ().remove_edge (_parent->state_dependency (), _out_a);

 delete _out;
 delete _out_c;
 delete _out_a;

 delete _arriving;
 delete _leaving;

 // TODO: Clean MAP
 //while (!_in.empty()) {
 // delete _in.back();
 // _in.pop_back();
 //}
}

void IvyAccess::set_arriving(string v) {
  _arriving->set_value (v, true);

  djnn::get_exclusive_access (DBG_GET);
  GRAPH_EXEC;
  djnn::release_exclusive_access (DBG_REL);
}

void IvyAccess::set_leaving(string v) {
  _leaving->set_value (v, true);

  djnn::get_exclusive_access (DBG_GET);
  GRAPH_EXEC;
  djnn::release_exclusive_access (DBG_REL);
}

void
IvyAccess::activate ()
{
  please_stop ();

  /* launche thread */
  start_thread ();
}

void
IvyAccess::deactivate ()
{
  please_stop();

  /* requeste Ivy to stop */
  IvyStop();
}

static void  _before_select (void *data){
  djnn::get_exclusive_access (DBG_GET);
  GRAPH_EXEC;    
  djnn::release_exclusive_access (DBG_REL);
}

static void  _after_select (void *data){
  //djnn::get_exclusive_access (DBG_REL);
}

void
IvyAccess::run ()
{
  set_please_stop (false);
  try {

      /* enable coupling */
    //djnn::get_exclusive_access (DBG_GET);
    _out_c->enable();
    //djnn::release_exclusive_access (DBG_REL);

    IvyInit (_appname.c_str(), _ready_message.c_str(), _on_ivy_arriving_leaving_agent, this, 0, 0);

      /* get exclusive_access - before select */
    IvySetBeforeSelectHook(_before_select,0);
      /* release exclusive_access - after select */
    IvySetAfterSelectHook(_after_select,0);

    IvyStart(_bus.c_str());
    while (!get_please_stop ()) {
      /* start Ivy mainloop */
      IvyMainLoop ();
    }
    /* disable coupling */
    //djnn::get_exclusive_access (DBG_GET);
    _out_c->disable();
    //djnn::release_exclusive_access (DBG_REL); 

  } catch (exception& e) {
    warning (nullptr, e.what());
  }
}

Process*
IvyAccess::find_component (const string& key)
{
  
#ifdef __IVY_DEBUG__
  cout << "Ivy find_component key : " << key << endl ;
#endif

  if (key.at(0) == 'i' && key.at(1) == 'n' && key.at(2) == '/'){

    map<string, Process*>::iterator it = _symtable.find (key.substr (3));
    if (it != _symtable.end ()) {
        /* key exist  - return */
      return it->second;
    }
    else {
        /* key don't exist - create*/

      /* build the substring */
      int nb_subexp, len = 0;
      const char* re_end = _skim_regex (key.substr (3).c_str(), &nb_subexp);
      string full_exp = key.substr (3);
      string regexp = full_exp;
      if(*re_end != '\0'){
        len = key.find (re_end, 3);
        regexp = key.substr (3, len-3);
      }
      int index = stoi (re_end+1);

      /* add as a new _in child */
      /* and keep track of "/number" */
      TextProperty* newin = new TextProperty ( this, full_exp, "");

      /* if it is the first binding on this regexp we had  the callback else nothing */
      map<string, vector<pair<int, djnn::TextProperty*>>>::iterator mit;
      mit = _in_map.find(regexp);
      if (mit == _in_map.end ()) {
        /* the only way for now is to save in a pair <regexp, in_map*>* to keep track on cb */
        pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*>* regexp_keypair = new pair<string, map<string, vector<pair<int, djnn::TextProperty*>>>*> (regexp, &_in_map);
        IvyBindMsg(_on_ivy_message, regexp_keypair, "%s", regexp.c_str() );
      }

      /* register in _in_map */  
      _in_map[regexp].push_back (make_pair(index, newin));
      
     
#ifdef __IVY_DEBUG__
       _ivy_debug_mapping (_in_map);
      cout << "nb sub : " << nb_subexp <<  " endl : \"" <<  re_end << "\" len : " << len << " index : " << index << endl ;
      cout << " regexp : \"" << regexp << "\" - full : \"" << full_exp << "\"" << endl << endl;
#endif

    
      return newin;
    }
  }

  else if (key.compare ("out") == 0)
    return _out;

  else if (key.compare ("arriving") == 0)
    return _arriving;

  else if (key.compare ("leaving") == 0)
    return _leaving; 

  else
    return 0;
}

}

