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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */


#include "../core/tree/component.h"
#include "../core/serializer/serializer.h"
#include "log.h"
#include "text.h"
#include "connector.h"

#include <iostream>

namespace djnn
{
  LogPrinter::LogPrinter (Process *p, const std::string& n, const std::string& label) :
      Component (p, n)
  {
    Process* tc = new TextCatenator (this, "catenator");
    Process* tp = new TextPrinter (this, "printer");
    alias (this, "label", tc->find_component("head"));
    alias (this, "input", tc->find_component("tail"));
    ((TextProperty*)this->find_component ("label"))->set_value (label, false);
    new Connector (this, "", tc, "output", tp, "input", false);
  }

  void
  LogPrinter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:logprinter");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("label", ((TextProperty*)this->find_component ("label"))->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}









