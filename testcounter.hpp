

#ifndef COUNTER_H
#define COUNTER_H

#include "sensor.hpp"

namespace Sensor {
    class CounterSensor : public Sensor {
    private:
        float count;
    public:
        CounterSensor();
        ~CounterSensor();

        void tick();
        float read();
    };

    CounterSensor::CounterSensor() {
        count = 0;
    }

    CounterSensor::~CounterSensor() {

    }

    void CounterSensor::tick() {

    }

    float CounterSensor::read() {
        count++;
        return count;
    }
}

#endif
