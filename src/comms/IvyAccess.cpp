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

#include "ivy.h" //FIXME
#include "ivyloop.h" // FIXME
#include "core/syshook/../utils/error.h"

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
  //djnn::get_exclusive_access (DBG_GET);
  IvySendMsg ("%s", _out->get_value ().c_str ());
  //djnn::release_exclusive_access (DBG_REL);
}


  /**** IVY ACCESS ****/

 IvyAccess::IvyAccess (Process *parent, const std::string& name, 
  const std::string& bus, const std::string& appname, const std::string& ready, bool isModel)
 : Process (name, isModel),
 _out ( this, "out", ""),
 _out_a (this, "out_action", &_out),
 _out_c ( &_out , ACTIVATION, &_out_a, ACTIVATION ),
 _arriving ( this,  "arriving", ""),
 _leaving ( this,  "leaving", "")
 {
  _bus =  bus;
  _appname =  appname;
  _ready_message = ready;

  _out_c.disable ();
  Graph::instance().add_edge (&_out, &_out_a);

  /* IN is a special child build in IvyAccess::find_component */

  Process::finalize_construction (parent, name);
}

IvyAccess::~IvyAccess ()
{
  Graph::instance().remove_edge (&_out, &_out_a);
  remove_state_dependency (get_parent (), &_out_a);
}

void
IvyAccess::set_parent (Process* p)
{ 
  /* in case of re-parenting remove edge dependency in graph */
  if (get_parent ()) {
      remove_state_dependency (get_parent (), &_out_a);
  }

  add_state_dependency (p, &_out_a);
    
  Process::set_parent (p); 
}

void IvyAccess::set_arriving(string v) {
  djnn::get_exclusive_access (DBG_GET);
  _arriving.set_value (v, true);
  GRAPH_EXEC;
  djnn::release_exclusive_access (DBG_REL);
}

void IvyAccess::set_leaving(string v) {
  djnn::get_exclusive_access (DBG_GET);
  _leaving.set_value (v, true);
  GRAPH_EXEC;
  djnn::release_exclusive_access (DBG_REL);
}

void
IvyAccess::impl_activate ()
{

  if ( get_activation_state () == ACTIVATED )
    /* should never happen for IvyAccess */
    return;

  /* launche thread */
  start_thread ();
}

void
IvyAccess::impl_deactivate ()
{
  please_stop();

  /* requeste Ivy to stop */
  /* note:
  *  IvyStop has never been implemented : 
  *  so Ivy won't stop if you insert into your main root and delete it 
  */
  warning (nullptr, " IvyAcess::impl_deactivate is not working correctly yet because IvyStop has never been implemented ! \n\t\t solution: do not give parent to your IvyAccess (or nullptr)) and manage it separently from your main root \n\t\t and to not delete it - it will block");
  //IvyStop();
  
}

static void  _before_select (void *data){
  if(ExternalSource::thread_local_cancelled) {
    //std::cerr << this << " " << get_name () << " cancelled" << std::endl;
    return;
  }
  djnn::get_exclusive_access (DBG_GET);
  if(ExternalSource::thread_local_cancelled) {
    //std::cerr << this << " " << get_name () << " cancelled" << std::endl;
    return;
  }
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
    djnn::get_exclusive_access (DBG_GET);
    _out_c.enable();
    djnn::release_exclusive_access (DBG_REL);

    IvyInit (_appname.c_str(), _ready_message.c_str(), _on_ivy_arriving_leaving_agent, this, 0, 0);

      /* get exclusive_access - before select */
    IvySetBeforeSelectHook(_before_select,0);
      /* release exclusive_access - after select */
    IvySetAfterSelectHook(_after_select,0);

    IvyStart(_bus.c_str());
    
    /* note :
    * for now,
    * We can NOT get out of IvyMainLoop:
    * IvyStop has never been implemented
    * 
    */
    while (!get_please_stop ()) {
      /* start Ivy mainloop */
      if(thread_local_cancelled) break;
      IvyMainLoop ();
    }
    
    /* disable coupling */
    djnn::get_exclusive_access (DBG_GET);
    _out_c.disable();
    djnn::release_exclusive_access (DBG_REL);

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

    map<string, Process*>::iterator it = symtable ().find (key.substr (3));
    if (it != symtable ().end ()) {
        /* key exist  - return */
      return it->second;
    }
    else {
        /* key don't exist - create*/

      /* build the substring */
      int nb_subexp, len = 0;
      string full_exp = key.substr (3);
      const char* re_end = _skim_regex (full_exp.c_str(), &nb_subexp);
      
      string regexp = full_exp;
      if(re_end && *re_end != '\0'){
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
    return &_out;

  else if (key.compare ("arriving") == 0)
    return &_arriving;

  else if (key.compare ("leaving") == 0)
    return &_leaving; 

  else
    return 0;
}

}

