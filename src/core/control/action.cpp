#include "action.h"

#include <cassert>

namespace djnn {
	Action::~Action ()
	{
		if(_src) {
			assert (_dst);
			Graph::instance ().remove_edge (_src, _dst);	
		}
	}

    void
    Action::add_native_edge (Process * src, Process * dst)
    {
    	assert (src);
    	assert (dst);
    	assert (_src == nullptr);
    	assert (_dst == nullptr);
    	Graph::instance ().add_edge (src, dst);
    	_src = src;
    	_dst = dst;
    }
}