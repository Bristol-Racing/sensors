
//  A header guard prevents the file from being included twice
#ifndef VOLTAGE_H
#define VOLTAGE_H

//  Include the parent sensor
#include "sensor.hpp"

namespace Sensor {
    class VoltageSensor : public Sensor {
    private:
        uint8_t vin;    //  Voltage input pin
        double gradient;    //  Gradient produced by potential divider, used for calibration
    public:

        VoltageSensor(uint8_t pin, double grad);   //  Called when a new sensor object is created
        ~VoltageSensor();  //  Called when a sensor object is destroyed

        void tick();        //  Both called by the sensor manager
        double report();
    };

    VoltageSensor::VoltageSensor(uint8_t pin, double grad) {
        //  Setup and store the input pin and gradient
        vin = pin;
        gradient = grad;
        pinMode(pin, INPUT);
    }

    VoltageSensor::~VoltageSensor() {
        //  Don't need to do anything when the object is destroyed
    }

    void VoltageSensor::tick() {
        //  We don't bother averaging
    }

    double VoltageSensor::report() {
        //  Called by the sensor manager whenever a reading should be reported

        //  Read from the input pin and calculate the voltage before the potential divider
        double voltage = (double)analogRead(vin) * gradient;

        return voltage;
    }
}

#endif
