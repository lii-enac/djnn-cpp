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
 *
 */

#include "core/utils/error.h"
#include "serializer.h"

#if !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif

namespace djnn
{

  using namespace std;

#if !defined(DJNN_NO_SERIALIZE)

  /* init static variable */
  CoreProcess* AbstractSerializer::serializationRoot = nullptr;
  AbstractSerializer* AbstractSerializer::serializer = nullptr;

  static string __cur_format;
  
  void
  AbstractSerializer::pre_serialize (CoreProcess* root, const std::string& format) {
     if (AbstractSerializer::serializationRoot == 0) {

        AbstractSerializer::serializationRoot = root;
        __cur_format = format;
        
        if (format.compare("XML") == 0) {
          AbstractSerializer::serializer = new XMLSerializer();
          cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n";
        }
        else if (format.compare ("JSON") == 0){
          AbstractSerializer::serializer = new JSONSerializer();
          cout << "{\n";
        }
        else
          warning (nullptr, format + " is not a valid serializer format (XML|JSON) " );
     }
  }

  void
  AbstractSerializer::post_serialize (CoreProcess* root) {
    if (AbstractSerializer::serializationRoot == root) {
      AbstractSerializer::serializationRoot = nullptr;
      AbstractSerializer::serializer = nullptr;

      if (__cur_format.compare("JSON") == 0) 
      cout << "}\n";
    }
  }


  /*
 * recursive helper function that computes the simplest path from an origin
 * to a target while staying within the tree defined by a root node.
 */

typedef struct __path_context {
    Process *f;
    CoreProcess *t;
    struct __path_context* prev;
} __path_context;

static void
path_compute (Process* from, CoreProcess* to,
       __path_context* pc,  __path_context* ec, string& buf)
{
#ifdef DEBUG
  cerr << __FUNCTION__ << " - FROM " << (from ? from->get_name () : "<anonymous>")
    << " - TO " << (to ? to->get_name () : "<anonymous>") << endl;
#endif
  
  /*
   * design principles: we recursively go up the parent chains of from
   * and to, going up until we reach root or the end of the parent chain
   * of to, creating a linked list of the values at each recursion, and
   * keeping track of where the first of the two reaches root. When the
   * recursion stops, we use the linked list to create the path.
   */
  __path_context c = { from, to, pc };
  int insert_slash = 1;

  /*
   * 1. we stop the recursion when:
   *  - the two chains have reached the root;
   *  - or we have found that the origin is not connected to the root, and
   *    we have reached the top of the parent chain of the target;
   *  - or we find that the target is not connected to the root.
   */
  if (to == AbstractSerializer::serializationRoot ? (from == AbstractSerializer::serializationRoot || from == 0) : to == 0) {
    /* 1.1 in the first case... */
    if (from && to) {
      /* ... we start where each chain has reached root */
      __path_context *fc = pc->f == AbstractSerializer::serializationRoot ? ec : &c;
      __path_context *tc = pc->t == AbstractSerializer::serializationRoot ? ec : &c;
      /* then go to where these chains diverge, */
      while (fc && (fc->f == tc->t)) {
        fc = fc->prev;
        tc = tc->prev;
      }
      /* continue on the 'from' chain to add the right number of '..', */
      insert_slash = 0;
      while (fc && fc->f) {
        buf += "..";
        if (fc->prev->f)
          buf += "/";
        insert_slash = 1;
        fc = fc->prev;
      }
      /* and position ourselves at the divergence point. */
      pc = tc;
    }

    /* 1.2 in all cases, we create the remaining path to the target. */
    while (pc && pc->t) {
      if (insert_slash)
        buf += "/";
      else
        insert_slash = 1;
      auto * pt = dynamic_cast<Process*>(pc->t);
      std::string name;
      if(pt)
        name = pt->get_name();
      buf +=  !(name.empty ()) ? name : "<anonymous>";
      pc = pc->prev;
    }
    return;
  }

  /* 2. if only one of the two chains has just reached root, memorize it. */
  if (!ec && (to == AbstractSerializer::serializationRoot || from == AbstractSerializer::serializationRoot))
    ec = &c;

  /* 3. recurse up to root or the end of chains. */
  path_compute ((from == AbstractSerializer::serializationRoot || !from) ? from : from->get_parent (),
          (to == AbstractSerializer::serializationRoot || !to) ? to : to->get_parent (),
          &c, ec, buf);
}


  void
  AbstractSerializer::compute_path (Process* from, CoreProcess* to, string& buf)
  {
    __path_context c = { 0, 0, 0 };

    if (from == to) {
      buf += '.';
      return;
    }

    path_compute (from, to, &c, 0, buf);
  }
#endif

}
