/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stéphane Convercy <stephane.convercy@enac.fr>
 *
 */

#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h" // DisplayBackend::instance
#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "gui/shape/sdf.h"

#include "core/control/blank.h"
#include "core/tree/list.h"
#include "display/window.h"
#include "gui/shape/path.h"
#include "gui/shape/path_clip.h"

#include "core/utils/error.h"
#include "utils/debug.h" // UNIMPL



namespace djnn
{
  static const char*
  ParseCoords (const char *v, int num, double *coord, int *numout)
  {
    char *p;
    int i = 0;

    /* read a list of coordinate pairs */
    while (*v && i < num)
      {

        /* skip comma and separating white space */
        while (*v == ',' || *v == ' ' || *v == '\t' || *v == '\n' || *v == '\r')
          ++v;

        /* try and read coord */
        *coord = strtod (v, &p);
        if (p == v) break;

        v = p;
        ++i;
        ++coord;
      }

    *numout = i;
    return v;
  }

  int
  parse_path (Path *p, const char *path_spec)
  {
    double initx = 0., inity = 0.;
    double curx = 0., cury = 0.;
    double ctrlx = 0., ctrly = 0.;
    double coords[7];
    int rel;
    int num;
    int firstPt = 1;
    char prevItem = ' ', curItem = ' ';

    /* read a list of commands and their parameters */
    while (*path_spec)
      {

        /* skip leading white space */
        while (*path_spec == ' ' || *path_spec == '\t' || *path_spec == '\n'
            || *path_spec == '\r')
          ++path_spec;

        /* try and read command */
        rel = 0;
        curItem = *path_spec;

        switch (*path_spec++)
          {

          case 'm':
            rel = 1;
          case 'M':
            path_spec = ParseCoords (path_spec, 2, coords, &num);
            if (num != 2) goto error;
            curx = rel ? curx + coords[0] : coords[0];
            cury = rel ? cury + coords[1] : coords[1];
            new PathMove (p, "", curx, cury);
            if (firstPt)
              {
                firstPt = 0;
                initx = curx;
                inity = cury;
              }
            while (num == 2)
              {
                path_spec = ParseCoords (path_spec, 2, coords, &num);
                if (num == 2)
                  {
                    curx = rel ? curx + coords[0] : coords[0];
                    cury = rel ? cury + coords[1] : coords[1];
                    new PathLine (p, "", curx, cury);
                  }
              }
            if (num != 0) goto error;
            break;

          case 'l':
            rel = 1;
          case 'L':
            do
              {
                path_spec = ParseCoords (path_spec, 2, coords, &num);
                if (num != 2) break;
                curx = rel ? curx + coords[0] : coords[0];
                cury = rel ? cury + coords[1] : coords[1];
                new PathLine (p, "", curx, cury);
              }
            while (num == 2);
            if (num != 0) goto error;
            break;

          case 'v':
            rel = 1;
          case 'V':
            do
              {
                path_spec = ParseCoords (path_spec, 1, coords, &num);
                if (num != 1) break;
                cury = rel ? cury + coords[0] : coords[0];
                new PathLine (p, "", curx, cury);
              }
            while (num == 1);
            break;

          case 'h':
            rel = 1;
          case 'H':
            do
              {
                path_spec = ParseCoords (path_spec, 1, coords, &num);
                if (num != 1) break;
                curx = rel ? curx + coords[0] : coords[0];
                new PathLine (p, "", curx, cury);
              }
            while (num == 1);
            break;

          case 'c':
            rel = 1;
          case 'C':
            do
              {
                path_spec = ParseCoords (path_spec, 6, coords, &num);
                if (num != 6) break;
                if (rel)
                  {
                    int i = 0;
                    while (i < 6)
                      {
                        coords[i++] += curx;
                        coords[i++] += cury;
                      }
                  }
                curx = coords[4];
                cury = coords[5];
                new PathCubic (p, "", coords[0], coords[1], coords[2],
                               coords[3], coords[4], coords[5]);
                ctrlx = coords[2];
                ctrly = coords[3];
              }
            while (num == 6);
            if (num != 0) goto error;
            break;

          case 's':
            rel = 1;
          case 'S':
            do
              {
                path_spec = ParseCoords (path_spec, 4, coords, &num);
                if (num != 4) break;
                if (prevItem == 'c' || prevItem == 'C' || prevItem == 's'
                    || prevItem == 'S')
                  {
                    ctrlx = 2 * curx - ctrlx;
                    ctrly = 2 * cury - ctrly;
                  }
                else
                  {
                    ctrlx = curx;
                    ctrly = cury;
                  }
                if (rel)
                  {
                    int i = 0;
                    while (i < 4)
                      {
                        coords[i++] += curx;
                        coords[i++] += cury;
                      }
                  }
                curx = coords[2];
                cury = coords[3];
                new PathCubic (p, "", ctrlx, ctrly, coords[0], coords[1],
                               coords[2], coords[3]);
                ctrlx = coords[0];
                ctrly = coords[1];
              }
            while (num == 4);
            if (num != 0) goto error;
            break;

          case 'q':
            rel = 1;
          case 'Q':
            do
              {
                path_spec = ParseCoords (path_spec, 4, coords, &num);
                if (num != 4) break;
                if (rel)
                  {
                    int i = 0;
                    while (i < 4)
                      {
                        coords[i++] += curx;
                        coords[i++] += cury;
                      }
                  }
                curx = coords[2];
                cury = coords[3];
                ctrlx = coords[0];
                ctrly = coords[1];
                new PathQuadratic (p, "", coords[0], coords[1], coords[2],
                                   coords[3]);
              }
            while (num == 4);
            if (num != 0) goto error;
            break;

          case 't':
            rel = 1;
          case 'T':
            do
              {
                path_spec = ParseCoords (path_spec, 2, coords, &num);
                if (num != 2) break;
                if (prevItem == 'q' || prevItem == 'Q' || prevItem == 't'
                    || prevItem == 'T')
                  {
                    ctrlx = 2 * curx - ctrlx;
                    ctrly = 2 * cury - ctrly;
                  }
                else
                  {
                    ctrlx = curx;
                    ctrly = cury;
                  }
                curx = rel ? curx + coords[0] : coords[0];
                cury = rel ? cury + coords[1] : coords[1];
                new PathQuadratic (p, "", ctrlx, ctrly, curx, cury);
              }
            while (num == 2);
            if (num != 0) goto error;
            break;

          case 'a':
            rel = 1;
          case 'A':
            do
              {
                path_spec = ParseCoords (path_spec, 7, coords, &num);
                if (num != 7) break;
                if (rel)
                  {
                    coords[5] += curx;
                    coords[6] += cury;
                  }
                curx = coords[5];
                cury = coords[6];
                new PathArc (p, "", coords[0], coords[1], coords[2], coords[3],
                             coords[4], coords[5], coords[6]);
              }
            while (num == 7);
            if (num != 0) goto error;
            break;

          case 'z':
          case 'Z':
            new PathClosure (p, "");
            curx = initx;
            cury = inity;
            firstPt = 1;
            while (*path_spec == ' ' || *path_spec == '\t' || *path_spec == '\n'
                || *path_spec == '\r')
              ++path_spec;
            break;

          default:
            fprintf (stderr, "SVG path parser: unknown command '%c' in path\n",
                     *(path_spec - 1));
          }
        prevItem = curItem;
      }
    return 1;

error: fprintf (stderr, "SVG path parser: error in path coordinates\n");
    return 0;
  }

