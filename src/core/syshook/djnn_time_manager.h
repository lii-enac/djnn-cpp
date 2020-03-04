/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "time_manager.h"
#include "core/syshook/external_source.h"

#include <mutex>

namespace djnn {

    class DjnnTimeManager : public djnn_internal::Time::Manager, public djnn::ExternalSource {
    public:
        DjnnTimeManager();

        static DjnnTimeManager& instance ();
        static DjnnTimeManager _instance;

        virtual void please_stop () override;

        void update_ref_now_in_scheduled_timers ();

        void run_for_emscripten ();

    protected:
        // djnn_internal::Time::Manager
        virtual void firstTimerHasChanged() override;
    private:
        // ExternalSource
        void run () override;
        std::timed_mutex cancel_mutex;
    };
}
