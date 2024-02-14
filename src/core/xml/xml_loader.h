#include "core/visitor.h"
#include "core/utils/containers.h" // impl should be opaque :-/
#include "core/utils/containers/string.h" // impl should be opaque :-/

namespace djnn {
    class CoreProcess;
    class IntProperty;
    class Binding;

    class XMLLoader : public Visitor {
        public:
            void load(const djnnstl::string& path);
            virtual void visit(CoreProcess*) {}
            virtual void visit(IntProperty*);
            virtual void visit(Binding*);
            virtual void visit(Connector*);
        
        protected:
            djnnstl::vector<CoreProcess*> _current_parent_stack;
    };
}