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

#pragma once

#include "core/ontology/process.h"

namespace djnn {

class AbstractSerializer {
  public:
    virtual ~AbstractSerializer () {}
    using string = djnnstl::string;
    static void  pre_serialize (const CoreProcess* root, const string& format);
    static void  post_serialize (const CoreProcess* root);
    static void  compute_path (FatProcess* from, CoreProcess* to, string& buf);
    virtual void start (const string& name)                               = 0;
    virtual void text_attribute (const string& name, const string& value) = 0;
    virtual void int_attribute (const string& name, int value)            = 0;
    virtual void float_attribute (const string& name, double value)       = 0;
    void         cpptype_attribute (const string& name, const string& value) { text_attribute (name, value); }
    void         cpptype_attribute (const string& name, int value) { int_attribute (name, value); }
    void         cpptype_attribute (const string& name, double value) { float_attribute (name, value); }
    virtual void end () = 0;

  public:
    static const CoreProcess*  serializationRoot;
    static AbstractSerializer* serializer;
};

class XMLSerializer : public AbstractSerializer {
  public:
    void start (const string& classname) override;
    void text_attribute (const string& name, const string& value) override;
    void int_attribute (const string& name, int value) override;
    void float_attribute (const string& name, double value) override;
    void end () override;
};

class JSONSerializer : public AbstractSerializer {
  public:
    void start (const string& classname) override;
    void text_attribute (const string& name, const string& value) override;
    void int_attribute (const string& name, int value) override;
    void float_attribute (const string& name, double value) override;
    void end () override;
};

} // namespace djnn
