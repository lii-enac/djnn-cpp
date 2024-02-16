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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display/drm/drm_window.h"

#include "core/utils/error.h"

namespace djnn {

DRMWindow::DRMWindow (djnn::Window* win, const string& title, double x, double y, double w, double h)
    : _window (win), _conn (nullptr), is_activated (false)
{
    for (auto c : conn_list) {
        if (c->is_connected ()) {
            _conn = c;
            break;
        }
    }
    if (_conn == nullptr)
        error (nullptr, "No display available for frame " + title);
}

int
DRMWindow::set_dpy_connection ()
{
    auto* p = _window->get_display ();
    _conn   = dynamic_cast<DRMConnector*> (p);
    if (_conn == nullptr) {
        return 0;
    } else if (!_conn->is_connected ()) {
        _conn = nullptr;
        return 0;
    }
    return 1;
}

DRMWindow::~DRMWindow ()
{
    _conn = nullptr;
}

buff*
DRMWindow::get_next_buff ()
{
    if (_conn == nullptr)
        error (nullptr, "invalid connector");
    return _conn->get_next_buff ();
}

void
DRMWindow::flip_page ()
{
    if (_conn == nullptr)
        error (nullptr, "invalid connector");
    _conn->flip_page ();
}

void
DRMWindow::flip_page_fb (uint32_t fb)
{
    if (_conn == nullptr)
        error (nullptr, "invalid connector");
    _conn->flip_page_fb (fb);
}

bool
DRMWindow::is_waiting_for_vblank () const
{
    if (_conn != nullptr)
        return _conn->is_waiting_for_vblank ();
    else {
        error (nullptr, "Bad DRMWindow initialization");
        return 0;
    }
}

} // namespace djnn
