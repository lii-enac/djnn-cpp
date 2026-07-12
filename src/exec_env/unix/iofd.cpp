/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */
#include "core/utils/remotery.h"

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
#include <poll.h>

#endif
// dbg
#include "core/utils/iostream.h"
#include "utils/debug.h"
#include "utils/utils-dev.h"
#include "core/utils/utils-dev.h"
// #include "utils/ext/debugbreak.h"
// #include "core/utils/to_string.h"

namespace djnn {

IOFD::IOFD (CoreProcess* parent, const CoreProcess::string& name, int readfd)
    : FatProcess (name),
      ExternalSource (name),
      _readfd (readfd),
      _readable (this, "readable"),
      _writable (this, "writable"),
      _except (this, "except")
{
    // MainLoop::instance().add_external_source(this);
    finalize_construction (parent, name);
    //"IOFD"+djnnstl::to_string(readfd)
    printf("IOFD %d %s\n", readfd, get_hierarchy_name(this).c_str());
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
        //std::cerr << get_hierarchy_name(this) << __FL__;

        fd_set reads, writes, excepts;

        FD_ZERO (&reads);
        FD_ZERO (&writes);
        FD_ZERO (&excepts);

        //if (_readable.has_coupling()) {
            //printf("read set %d %s\n", _readfd, get_hierarchy_name(this).c_str());
            FD_SET (_readfd, &reads);
        //}
        // if (_writable.has_coupling()) {
        //     printf("write set %d %s\n", _readfd, get_hierarchy_name(this).c_str());
        //     FD_SET (_readfd, &writes);
        // }
        // if (_except.has_coupling()) {
        //     printf("except set %d %s\n", _readfd, get_hierarchy_name(this).c_str());
        //     FD_SET (_readfd, &excepts);
        // }

        int ret = select (_readfd + 1, &reads, &writes, &excepts, nullptr); // blocking call
        //int ret = select (_readfd + 1, &reads, nullptr, nullptr, nullptr); // blocking call
        
        // pollfd reads[1];
        // reads[0].fd = _readfd;
        // reads[0].events = 0;

        // if (_readable.has_coupling()) {
        //     printf("read set %d %s\n", _readfd, get_hierarchy_name(this).c_str());
        //     reads[0].events |= POLLIN;
        // }
        // if (_writable.has_coupling()) {
        //     //printf("write set %d\n", _readfd);
        //     reads[0].events |= POLLOUT;
        // }
        // if (_except.has_coupling()) {
        //     //printf("except set %d\n", _readfd);
        //     reads[0].events |= POLLPRI;
        // }
        
        // int ret = poll(reads, 1, -1);
        
        // int ret = read(_readfd);
        if (ret == -1) {
            perror("error select(2)ing/poll(2)ing fd");
            warning (nullptr, "error select(2)ing/poll(2)ing fd");
            return;
        }
        if (should_i_stop ()) {
            return;
        }
        //DBG;
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if (!should_i_stop ()) {
            rmt_BeginCPUSample(external_source_iofd, 0);
            if (FD_ISSET(_readfd, &reads)) {
            //if (reads[0].revents & POLLIN) {
                //printf("read returns %d %s\n", _readfd, get_hierarchy_name(this).c_str());
                _readable.activate (); // propagating
            }
            //if (FD_ISSET(_readfd, &writes)) {
            // if (reads[0].revents & POLLIN) {
            //     //printf("write returns %d %s\n", _readfd, get_hierarchy_name(this).c_str());
            //     _writable.activate (); // propagating
            // }
            // //if (FD_ISSET(_readfd, &excepts)) {
            // if (reads[0].revents & POLLPRI) {
            //     printf("except returns %d %s \n", _readfd, get_hierarchy_name(this).c_str());
            //     _except.activate (); // propagating
            // }
            GRAPH_EXEC;            // executing
            rmt_EndCPUSample();
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    }
    // } catch (exception& e) {
    //   warning (nullptr, e.what());
    // }
}
} // namespace djnn
