
//  A header guard prevents the file from being included twice
#ifndef CHARGE_H
#define CHARGE_H

//  Include the parent sensor
#include "sensor.hpp"

//  Includes the current sensor and manager to get readings from
#include "sensorManager.hpp"
#include "ADC_Current.hpp"

//  Calculates the charge used from the battery
//  Hopelessly inaccurate

namespace Sensor {
    class ChargeMonitor : public Sensor {
    private:
        SensorManager* m;   //  A reference to the sensor manager, to ask for the current readings from
        CurrentSensor* c;   //  A reference to the current sensor, to ask for the current readings from

        double ah;  //  The amp hours left in the battery
    public:
        ChargeMonitor(SensorManager* manager, CurrentSensor* current, double startingAmpHours);  //  Called when a new sensor object is created
        ~ChargeMonitor();   //  Called when a sensor object is destroyed

        void tick();        //  Both called by the sensor manager
        double report();
    };

    ChargeMonitor::ChargeMonitor(SensorManager* manager, CurrentSensor* current, double startingAmpHours) {
        //  Sets the sensor and manager references
        m = manager;
        c = current;

        //  The battery starts full
        ah = startingAmpHours;
    }

    ChargeMonitor::~ChargeMonitor() {
        //  Don't need to do anything when the object is destroyed
    }

    void ChargeMonitor::tick() {
        //  Don't need to do anything for ticks
    }

    double ChargeMonitor::report() {
        //  Called by the sensor manager whenever a new charge used should be reported

        //  Get the last reported current reading from the current sensor
        double current = m->getLastReport(c);

        //  Reports are once per second, so the current needs to be divided by the number of seconds in an hour
        //  to give the number of amp hours used 
        ah -= current * (1.0 / 60.0 / 60.0);

        //  Return the new remaining charge to the sensor manager
        return ah;
    }
}

#endif
