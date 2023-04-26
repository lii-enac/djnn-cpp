#include "core/utils/containers.h"
#include "core/utils/algorithm.h"
#include "core/ontology/process.h"

namespace djnn {
    class CoreProcess;
    class Coupling;
}

#if DJNN_STL_STD
template class std::basic_string<char>;
#endif
template class djnnstl::map<djnnstl::string, djnn::CoreProcess*>;
template class djnnstl::vector<djnn::Coupling*>;

