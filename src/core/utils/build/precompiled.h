#include <vector>
#include <map>
#include <string>
#include <list>

#include "core/ontology/process.h"

namespace djnn {
    class CoreProcess;
    class Coupling;
}


extern template class std::basic_string<char>;
extern template class std::map<std::string, djnn::CoreProcess*>;
extern template class std::vector<djnn::Coupling*>;

