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

#include <string>

namespace djnn {

  //using namespace std;  

  class AbstractSerializer
  {
  
  public:
  	static void pre_serialize (CoreProcess* root, const std::string& format);
  	static void post_serialize (CoreProcess* root);
    static  void compute_path (FatProcess* from, CoreProcess* to, std::string& buf);
  	virtual void start (const std::string& name) = 0;
  	virtual void text_attribute (const std::string& name, const std::string& value) = 0;
  	virtual void int_attribute (const std::string& name, int value) = 0;
  	virtual void float_attribute (const std::string& name, double value) = 0;
    void cpptype_attribute(const std::string& name, const std::string& value) { text_attribute(name, value); }
    void cpptype_attribute(const std::string& name, int value) { int_attribute(name, value); }
    void cpptype_attribute(const std::string& name, double value) { float_attribute(name, value); }
  	virtual void end () = 0;
  
  public:
  	static CoreProcess* serializationRoot;
  	static AbstractSerializer* serializer;

  };


  class XMLSerializer : public AbstractSerializer 
  {
  public:
  	void start (const std::string& classname) override;
  	void text_attribute (const std::string& name, const std::string& value) override;
  	void int_attribute (const std::string& name, int value) override;
  	void float_attribute (const std::string& name, double value) override;
  	void end () override;
  };

  
  class JSONSerializer : public AbstractSerializer 
  {
  public:
  	void start (const std::string& classname) override;
  	void text_attribute (const std::string& name, const std::string& value) override;
  	void int_attribute (const std::string& name, int value) override;
  	void float_attribute (const std::string& name, double value) override;
  	void end () override;
  };

}
