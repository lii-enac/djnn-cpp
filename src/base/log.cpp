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


#include "core/tree/component.h"
#include "core/serializer/serializer.h"
#include "log.h"


namespace djnn
{

#if NEW_LOG_PLAIN_OBJ
  LogPrinter::LogPrinter (FatProcess *parent, const std::string& name, const std::string& label) 
  : Component (p, n),
  _tc (this, "catenator"),
  _tp (this, "printer"),
  _c (this, "logprinter_connector", &_tc, "output", &_tp, "input", false)
  {
    alias (this, "label", _tc.find_child("head"));
    alias (this, "input", _tc.find_child("tail"));
    ((TextProperty*)this->find_child ("label"))->set_value (label, false);
  }
#else

  LogPrinter::LogPrinter (FatProcess *parent, const std::string& name, const std::string& label)
  : Component (parent, name)
  {
    _tc = new TextCatenator (this, "catenator");
    _tp = new TextPrinter (this, "printer");
    _c = new Connector (this, "logprinter_connector", _tc, "output", _tp, "input", false);
    alias (this, "label", _tc->find_child("head"));
    alias (this, "input", _tc->find_child("tail"));
    ((TextProperty*)this->find_child ("label"))->set_value (label, false);
  }
#endif

#ifndef DJNN_NO_SERIALIZE
  void
  LogPrinter::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:logprinter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("label", ((TextProperty*)this->find_child ("label"))->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif
}









