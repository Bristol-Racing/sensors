
//  A header guard prevents the file from being included twice
#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

//  Include the parent sensor
#include "sensor.hpp"

namespace Sensor {
    class Potentiometer : public Sensor {
    private:
        uint8_t vin;    //  Voltage input pin

        long totalReading;  //  Stores the sum of the readings taken since the last report
        int readings;   //  Stores the number of readings taken since the last report

        bool rev;   //  Is the potentiometer reversed?

    public:
        Potentiometer(uint8_t pin, bool reverse = false);   //  Called when a new sensor object is created
        ~Potentiometer();  //  Called when a sensor object is destroyed

        void tick();        //  Both called by the sensor manager
        double report();
    };

    Potentiometer::Potentiometer(uint8_t pin, bool reverse = false) {
        //  Setup and store the input pin 
        vin = pin;
        pinMode(pin, INPUT);

        //  Reset the running total
        totalReading = 0;
        readings = 0;

        //  Store whether the potentiometer is reversed
        rev = reverse;
    }

    Potentiometer::~Potentiometer() {
        //  Don't need to do anything when the object is destroyed
    }

    void Potentiometer::tick() {
        //  Called by the sensor manager whenever a new reading needs adding to the average

        //  Read from the voltage input and add it to the running total
        totalReading += analogRead(vin);
        readings++;
    }

    double Potentiometer::report() {
        //  Called by the sensor manager whenever an average reading should be reported

        //  Compute the average reading and convert it to a voltage
        double average = (double)totalReading / (double)readings;
        double voltage = (5.0 / 1023.0) * (double)average;

        //  Reverse it if necessary
        if (rev) {
            voltage = 5.0 - voltage;
        }

        //  Reset the running totals
        totalReading = 0;
        readings = 0;

        return voltage;
    }
}

#endif
