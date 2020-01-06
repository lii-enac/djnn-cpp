#include "time_manager.h"
#include "core/syshook/external_source.h"

#include <mutex>

namespace djnn {

class DjnnTimeManager : public djnn_internal::Time::Manager, public djnn::ExternalSource {
    public:
        DjnnTimeManager(); // : _acc_duration(0) {}

        static DjnnTimeManager& instance ();
        //static std::once_flag onceFlag;
        static DjnnTimeManager _instance;

        void activate () { start_thread (); }
      
        // djnn_internal::Time::Manager
        virtual void firstTimerHasChanged() override;

        void run_for_emscripten ();
        int _acc_duration;
    private:
        // ExternalSource
        void run () override;
        std::timed_mutex cancel_mutex;
      };
}
