#include "core/xml/xml_loader.h"

#include "core/core.h"

namespace djnn {

void
XMLLoader::load (const djnnstl::string& path)
{
    // TODO
    // open xml stream with expat

    _current_parent_stack.clear ();
    _current_parent_stack.push_back (nullptr);
    // then for each node type:
    // if (type==int) {
    //   auto* p = new IntProperty(_current_parent_stack.top(), "");
    //   visit(p);
    // } else
    // if (type==Component) {
    //   autoy* p = new Component(_current_parent_stack.top(), "");
    //   visit(p);
    //   _current_parent_stack.push_back(p);
    // } else ...
}

void
XMLLoader::visit (IntProperty* p)
{
}

void
XMLLoader::visit (Binding* p)
{
}

void
XMLLoader::visit (Connector* p)
{
}

} // namespace djnn
