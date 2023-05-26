#ifndef HALL_EFFECT_H
#define HALL_EFFECT_H

#include "sensor.hpp"

#define HALL_PIN 2

const int ticksPerRev = 3;
const int calcTime = 10;

bool exists = false;
volatile int ticks = 0;

void increment() {
    ticks++;
}


namespace Sensor {
    class HallEffect : public Sensor {
    private:
        unsigned long prevTime = 0;

        bool inTick = false;
    public:
        HallEffect();
        ~HallEffect();

        void tick();
        double read();
    };

    HallEffect::HallEffect() {
        CHECK(!exists, "Only one hall effect sensor can be used.")

        attachInterrupt(digitalPinToInterrupt(HALL_PIN), increment, RISING);
        exists = true;
        // pinMode(HALL_PIN, INPUT);
    }

    HallEffect::~HallEffect() {

    }

    void HallEffect::tick() {
        // if (!digitalRead(HALL_PIN)) {
        //     if (!inTick) {
        //         ticks++;  
        //         inTick = true;
        //     }
        // }
        // else {
        //     inTick = false;
        // }

        // currentTime += 1.0 / 1000.0;
    }

    double HallEffect::read() {
        unsigned long currentTime = millis();
        int timeDelta = currentTime - prevTime;

        double time = (double)timeDelta / 1000.0;

        double rpm = ((double)ticks / (double)ticksPerRev) / (time / 60.0);

        prevTime = currentTime;
        ticks = 0;

        return rpm;
    }
}

#endif