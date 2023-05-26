// Date and time functions using a DS3231 RTC connected via I2C and Wire lib


#ifndef clock_H
#define clock_H

#include "sensor.hpp"

#include <Wire.h> 
#include "RTClib.h"

namespace Sensor {

    class Clock : public Sensor {
    private:
        RTC_DS3231 rtc;
        bool rtcStatus;
        DateTime start;
    public:
        Clock();
        ~Clock();

        void setup();

        void tick();
        double read();
    };

    Clock::Clock() {

    }

    Clock::~Clock() {

    }

    void Clock::setup() {
        rtcStatus = rtc.begin();
        CHECK(rtcStatus == true, "RTC initialisation failed");

        CHECK(rtc.lostPower() == false, "RTC lost power. Needs resetting");

        start = rtc.now();

        TimeSpan offset(0, 1, 0, 0);

        start = start - offset;
    }

    void Clock::tick() {

    }

    double Clock::read() {
        DateTime now = rtc.now();
        TimeSpan span = now - start;
        // Serial.println(now.unixtime());
        return span.totalseconds();
    }
}

#endif
