

#ifndef CURRENT_H
#define CURRENT_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>


#include "sensor.hpp"

namespace Sensor {
    class CurrentSensor : public Sensor {
    private:
        uint8_t vin;

        long totalReading;
        int readings;
        Adafruit_ADS1115 ads1115;

        double off;
        double grad;
    public:
        CurrentSensor(double offset, double gradient);
        ~CurrentSensor();

        void setup();

        void tick();
        double read();
    };

    CurrentSensor::CurrentSensor(double offset, double gradient) {
        totalReading = 0;
        readings = 0;

        off = offset;
        grad = gradient;
    }

    CurrentSensor::~CurrentSensor() {

    }

    void CurrentSensor::setup() {
        bool status = ads1115.begin();
        CHECK(status == true, "ADC initialisation failed");

        //  ads1115.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV (default)

        // ads1115.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
        // ads1115.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV
         ads1115.setGain(GAIN_FOUR);    // 4x gain   +/- 1.024V  1 bit = 0.5mV
        // ads1115.setGain(GAIN_EIGHT);   // 8x gain   +/- 0.512V  1 bit = 0.25mV
        // ads1115.setGain(GAIN_SIXTEEN); // 16x gain  +/- 0.256V  1 bit = 0.125mV
    }

    void CurrentSensor::tick() {
        int16_t results;

        results = ads1115.readADC_Differential_0_1();
        totalReading += results;
        readings++;
    }

    double CurrentSensor::read() {
        double average = (double)totalReading / (double)readings;
        double current = off + grad * average;

        totalReading = 0;
        readings = 0;

        return current;
    }
}

#endif
