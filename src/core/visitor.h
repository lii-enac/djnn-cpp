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
namespace djnn {
class CoreProcess;
class IntProperty;
class Binding;
class Connector; // in base :-/ => put all visitor stuff in its own module, not in core?

class Visitor {
  public:
    virtual void visit (CoreProcess*) {}
    virtual void visit (IntProperty*) {}
    virtual void visit (Binding*) {}
    virtual void visit (Connector*) {}
};
} // namespace djnn