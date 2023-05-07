

#ifndef CURRENT_H
#define CURRENT_H

// #define VIN A0
const float vcc    = 5.00;// supply voltage 5V or 3.3V
const float factor = 0.02;// 20mV/A is the factor
// int ledPin = 3;      // LED connected to digital pin 9
// int val = 0;         // variable to store the read value

#include "sensor.hpp"

namespace Sensor {
    class CurrentSensor : public Sensor {
    private:
        uint8_t vin;

        long totalReading;
        int readings;
    public:
        CurrentSensor(uint8_t pin);
        ~CurrentSensor();

        void tick();
        double read();
    };

    CurrentSensor::CurrentSensor(uint8_t pin) {
        vin = pin;
        pinMode(pin, INPUT);

        totalReading = 0;
        readings = 0;
    }

    CurrentSensor::~CurrentSensor() {

    }

    void CurrentSensor::tick() {
        totalReading += analogRead(vin);
        readings++;
    }

    double CurrentSensor::read() {
        //Robojax.com ACS758 Current Sensor 
        double average = (double)totalReading / (double)readings;
        double voltage = (5.0 / 1023.0) * (double)average;// Read the voltage from sensor
        // double a = voltage - (vcc * 0.5) + 0.007;// 0.007 is a value to make voltage zero when there is no current
        // double current = voltage / a;

        totalReading = 0;
        readings = 0;

        // return current;
        return voltage;
    }
}

#endif
