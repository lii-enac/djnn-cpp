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
#include <cmath>

#include "style.h"
#include "../../core/tree/int_property.h"
#include "../../core/ontology/coupling.h"

namespace djnn
{

// D65 Reference White
#define X_R 0.95047
#define Y_R 1.00000
#define Z_R 1.08883

#define KAPPA 903.3
#define EPSILON 0.008856

  static double
  f (double v)
  {
    if (v > EPSILON)
      return pow (v, 1.0 / 3.0);
    else
      return (KAPPA * v + 16) / 116;
  }

  RGBToLCHConverter::ConverterAction::ConverterAction (RGBToLCHConverter *p, const std::string &n) :
      Action (p, n), _p (p)
  {
    Process::finalize_construction ();
  }

  void
  RGBToLCHConverter::ConverterAction::impl_activate ()
  {
    double R = _p->_r->get_value () / 255.0;
    double G = _p->_g->get_value () / 255.0;
    double B = _p->_b->get_value () / 255.0;

    double rgb[3];
    rgb[0] = R <= 0.04045 ? (R / 12.92) : pow ((R + 0.055) / 1.055, 2.4);
    rgb[1] = G <= 0.04045 ? (G / 12.92) : pow ((G + 0.055) / 1.055, 2.4);
    rgb[2] = B <= 0.04045 ? (B / 12.92) : pow ((B + 0.055) / 1.055, 2.4);

    double matrix[3][3] =
      {
	{ 0.4124564, 0.3575761, 0.1804375 },
	{ 0.2126729, 0.7151522, 0.0721750 },
	{ 0.0193339, 0.1191920, 0.9503041 } };
    double X = rgb[0] * matrix[0][0] + rgb[1] * matrix[0][1] + rgb[2] * matrix[0][2];
    double Y = rgb[0] * matrix[1][0] + rgb[1] * matrix[1][1] + rgb[2] * matrix[1][2];
    double Z = rgb[0] * matrix[2][0] + rgb[1] * matrix[2][1] + rgb[2] * matrix[2][2];

    double f_x = f (X / X_R);
    double f_y = f (Y / Y_R);
    double f_z = f (Z / Z_R);
    double L = 116 * f_y - 16;
    double a = 500 * (f_x - f_y);
    double b = 200 * (f_y - f_z);

    double C = sqrt (a * a + b * b);
    double H = atan2 (b, a) * (180.0 / 3.141592653589793238463);
    if (H < 0)
      H += 360;

    _p->_l->set_value (L, true);
    _p->_c->set_value (C, true);
    _p->_h->set_value (H, true);
  }

  RGBToLCHConverter::RGBToLCHConverter (Process *p, const std::string &n) :
      Process (p, n)
  {
    _r = new IntProperty (this, "r", 0);
    _g = new IntProperty (this, "g", 0);
    _b = new IntProperty (this, "b", 0);
    _l = new DoubleProperty (this, "l", 0);
    _c = new DoubleProperty (this, "c", 0);
    _h = new DoubleProperty (this, "h", 0);
    _action = new ConverterAction (this, "action");
    _cr = new Coupling (_r, ACTIVATION, _action, ACTIVATION);
    _cr->disable ();
    _cg = new Coupling (_g, ACTIVATION, _action, ACTIVATION);
    _cg->disable ();
    _cb = new Coupling (_b, ACTIVATION, _action, ACTIVATION);
    _cb->disable ();

    Graph::instance ().add_edge (_r, _action);
    Graph::instance ().add_edge (_g, _action);
    Graph::instance ().add_edge (_b, _action);
    Graph::instance ().add_edge (_action, _l);
    Graph::instance ().add_edge (_action, _c);
    Graph::instance ().add_edge (_action, _h);
    add_state_dependency (_parent, _action);
    Process::finalize_construction ();
  }

  RGBToLCHConverter::~RGBToLCHConverter ()
  {
    remove_state_dependency (_parent, _action);
    Graph::instance ().remove_edge (_action, _h);
    Graph::instance ().remove_edge (_action, _c);
    Graph::instance ().remove_edge (_action, _l);
    Graph::instance ().remove_edge (_b, _action);
    Graph::instance ().remove_edge (_g, _action);
    Graph::instance ().remove_edge (_r, _action);

    delete _cb;
    delete _cg;
    delete _cr;
    delete _action;
    delete _h;
    delete _c;
    delete _l;
    delete _b;
    delete _g;
    delete _r;
  }

  void
  RGBToLCHConverter::impl_activate ()
  {
    _cr->enable ();
    _cg->enable ();
    _cb->enable ();
  }

  void
  RGBToLCHConverter::impl_deactivate ()
  {
    _cr->disable ();
    _cg->disable ();
    _cb->disable ();
  }

