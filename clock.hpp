//  A header guard prevents the file from being included twice
#ifndef CLOCK_H
#define CLOCK_H
#include "sensor.hpp"//  Include the parent sensor
#include <Wire.h>//  Include the clock library
#include "RTClib.h"

namespace Sensor {
    class Clock : public Sensor {
    private:
        RTC_DS3231 rtc; //  The real time clock object
        DateTime start; //  The time the clock was turned on
    public:
        Clock();        //  Called when a new sensor object is created
        ~Clock();       //  Called when a sensor object is destroyed
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
        start = rtc.now(); //  Set start time
    }

    void Clock::tick() {
        //  Don't need to do anything for ticks
    }

    double Clock::report() {
        //  Called by the sensor manager whenever the time should be reported
        DateTime now = rtc.now();           //  Check current time
        uint32_t unixNow = now.unixtime();  //  Calculate time since start
        uint32_t relTime = 1704067200;      // unixtime of 01/01/2024 00:00:00
        double relUnixNow = unixNow - relTime; // time in seconds relative to 01/01/2024 00:00:00
        return relUnixNow;
    }
}

#endif