/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/tree/int_property.h"
#include "core/tree/text_property.h"
#include "core/utils/to_string.h"

#include <chrono>
#include <iomanip> // put_time
#include <cassert>

namespace djnn
{
  class WallClock : public FatProcess
  {
      struct WallClockIntProperty : public AbstractIntProperty {
          WallClockIntProperty (FatProcess* parent, const string& name)
          : AbstractIntProperty (parent, name) { finalize_construction (parent, name); }
          protected:
            int& get_ref_value() override {
                _ref = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch ().count ();
                return _ref;
            }
            const int& get_ref_value() const override {
                return const_cast<WallClockIntProperty&>(*this).get_ref_value();
            }
            int _ref;
      };
      struct WallClockTextProperty : public AbstractTextProperty {
          WallClockTextProperty (FatProcess* parent, const string& name)
          : AbstractTextProperty (parent, name) { finalize_construction (parent, name); }
          protected:
            string& get_ref_value() override {
                // code borrowed from https://stackoverflow.com/a/54918353
                std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                std::chrono::milliseconds now2 = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                struct tm currentLocalTime;
                //localtime_r(&currentTime, &currentLocalTime);
                currentLocalTime = *localtime(&currentTime); // not thread-safe
                char timeBuffer[80];
                string format = ((WallClock*)get_parent())->_format.get_value();
                auto ms_pos = format.find("%k"); // our own ms formater is %k
                if(ms_pos != string::npos) {
                    format = format.substr(0,ms_pos) + "%" + format.substr(ms_pos,string::npos);
                }
                std::size_t charCount = std::strftime( timeBuffer, 80,
                                            //"%Y-%m-%d_%Hh%Mm%Ss",
                                            format.c_str(),
                                            &currentLocalTime);
                assert(charCount);
                string res (' ', charCount);
                std::copy(timeBuffer, timeBuffer+charCount, res.begin());
                res.resize (charCount);

                if(ms_pos != string::npos) {
                    auto ms = now2.count() % 1000;
                    auto ms_str = djnn::to_string(ms);
                    if (ms<10) ms_str = "00" + ms_str;
                    else if (ms<100) ms_str = "0" + ms_str;

                    ms_pos = res.find("%k"); // our own ms formater is %k
                    res = res.substr(0,ms_pos) + ms_str + res.substr(ms_pos+2, string::npos);
                    //res.resize (charCount+1); // we have a 3-digit ms value, and we replace a 2-digit "%k" format 
                    //std::copy(ms_pos+2, res.end()-1, &res[ms_pos+3]);
                    //std::copy(&res[ms_pos], ms_str.begin(), ms_str.end());
                }

                _ref = res;
                return _ref;
            }
            const string& get_ref_value() const override {
                return const_cast<WallClockTextProperty&>(*this).get_ref_value();
            }
            string _ref;
      };
    public:
        WallClock (ParentProcess* parent, const string& name)
        : FatProcess (name),
        _state (this, "state"),
        _state_text (this, "state_text"),
        _format (this, "format", "%Y-%m-%d_%Hh%Mm%Ss")
        {
            finalize_construction (parent, name);
        }
    protected:
        virtual void impl_activate () override {}
        virtual void impl_deactivate () override {}
        WallClockIntProperty _state;
        WallClockTextProperty _state_text;
        TextProperty _format;
  };
}
