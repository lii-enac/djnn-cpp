/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "iofd.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/error.h"
#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"

#ifdef _WIN32
// TODO: Windows hack - do better
#define NOGDI
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#include <winsock2.h>
#include <ws2tcpip.h>
#else

#include <sys/select.h>

#endif
// dbg
// #include "core/utils/iostream.h"
// #include "utils/debug.h"
// #include "utils/utils-dev.h"
// #include "utils/ext/debugbreak.h"
// #include "core/utils/to_string.h"

namespace djnn {

IOFD::IOFD (CoreProcess* parent, const CoreProcess::string& name, int readfd)
    : FatProcess (name),
      ExternalSource (name),
      _readfd (readfd),
      _readable (this, "readable")
{
    // MainLoop::instance().add_external_source(this);
    finalize_construction (parent, name);
    //"IOFD"+djnnstl::to_string(readfd)
}

IOFD::~IOFD ()
{
    // MainLoop::instance().remove_external_source(this);
}

void
IOFD::impl_activate ()
{
    start_thread ();
}

void
IOFD::impl_deactivate ()
{
    please_stop ();
}

void
IOFD::run ()
{
    set_please_stop (false);
    // try {
    while (!should_i_stop ()) {
        fd_set reads;
        FD_ZERO (&reads);
        FD_SET (_readfd, &reads);
        int ret = select (_readfd + 1, &reads, nullptr, nullptr, nullptr); // blocking call
        // int ret = read(_readfd);
        if (ret == -1) {
            warning (nullptr, "error reading fd");
            return;
        }
        if (should_i_stop ()) {
            return;
        }
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if (!should_i_stop ()) {
            _readable.activate (); // propagating
            GRAPH_EXEC;            // executing
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    }
    // } catch (exception& e) {
    //   warning (nullptr, e.what());
    // }
}
} // namespace djnn
