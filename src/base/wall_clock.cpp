#include <chrono>
#include <iomanip> // put_time
#include <cassert>

#include "wall_clock.h"
#include "core/utils/to_string.h"


namespace djnn {
    
int&
WallClock::WallClockIntProperty::get_ref_value()
{
    _ref = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch ().count ();
    return _ref;
}

const int&
WallClock::WallClockIntProperty::get_ref_value() const
{
    return const_cast<WallClockIntProperty&>(*this).get_ref_value();
}


string&
WallClock::WallClockTextProperty::get_ref_value()
{
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

const string&
WallClock::WallClockTextProperty::get_ref_value() const
{
    return const_cast<WallClockTextProperty&>(*this).get_ref_value();
}

}