  LCHToRGBConverter::ConverterAction::ConverterAction (LCHToRGBConverter *p, const std::string &n) :
      Action (p, n), _p (p)
  {
    Process::finalize_construction ();
  }

  void
  LCHToRGBConverter::ConverterAction::impl_activate ()
  {
    double L = _p->_l->get_value ();
    double C = _p->_c->get_value ();
    double H = _p->_h->get_value ();

    double a = C * cos (H * 0.0174533);
    double b = C * sin (H * 0.0174533);

    double f_y = (L + 16) / 116.0;
    double f_z = f_y - (b / 200.0);
    double f_x = (a / 500.0) + f_y;

    double f_x_3 = pow (f_x, 3);
    double f_y_3 = pow (f_y, 3);
    double f_z_3 = pow (f_z, 3);
    double x_r = f_x_3 > EPSILON ? f_x_3 : (116 * f_x - 16) / KAPPA;
    double y_r = L > (KAPPA * EPSILON) ? f_y_3 : (L / KAPPA);
    double z_r = f_z_3 > EPSILON ? f_z_3 : (116 * f_z - 16) / KAPPA;

    double X = X_R * x_r;
    double Y = Y_R * y_r;
    double Z = Z_R * z_r;

    double invmatrix[3][3] =
      {
	{ 3.2404542, -1.5371385, -0.4985314 },
	{ -0.969266, 1.8760108, 0.04155600 },
	{ 0.0556434, -0.2040259, 1.05722520 } };

    double rgb[3];
    rgb[0] = X * invmatrix[0][0] + Y * invmatrix[0][1] + Z * invmatrix[0][2];
    rgb[1] = X * invmatrix[1][0] + Y * invmatrix[1][1] + Z * invmatrix[1][2];
    rgb[2] = X * invmatrix[2][0] + Y * invmatrix[2][1] + Z * invmatrix[2][2];

    int R = (int) 255 * (rgb[0] <= 0.0031308 ? rgb[0] * 12.92 : 1.055 * pow (rgb[0], 1 / 2.4) - 0.055);
    int G = (int) 255 * (rgb[1] <= 0.0031308 ? rgb[1] * 12.92 : 1.055 * pow (rgb[1], 1 / 2.4) - 0.055);
    int B = (int) 255 * (rgb[2] <= 0.0031308 ? rgb[2] * 12.92 : 1.055 * pow (rgb[2], 1 / 2.4) - 0.055);

    _p->_r->set_value (R < 0 ? 0 : (R > 255) ? 255 : R, true);
    _p->_g->set_value (G < 0 ? 0 : (G > 255) ? 255 : G, true);
    _p->_b->set_value (B < 0 ? 0 : (B > 255) ? 255 : B, true);
  }

  LCHToRGBConverter::LCHToRGBConverter (Process *p, const std::string &n) :
      Process (p, n)
  {
    _r = new IntProperty (this, "r", 0);
    _g = new IntProperty (this, "g", 0);
    _b = new IntProperty (this, "b", 0);
    _l = new DoubleProperty (this, "l", 0);
    _c = new DoubleProperty (this, "c", 0);
    _h = new DoubleProperty (this, "h", 0);
    _action = new ConverterAction (this, "action");
    _cl = new Coupling (_l, ACTIVATION, _action, ACTIVATION);
    _cl->disable ();
    _cc = new Coupling (_c, ACTIVATION, _action, ACTIVATION);
    _cc->disable ();
    _ch = new Coupling (_h, ACTIVATION, _action, ACTIVATION);
    _ch->disable ();

    Graph::instance ().add_edge (_l, _action);
    Graph::instance ().add_edge (_c, _action);
    Graph::instance ().add_edge (_h, _action);
    Graph::instance ().add_edge (_action, _r);
    Graph::instance ().add_edge (_action, _g);
    Graph::instance ().add_edge (_action, _b);
    add_state_dependency (_parent, _action);
    Process::finalize_construction ();
  }

  LCHToRGBConverter::~LCHToRGBConverter ()
  {
    remove_state_dependency (_parent, _action);
    Graph::instance ().remove_edge (_action, _b);
    Graph::instance ().remove_edge (_action, _g);
    Graph::instance ().remove_edge (_action, _r);
    Graph::instance ().remove_edge (_h, _action);
    Graph::instance ().remove_edge (_c, _action);
    Graph::instance ().remove_edge (_l, _action);

    delete _ch;
    delete _cc;
    delete _cl;
    delete _action;
    delete _h;
    delete _c;
    delete _l;
    delete _b;
    delete _g;
    delete _r;
  }

  void
  LCHToRGBConverter::impl_activate ()
  {
    _cl->enable ();
    _cc->enable ();
    _ch->enable ();
  }

  void
  LCHToRGBConverter::impl_deactivate ()
  {
    _cl->disable ();
    _cc->disable ();
    _ch->disable ();
  }
}

