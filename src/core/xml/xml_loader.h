/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2023-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#include "core/utils/containers.h"        // impl should be opaque :-/
#include "core/utils/containers/string.h" // impl should be opaque :-/
#include "core/visitor.h"

namespace djnn {
class CoreProcess;
class IntProperty;
class Binding;

class XMLLoader : public Visitor {
  public:
    void         load (const djnnstl::string& path);
    virtual void visit (CoreProcess*) {}
    virtual void visit (IntProperty*);
    virtual void visit (Binding*);
    virtual void visit (Connector*);

  protected:
    djnnstl::vector<CoreProcess*> _current_parent_stack;
};
} // namespace djnn