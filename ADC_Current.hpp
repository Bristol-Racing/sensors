// Calculated the current through LEM HO 100-S via using ADC on adafruit 

//  A header guard prevents the file from being included twice
#ifndef CURRENT_H
#define CURRENT_H
#include "sensor.hpp"//  Include the parent sensor

namespace Sensor {
    class CurrentSensor : public Sensor {
    private:
        int readings;       //  Stores the number of readings taken since the last report
        long totalReading;  //  Stores the sum of the readings taken since the last report
        uint8_t Vref;
        uint8_t Vout;
        double off;         //  Offset and gradient, used to calibrate the current sensor
        double grad;        //  so accurate current readings can be calculated from the voltage it outputs
    public:
        CurrentSensor(uint8_t pin1, uint8_t pin2, double offset, double gradient);  //  Called when a new sensor object is created
        ~CurrentSensor();   //  Called when a sensor object is destroyed
        void setup();       //  Connects to the ADC chip and sets it up
        void tick();        //  Both called by the sensor manager
        double report();
    };

    CurrentSensor::CurrentSensor(uint8_t pin1, uint8_t pin2, double offset, double gradient) {
        totalReading = 0;   //  Initialise readings to 0
        readings = 0;       //  Initialise readings to 0
        Vref = pin1;
        Vout = pin2;
        pinMode(Vref, INPUT);
        pinMode(Vout, INPUT);
        off = offset;       //  Store the calibrated offset
        grad = gradient;    // Store gradient
    }

    CurrentSensor::~CurrentSensor() {
        //  Don't need to do anything when the object is destroyed
    }

    void CurrentSensor::setup() {
        //
    }

    void CurrentSensor::tick() {
        //  Called by the sensor manager whenever a new reading needs adding to the average
        //  Compares pins 0 and 1 during the reading, to make a differential measurement
        int16_t ref_reading;
        int16_t out_reading;
        ref_reading = analogRead(Vref); //  Pin 0 should be the current sensor reference voltage
        out_reading = analogRead(Vout); //  Pin 1 should be the current sensor output voltage
        int16_t diff_reading = out_reading - ref_reading; //  The readings aren't accurate enough if the ADC isn't used in differential mode
        totalReading += diff_reading;   //  Adds the reading to the running total
        readings++;
    }

    double CurrentSensor::report() {
        //  Called by the sensor manager whenever an average reading should be reported
        double current = -1*(double)totalReading / (double)readings; //  average V reading
        totalReading = 0;   //  Reset the running totals ready for the next average to be computed
        readings = 0;
        return current;     //  Return the current to the sensor manager
    }
}

#endif
