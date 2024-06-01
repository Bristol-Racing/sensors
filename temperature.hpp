// File that takes a temperature reading from an analog read and applies calibration

//  A header guard prevents the file from being included twice
#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "sensor.hpp"//  Include the parent sensor

//  Values from calibration
#define MINTEMP 15.0
#define MINMV 1939.0
#define MAXTEMP 100.0
#define MAXMV 997.0
#define TEMP_GRADIENT ((MAXMV - MINMV) / (MAXTEMP - MINTEMP))

namespace Sensor {
    class TemperatureSensor : public Sensor {
    private:
        uint8_t vin;    //  Voltage input pin
    public:
        TemperatureSensor(uint8_t pin);   //  Called when a new sensor object is created
        ~TemperatureSensor();  //  Called when a sensor object is destroyed
        void tick();        //  Both called by the sensor manager
        double report();
    };

    TemperatureSensor::TemperatureSensor(uint8_t pin) {
        //  Setup and store the input pin 
        vin = pin;
        pinMode(pin, INPUT);
    }

    TemperatureSensor::~TemperatureSensor() {
        //  Do nothing when the object is destroyed
    }

    void TemperatureSensor::tick() {
        //  No averaging applied
    }

    double TemperatureSensor::report() {
        // Takes an analog voltage reading, applies a calibration and returns the temperature

        //  Called by the sensor manager when a reading should be reported
        int reading = analogRead(vin); //  Read from the voltage input and convert it to a voltage (in millivolts)
        double voltage = (5000.0 / 1023.0) * (double)reading;
        double temperature = (voltage - MINMV) / TEMP_GRADIENT + MINTEMP;//  Calculate the temperature from the calibration values
        return temperature;
    }
}

#endif
