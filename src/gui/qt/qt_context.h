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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include <QtGui/QBrush>
#include <QtGui/QFont>
#include <QtGui/QMatrix4x4>
#include <QtGui/QPainterPath>
#include <QtGui/QPen>
#include <QtGui/QTransform>
#include <utility>

#include "core/tree/component_observer.h"
#include "core/utils/containers.h"
#include "gui/shape/abstract_gshape.h"
#include "gui/style/style_types.h"

namespace djnn {
class QtContext {
    friend class QtBackend;

  public:
    QtContext ();
    QtContext (QtContext* parent);
    virtual ~QtContext ();
    int z_order () { return _z_order; }

  private:
    int          DEFAULT_DPI_RES;
    double       alpha;
    Qt::FillRule fillRule;
    QPen         pen;
    QBrush       brush;
    QMatrix4x4   matrix;
    QTransform   gradientTransform;
    QTransform   clipTransform;
    QFont        font;
    QPainterPath clip;
    double       factor[10];
    int          textAnchor;
    int          _z_order;
    unsigned int _pick_bitset;
    void         update_relative_units ();
    double       get_unit_factor (djnLengthUnit unit);

    void set_pick_bitset (pick_bit_mask MASK, pick_bit_shift SHIFT, unsigned int VALUE) { _pick_bitset = (_pick_bitset & ~MASK) | (VALUE << SHIFT); }
    int  get_pick_bitset (pick_bit_mask MASK, pick_bit_shift SHIFT) const { return (_pick_bitset & MASK) >> SHIFT; }

    bool pick_fill_undefined () const { return get_pick_bitset (PICK_FILL_UNDEFINED_MASK, PICK_FILL_UNDEFINED_SHIFT); }
    void set_pick_fill_undefined (bool VALUE) { set_pick_bitset (PICK_FILL_UNDEFINED_MASK, PICK_FILL_UNDEFINED_SHIFT, VALUE); }

    bool pick_outline_undefined () const { return get_pick_bitset (PICK_OUTLINE_UNDEFINED_MASK, PICK_OUTLINE_UNDEFINED_SHIFT); }
    void set_pick_outline_undefined (bool VALUE) { set_pick_bitset (PICK_OUTLINE_UNDEFINED_MASK, PICK_OUTLINE_UNDEFINED_SHIFT, VALUE); }

    bool pick_fill () const { return get_pick_bitset (PICK_FILL_MASK, PICK_FILL_SHIFT); }
    void set_pick_fill (bool VALUE) {
        set_pick_bitset (PICK_FILL_MASK, PICK_FILL_SHIFT, VALUE);
        set_pick_fill_undefined (false);
    }

    bool pick_outline () const { return get_pick_bitset (PICK_OUTLINE_MASK, PICK_OUTLINE_SHIFT); }
    void set_pick_outline (bool VALUE) {
        set_pick_bitset (PICK_OUTLINE_MASK, PICK_OUTLINE_SHIFT, VALUE);
        set_pick_outline_undefined (false);
    }
};

class QtContextManager : public ContextManager {
  public:
    QtContextManager ()
        : ContextManager () {
        ComponentObserver::instance ().add_draw_context_manager (this);
    };
    virtual ~QtContextManager (){};
    void       pop () override;
    void       push () override;
    QtContext* get_current ();

  private:
    djnnstl::vector<QtContext*> _context_list;
};

class QtVectorShapeToDraw {
  public:
    QtVectorShapeToDraw (int z_order)
        : _z_order (z_order) {}
    virtual ~QtVectorShapeToDraw ();
    void                                                       draw ();
    int                                                        z_order () { return _z_order; }
    void                                                       add_item (AbstractGShape* shape, QtContext* context);
    djnnstl::vector<djnnstl::pair<AbstractGShape*, QtContext>> shapes () { return _shapes; }

  private:
    djnnstl::vector<djnnstl::pair<AbstractGShape*, QtContext>> _shapes;
    int                                                        _z_order;
};

void                                         add_shape (AbstractGShape* shape, QtContext* context);
void                                         clear_shapes ();
extern djnnstl::vector<QtVectorShapeToDraw*> shapes_vectors;
extern int                                   z_processing_step;
extern QtContext*                            cur_context;

} /* namespace djnn */
