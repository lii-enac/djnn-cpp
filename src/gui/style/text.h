#pragma once

#include "abstract_style.h"
#include "style_types.h"

#include "gen/text_anchor.h"
#include "gen/abstract_prop_font_family.h"
#include "gen/abstract_prop_font_size.h"
#include "gen/abstract_prop_font_style.h"
#include "gen/abstract_prop_font_weight.h"

namespace djnn {
  class FontSize : public AbstractPropFontSize
  {
  public:
    FontSize (ParentProcess* parent, const string& name, djnLengthUnit unit, double size);
    FontSize (ParentProcess* parent, const string& name, int unit, double size);
    void impl_activate () override;
    void draw () override;
    FontSize* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  private:
  };

  class FontWeight : public AbstractPropFontWeight // AbstractStyle
  {
  public:
    FontWeight (ParentProcess* parent, const string& name, int weight);
    void impl_activate () override;
    void draw () override;
    FontWeight* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class FontStyle : public AbstractPropFontStyle // AbstractStyle
  {
  public:
    FontStyle (ParentProcess* parent, const string& name, djnFontSlope style);
    FontStyle (ParentProcess* parent, const string& name, int style);
    void impl_activate () override;
    void draw () override;
    FontStyle* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class FontFamily : public AbstractPropFontFamily // AbstractStyle
  {
  public:
    FontFamily (ParentProcess* parent, const string& name, const string& family);
    void impl_activate () override;
    void draw () override;
    FontFamily* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };
}