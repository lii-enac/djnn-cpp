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
template class djnn::map<djnn::string, djnn::CoreProcess*>;
template class djnn::vector<djnn::Coupling*>;

