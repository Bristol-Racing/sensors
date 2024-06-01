// File for reading voltage using a potential divider and the arduino analog 

//  A header guard prevents the file from being included twice
#ifndef VOLTAGE_H
#define VOLTAGE_H
#include "sensor.hpp" //  Include the parent sensor

namespace Sensor {
    class VoltageSensor : public Sensor {
    private:
        uint8_t vin;        //  Voltage input pin
        double gradient;    //  Gradient produced by potential divider, used for calibration
    public:
        VoltageSensor(uint8_t pin, double grad);   //  Called when a new sensor object is created
        ~VoltageSensor();   //  Called when a sensor object is destroyed
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
        double reading = (double)analogRead(vin);
        double voltage = reading * (5.0/1023.0) * gradient;// 5/1023 -- is the ratio of voltage to bits
        return voltage;
    }
}

#endif
