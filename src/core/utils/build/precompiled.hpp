#include "core/utils/containers.h"
#include "core/ontology/process.h"

namespace djnn {
    class CoreProcess;
    class Coupling;
}

extern template class djnnstl::basic_string<char>;
extern template class djnn::map<djnn::string, djnn::CoreProcess*>;
extern template class djnn::vector<djnn::Coupling*>;

