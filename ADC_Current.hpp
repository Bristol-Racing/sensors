
//  A header guard prevents the file from being included twice
#ifndef CURRENT_H
#define CURRENT_H

//  Include the ADC library
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

//  Include the parent sensor
#include "sensor.hpp"

namespace Sensor {
    class CurrentSensor : public Sensor {

    private:
        int readings;   //  Stores the number of readings taken since the last report
        long totalReading;  //  Stores the sum of the readings taken since the last report
        //  Used the calculate the average reading

        Adafruit_ADS1115 ads1115;   //  The Analogue to Digital Converter object

        uint8_t Vref;
        uint8_t Vout;
        double off;     //  Offset and gradient, used to calibrate the current sensor
        double grad;    //  so accurate current readings can be calculated from the voltage it outputs

    public:
        CurrentSensor(uint8_t pin1, uint8_t pin2, double offset, double gradient);  //  Called when a new sensor object is created
        ~CurrentSensor();   //  Called when a sensor object is destroyed

        void setup();   //  Connects to the ADC chip and sets it up

        void tick();        //  Both called by the sensor manager
        double report();
    };

    CurrentSensor::CurrentSensor(uint8_t pin1, uint8_t pin2, double offset, double gradient) {
        //  Initialise readings to 0
        totalReading = 0;
        readings = 0;

        Vref = pin1;
        Vout = pin2;
        pinMode(Vref, INPUT);
        pinMode(Vout, INPUT);
        //  Store the calibrated offset and gradient
        off = offset;
        grad = gradient;
    }

    CurrentSensor::~CurrentSensor() {
        //  Don't need to do anything when the object is destroyed
    }

    void CurrentSensor::setup() {
        //  Connect to the ADC chip and check it initialised correctly
        bool status = ads1115.begin();
        CHECK(status == true, "ADC initialisation failed");

        //  Set gain to 4x
        //  This gives the best balance of accuracy and range for the current sensor
        ads1115.setGain(GAIN_FOUR);    // 4x gain   +/- 1.024V  1 bit = 0.5mV
    }

    void CurrentSensor::tick() {
        //  Called by the sensor manager whenever a new reading needs adding to the average

        //  Read from the ADC
        //  Compares pins 0 and 1 during the reading
        //  Pin 0 should be the current sensor reference voltage
        //  Pin 1 should be the current sensor output voltage
        //  The readings aren't accurate enough if the ADC isn't used in differential mode
        int16_t ref_reading;
        int16_t out_reading;
        //reading = ads1115.readADC_Differential_0_1();
        ref_reading = analogRead(Vref);
        out_reading = analogRead(Vout);

        int16_t diff_reading = out_reading - ref_reading;



        //  Adds the reading to the running total
        totalReading += diff_reading;
        readings++;
    }

    double CurrentSensor::report() {
        //  Called by the sensor manager whenever an average reading should be reported

        //  Compute the average voltage reading
        double average = (double)totalReading / (double)readings;

        // Ben trying something
        //double voltage = (3.3 / 1023.0) * (double) average;
        //voltage = voltage - (3.3 * 0.5) + 0.007;
        double voltage = average;
        double current = (voltage);

        //  Calculate the current
        //double current = off + grad * average;

        //  Reset the running totals ready for the next average to be computed
        totalReading = 0;
        readings = 0;

        //  Return the current to the sensor manager
        return current;
    }
}

#endif
