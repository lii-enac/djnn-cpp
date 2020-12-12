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

#include <chrono>
#include <iomanip> // put_time
#include <cassert>

namespace djnn
{
  class WallClock : public FatProcess
  {
      struct LazyIntProperty : public AbstractIntProperty {
          LazyIntProperty (FatProcess* parent, const std::string& name)
          : AbstractIntProperty (parent, name) { finalize_construction (parent, name); }
          protected:
            int& get_ref_value() override {
                _ref = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch ().count ();
                return _ref;
            }
            const int& get_ref_value() const override {
                return const_cast<LazyIntProperty&>(*this).get_ref_value();
            }
            int _ref;
      };
      struct LazyTextProperty : public AbstractTextProperty {
          LazyTextProperty (FatProcess* parent, const std::string& name)
          : AbstractTextProperty (parent, name) { finalize_construction (parent, name); }
          protected:
            std::string& get_ref_value() override {
                // code borrowed from https://stackoverflow.com/a/54918353
                std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                std::chrono::milliseconds now2 = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                struct tm currentLocalTime;
                //localtime_r(&currentTime, &currentLocalTime);
                currentLocalTime = *localtime(&currentTime); // not thread-safe
                char timeBuffer[80];
                std::size_t charCount { std::strftime( timeBuffer, 80,
                                         "%Y-%m-%d_%Hh%Mm%Ss",
                                          &currentLocalTime)
                         };
                assert(charCount);
                std::string res (' ', charCount);
                std::copy(timeBuffer, timeBuffer+charCount, res.begin());
                res.resize (charCount);
                res += '.' + std::to_string(now2.count() % 1000);
                _ref = res;
                return _ref;
            }
            const std::string& get_ref_value() const override {
                return const_cast<LazyTextProperty&>(*this).get_ref_value();
            }
            std::string _ref;
      };
    public:
        WallClock (ParentProcess* parent, const std::string& name)
        : FatProcess (name),
        _state (this, "state"),
        _state_text (this, "state_text")
        {
            finalize_construction (parent, name);
        }
    protected:
        virtual void impl_activate () override {}
        virtual void impl_deactivate () override {}
        LazyIntProperty _state;
        LazyTextProperty _state_text;
  };
}
