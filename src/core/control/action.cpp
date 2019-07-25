#include "action.h"

#include <cassert>

namespace djnn {
    // [FIXME] this should go in a specific native_expression_action...
	Action::~Action ()
	{
		if(_src) {
            for (auto dst: _dsts ) {
			     Graph::instance ().remove_edge (_src, dst);
             }
		}
	}

    void
    Action::add_native_edge (Process * src, Process * dst)
    {
    	assert (src);
    	assert (dst);
        // there may be multiple output to a native expression, but with a single _src
        if(_src) assert (src==_src);
        else _src = src;
    	Graph::instance ().add_edge (src, dst);
        _dsts.push_back(dst);
    }
}