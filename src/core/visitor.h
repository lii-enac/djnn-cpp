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