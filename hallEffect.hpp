
//  A header guard prevents the file from being included twice
#ifndef HALL_EFFECT_H
#define HALL_EFFECT_H

//  Include the parent sensor
#include "sensor.hpp"

//  The pin to read the hall effect ticks from
#define HALL_PIN 2

//  Number of magnet passes for each revolution
const int ticksPerRev = 3;

//  Guards against multiple hall effects
bool exists = false;

//  Counts the magnet ticks
volatile int ticks = 0;

//  Interrupt function, called whenever the hall effect detects a magnet
//  Interrupts cannot be class methods
void increment() {
    ticks++;
}


namespace Sensor {
    class HallEffect : public Sensor {
    private:
        unsigned long prevTime = 0;

    public:
        HallEffect();   //  Called when a new sensor object is created
        ~HallEffect();  //  Called when a sensor object is destroyed

        void tick();        //  Both called by the sensor manager
        double report();
    };

    HallEffect::HallEffect() {
        //  Checks that there is only one hall effect
        CHECK(!exists, "Only one hall effect sensor can be used.")
        exists = true;

        //  Attaches the interrupt function so that it is called whenever a magnet is detected
        attachInterrupt(digitalPinToInterrupt(HALL_PIN), increment, RISING);
    }

    HallEffect::~HallEffect() {
        //  Don't need to do anything when the object is destroyed
    }

    void HallEffect::tick() {
        //  Don't need to do anything for ticks
    }

    double HallEffect::report() {
        //  Called by the sensor manager whenever the rpm should be reported

        //  Calculate the time since the last report
        unsigned long currentTime = millis();
        int timeDelta = currentTime - prevTime;
        double time = (double)timeDelta / 1000.0;

        //  Calculate the rpm from the number of ticks and time
        double rpm = ((double)ticks / (double)ticksPerRev) / (time / 60.0);

        //  Update time and reset ticks
        prevTime = currentTime;
        ticks = 0;

        return rpm;
    }
}

#endif