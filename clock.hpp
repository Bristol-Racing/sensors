//  A header guard prevents the file from being included twice
#ifndef CLOCK_H
#define CLOCK_H

//  Include the parent sensor
#include "sensor.hpp"

//  Include the clock library
#include <Wire.h>
#include "RTClib.h"

namespace Sensor {

    class Clock : public Sensor {

    private:
        RTC_DS3231 rtc; //  The real time clock object
        DateTime start; //  The time the clock was turned on

    public:
        Clock();    //  Called when a new sensor object is created
        ~Clock();   //  Called when a sensor object is destroyed

        void setup();   //  Connects to the RTC chip and sets it up

        void tick();    //  Both called by the sensor manager
        double report();
    };

    Clock::Clock() {
        //  Don't need to do anything when the object is created
    }

    Clock::~Clock() {
        //  Don't need to do anything when the object is destroyed
    }

    void Clock::setup() {
        //  Connect to the RTC chip and check it initialized correctly
        bool rtcStatus = rtc.begin();
        CHECK(rtcStatus == true, "RTC initialization failed");

        //  Ignore lost power error
        // CHECK(rtc.lostPower() == false, "RTC lost power. Needs resetting");

        //  Set start time
        start = rtc.now();
    }

    void Clock::tick() {
        //  Don't need to do anything for ticks
    }

    double Clock::report() {
        //  Called by the sensor manager whenever the time should be reported

        //  Check current time
        DateTime now = rtc.now();

        //  Calculate time since start
        TimeSpan span = now - start;

        return span.totalseconds();

    }
}

#endif
