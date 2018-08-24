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
#include <iostream>
#include "../tree/process.h"

using namespace std;

namespace djnn {
  

  class AbstractSerializer
  {
  
  public:
  	static void pre_serialize (Process* root, const string& format);
  	static void post_serialize (Process* root);
    static  void compute_path (Process* from, Process* to, string& buf);
  	virtual void start (const string& name) = 0;
  	virtual void text_attribute (const string& name, const string& value) = 0;
  	virtual void int_attribute (const string& name, int value) = 0;
  	virtual void float_attribute (const string& name, double value) = 0;
  	virtual void end () = 0;
  
  public:
  	static Process* serializationRoot;
  	static AbstractSerializer* serializer;

  };


  class XMLSerializer : public AbstractSerializer 
  {
  public:
  	void start (const string& classname) override;
  	void text_attribute (const string& name, const string& value) override;
  	void int_attribute (const string& name, int value) override;
  	void float_attribute (const string& name, double value) override;
  	void end () override;
  };

  
  class JSONSerializer : public AbstractSerializer 
  {
  public:
  	void start (const string& classname) override;
  	void text_attribute (const string& name, const string& value) override;
  	void int_attribute (const string& name, int value) override;
  	void float_attribute (const string& name, double value) override;
  	void end () override;
  };

}