  PathPoint::PathPoint (CoreProcess* parent, const string& name, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{.x=x, .y=y},
      _cx(nullptr), _cy(nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      error(this, "Parent of PathPoint must be <Path>");
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathPoint::~PathPoint ()
  {
    remove_edge (_cx);
    remove_edge (_cy);

    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  CoreProcess*
  PathPoint::find_child_impl (const string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  PathPoint::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathPoint::impl_deactivate ()
  {
    disable (_cx);
    disable (_cy);
    AbstractGObj::impl_deactivate ();
  }

  void
  PathLine::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_line (raw_props.x, raw_props.y);
    }
  }

  PathLine*
  PathLine::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new PathLine (nullptr, get_name (), raw_props.x, raw_props.y);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }

  void
  PathMove::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_move (raw_props.x, raw_props.y);
    }
  }

  PathMove*
  PathMove::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new PathMove (nullptr, get_name (), raw_props.x, raw_props.y);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }

  PathClosure::PathClosure (CoreProcess* parent, const string& name) :
    AbstractGObj (parent, name)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      error(this, "Parent of path closure must be Path");
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  void
  PathClosure::draw ()
  {
    Backend::instance ()->draw_path_closure ();
  }

  PathClosure*
  PathClosure::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new PathClosure (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }

  PathQuadratic::PathQuadratic (CoreProcess* parent, const string& name, double x1, double y1, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{ .x1=x1, .y1=y1, .x=x, .y=y },
      _cx1 (nullptr), _cy1 (nullptr), _cx (nullptr), _cy (nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      error(this, "Parent of path quadratic must be Path");
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathQuadratic::~PathQuadratic ()
  {
    remove_edge (_cx1);
    remove_edge (_cy1);
    remove_edge (_cx);
    remove_edge (_cy);

    delete _cx1;
    delete _cy1;
    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("x1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  CoreProcess*
  PathQuadratic::find_child_impl (const string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x1") {
      coupling=&_cx1;
      rawp=&raw_props.x1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp=&raw_props.y1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  PathQuadratic::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    //auto _frame = get_frame ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cx1, damaged);
    enable (_cy1, damaged);
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathQuadratic::impl_deactivate ()
  {
    disable (_cx1);
    disable (_cy1);
    disable (_cx);
    disable (_cy);

    AbstractGObj::impl_deactivate ();
  }

  void
  PathQuadratic::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_quadratic (raw_props.x1, raw_props.y1, raw_props.x, raw_props.y);
    }
  }

  PathQuadratic*
  PathQuadratic::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new PathQuadratic (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x, raw_props.y);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }


  PathCubic::PathCubic (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2, .x=x, .y=y},
      _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr), _cx (nullptr), _cy (nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      error(this,  "Parent of path cubic must be Path");
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathCubic::~PathCubic ()
  {
    remove_edge (_cx1);
    remove_edge (_cy1);
    remove_edge (_cx2);
    remove_edge (_cy2);
    remove_edge (_cx);
    remove_edge (_cy);

    delete _cx1;
    delete _cy1;
    delete _cx2;
    delete _cy2;
    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("x1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x2");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y2");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  CoreProcess*
  PathCubic::find_child_impl (const string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x1") {
      coupling=&_cx1;
      rawp=&raw_props.x1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp=&raw_props.y1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x2") {
      coupling=&_cx2;
      rawp=&raw_props.x2;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y2") {
      coupling=&_cy2;
      rawp=&raw_props.y2;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);    

    return res;
  }

  void
  PathCubic::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    //auto _frame = get_frame ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cx1, damaged);
    enable (_cy1, damaged);
    enable (_cx2, damaged);
    enable (_cy2, damaged);
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathCubic::impl_deactivate ()
  {
    disable (_cx1);
    disable (_cy1);
    disable (_cx2);
    disable (_cy2);
    disable (_cx);
    disable (_cy);
    AbstractGObj::impl_deactivate ();
  }

  void
  PathCubic::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_cubic (raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2, raw_props.x, raw_props.y);
    }
  }

  PathCubic*
  PathCubic::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new PathCubic (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2, raw_props.x, raw_props.y);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }

  PathArc::PathArc (CoreProcess* parent, const string& name, double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y) :
      AbstractGObj (parent, name),
      raw_props{.rx=rx, .ry=ry, .rotx=rotx, .fl=fl, .swfl=swfl, .x=x, .y=y},
      _crx (nullptr), _cry (nullptr), _crotx (nullptr), _cfl (nullptr), _cswfl(nullptr), _cx (nullptr), _cy (nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      error(this, "Parent of path arc must be Path");
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathArc::~PathArc ()
  {
    remove_edge (_crotx);
    remove_edge (_cfl);
    remove_edge (_cswfl);
    remove_edge (_crx);
    remove_edge (_cry);
    remove_edge (_cx);
    remove_edge (_cy);

    delete _crotx;
    delete _cfl;
    delete _cswfl;
    delete _crx;
    delete _cry;
    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("rotx");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("fl");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("swfl");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("rx");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("ry");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  CoreProcess*
  PathArc::find_child_impl (const string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="rotx") {
      coupling=&_crotx;
      rawp=&raw_props.rotx;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="fl") {
      coupling=&_cfl;
      rawp=&raw_props.fl;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="swfl") {
      coupling=&_cswfl;
      rawp=&raw_props.swfl;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="rx") {
      coupling=&_crx;
      rawp=&raw_props.rx;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="ry") {
      coupling=&_cry;
      rawp=&raw_props.ry;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;

    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);    

    return res;
  }

  void
  PathArc::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    //auto _frame = get_frame ();
    auto * damaged = get_frame ()->damaged ();
    enable (_crotx, damaged);
    enable (_cfl, damaged);
    enable (_cswfl, damaged);
    enable (_crx, damaged);
    enable (_cry, damaged);
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathArc::impl_deactivate ()
  {
    disable (_crotx);
    disable (_cfl);
    disable (_cswfl);
    disable (_crx);
    disable (_cry);
    disable (_cx);
    disable (_cy);
    AbstractGObj::impl_deactivate ();
  }

  void
  PathArc::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_arc (raw_props.rx, raw_props.ry, raw_props.rotx, raw_props.fl, raw_props.swfl, raw_props.x, raw_props.y);
    }
  }

  PathArc*
  PathArc::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new PathArc (nullptr, get_name (), raw_props.rx, raw_props.ry, raw_props.rotx, raw_props.fl, raw_props.swfl, raw_props.x, raw_props.y);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }

  Path::Path (CoreProcess* parent, const string& name) :
      AbstractGShape (parent, name)
  {
    _items = new List (this, "items");
    _bounding_box = new Blank (this, "bounding_box");
    _bbx = new DoubleProperty (nullptr, "bbx", 0);
    _bby = new DoubleProperty (nullptr, "bby", 0);
    _bbw = new DoubleProperty (nullptr, "bbw", 0);
    _bbh = new DoubleProperty (nullptr, "bbh", 0); 
    _bounding_box->add_symbol ("x", _bbx);
    _bounding_box->add_symbol ("y", _bby);
    _bounding_box->add_symbol ("width", _bbw);
    _bounding_box->add_symbol ("height", _bbh);
    invalidate_bounding_box ();

    // save pointer to th eGUIStructureHolder of items to call the correct draw function
    _items_GH = gui_structure_observer->structure_map()[_items];

    finalize_construction (parent, name);
  }

  Path::Path (CoreProcess* parent, const string& name, const string& path_spec) :
        Path (parent, name)
  {
    parse_path (this, path_spec.c_str());
  }


  Path::~Path ()
  {
    delete _bounding_box;

    delete _bbx;
    delete _bby;
    delete _bbw;
    delete _bbh;
    delete _items;
  }

  void
  Path::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    _items->activate ();
  }

  void
  Path::impl_deactivate ()
  {
    _items->deactivate ();
    AbstractGShape::impl_deactivate ();
  }

  void
  Path::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_path (this);
      AbstractGShape::post_draw ();
    }
  }

  /*Path*
  Pathimpl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    Path* clone = new Path (nullptr, get_name ());
    for (auto p: _items->children ()) {
      clone->items ()->add_child (p->clone (), "");
    }
    return clone;
  }*/

  Path* 
  Path::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto * clone = new Path (nullptr, get_name ());
    for (auto c : _items->children ()) {
      auto cclone = c->impl_clone (origs_clones);
      //origs_clones[c] = cclone;
      clone->items ()->add_child (cclone , this->find_child_name(c));
    }
    origs_clones[this] = clone;
    return clone;
  }

  void
  PathClip::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_clip (this);
    }
  }

  /*PathClip*
  PathClipimpl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    PathClip* clone = new PathClip (nullptr, get_name ());
    for (auto p: _items->children ()) {
      clone->items ()->add_child (p->clone (), "");
    }
    return clone;
  }*/

  PathClip* 
  PathClip::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto * clone = new PathClip (nullptr, get_name ());
    for (auto c : _items->children ()) {
      auto cclone = c->impl_clone (origs_clones);
      //origs_clones[c] = cclone;
      clone->items ()->add_child (cclone , this->find_child_name(c));
    }
    origs_clones[this] = clone;
    return clone;
  }

  void
  Path::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    // warning : does not work until the path is drawn
    // also, if a point in the path is modified, bbox is not recomputed until the path is drawn again
    if (is_bounding_box_valid ()) {
      x = _bbx->get_value ();
      y = _bby->get_value ();
      w = _bbw->get_value ();
      h = _bbh->get_value ();
    } else {
      UNIMPL;
      // should be computed for picking...
      //for (auto p: _items->children ()) { 
    }
  }

  double
  Path::sdf (double x, double y) const
  {
    UNIMPL;
    return 0;
  }

  void
  Path::set_bounding_box (double x, double y, double w, double h)
  {
    if (_bbx->get_double_value() != x)
      _bbx->set_value (x, true);
    if (_bby->get_double_value() != y)
      _bby->set_value (y, true);
    if (_bbw->get_double_value() != w)
      _bbw->set_value (w, true);
    if (_bbh->get_double_value() != h)
      _bbh->set_value (h, true);
  }
}
