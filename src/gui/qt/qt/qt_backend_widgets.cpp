/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2023)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include <QtWidgets/QFileDialog>

#include "display/qt/qt_window.h"
#include "display/qt/qt_window_moc.h"
#include "gui/backend.h"
#include "gui/qt/qt_backend.h"
#include "gui/widgets/file_dialog.h"

namespace djnn {
void
QtBackend::open_dialog (FileDialog* dialog)
{
    const string& title  = dialog->title ().get_value ();
    const string& from   = dialog->from ().get_value ();
    const string& filter = dialog->filter ().get_value ();
    auto          frame  = dialog->get_frame ();
    QtWindow*     impl   = dynamic_cast<QtWindow*> (frame->win_impl ());
    if (impl == nullptr)
        return;
    impl->qwidget ()->set_building (true);
    QString filename = QFileDialog::getOpenFileName (impl->qwidget (),
                                                     QString::fromStdString (title), QString::fromStdString (from), QString::fromStdString (filter));
    impl->qwidget ()->set_building (false);
    if (!filename.isEmpty ())
        dialog->set_file_selected (filename.toStdString ());
}

void
QtBackend::save_dialog (FileDialog* dialog)
{
    const string& title  = dialog->title ().get_value ();
    const string& from   = dialog->from ().get_value ();
    const string& filter = dialog->filter ().get_value ();
    auto          frame  = dialog->get_frame ();
    QtWindow*     impl   = dynamic_cast<QtWindow*> (frame->win_impl ());
    if (impl == nullptr)
        return;
    impl->qwidget ()->set_building (true);
    QString filename = QFileDialog::getSaveFileName (impl->qwidget (),
                                                     QString::fromStdString (title), QString::fromStdString (from), QString::fromStdString (filter));
    impl->qwidget ()->set_building (false);
    if (!filename.isEmpty ())
        dialog->set_file_selected (filename.toStdString ());
}
} // namespace djnn